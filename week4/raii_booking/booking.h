//
// Created by ortur on 18.02.2022.
//

#ifndef BROWN_BELT_BOOKING_H
#define BROWN_BELT_BOOKING_H

namespace RAII {
    template<typename Provider>
    class Booking {
    public:
        Booking(Provider* provider, int counter)
            : provider_(provider), id_(counter){}

        Booking(Booking&& that)
            : provider_(that.provider_), id_(that.id_)
        {
            that.provider_ = nullptr;
        }

        Booking& operator=(Booking&& other) {
            id_ = other.id_;
            provider_ = other.provider_;

            other.provider_ = nullptr;
            return *this;
        }

        Booking(const Booking&) = delete;
        Booking& operator=(const Booking&) = delete;


        ~Booking() {
            if (provider_)
                provider_->CancelOrComplete(*this);
        }

    private:
        Provider* provider_;
        int id_;
    };
}

#endif //BROWN_BELT_BOOKING_H
