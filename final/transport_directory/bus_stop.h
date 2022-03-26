#ifndef BROWN_BELT_BUS_STOP_H
#define BROWN_BELT_BUS_STOP_H

#include <string>

#include "sphere_point.h"

class BusStop : public SpherePoint{
public:
    BusStop(const std::string& name, double latitude = 0, double longitude = 0);
    BusStop(std::string&& name, double  latitude, double longitude);

    BusStop() = default;
    BusStop(const BusStop& other) = default;
    BusStop(BusStop&& other) = default;
    BusStop& operator=(BusStop&& other) = default;
    BusStop& operator=(const BusStop& other) = default;

	const std::string& GetName() const;
	
	void SetName(std::string_view name);

private:
    std::string name_;
};

std::ostream& operator<<(std::ostream& out, const BusStop& stop);
std::optional<BusStop> ParseBusStop(std::string_view input);

bool operator==(const BusStop& lhs, const BusStop& rhs);

namespace std {
  template<>
  struct hash<BusStop> {
      size_t operator()(const BusStop& stop) const {
          size_t base = 37;
          size_t A = std::hash<std::string>{}(stop.GetName());
          return base * base * A;
      }
  };
};

#endif //BROWN_BELT_BUS_STOP_H
