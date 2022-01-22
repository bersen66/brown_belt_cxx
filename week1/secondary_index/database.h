//
// Created by ortur on 21.01.2022.
//

#ifndef BROWN_BELT_DATABASE_H
#define BROWN_BELT_DATABASE_H

#include <string>
#include <unordered_map>
#include <map>

#include "record.h"


class Database {
public:
    bool Put(const Record &record);

    const Record *GetById(const std::string &id) const;

    bool Erase(const std::string &id);

    template<typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        if (low > high) { return; }

        IterateThrough(
                timestamp_index.lower_bound(low),
                timestamp_index.upper_bound(high),
                callback
        );
    }

    template<typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        if (low > high) { return; }
        IterateThrough(
                karma_index.lower_bound(low),
                karma_index.upper_bound(high),
                callback
        );
    }

    template<typename Callback>
    void AllByUser(const std::string &user, Callback callback) const {

        auto[range_begin, range_end] =
        user_index.equal_range(user);
        if (range_begin == range_end && range_begin == user_index.end())
            return;
        IterateThrough(
                range_begin,
                range_end,
                callback
        );

    }

private:

    template<typename Iterator, typename Callback>
    void IterateThrough(Iterator range_begin, Iterator range_end, Callback callback) const {
        for (auto &curr = range_begin; curr != range_end && callback(*curr->second); curr++) {}
    }

private:
    enum {
        UNSUCCESS_FLAG = 0,
        SUCCESS_FLAG = 1
    };
private:

    std::multimap<Karma, const Record *> karma_index;
    std::multimap<Timestamp, const Record *> timestamp_index;
    std::multimap<User, const Record *> user_index;
    std::unordered_map<Id, Record> records;

private:
    using karma_index_iterator      = std::multimap<Karma, const Record *>::iterator;
    using timestamp_index_iterator  = std::multimap<Timestamp, const Record *>::iterator;
    using user_index_iterator       = std::multimap<User, const Record *>::iterator;

    struct Locator {
        karma_index_iterator         karma_it;
        timestamp_index_iterator     timestamp_it;
        user_index_iterator          user_it;

        Locator(karma_index_iterator karma, timestamp_index_iterator timestamp, user_index_iterator user)
                : karma_it(karma), timestamp_it(timestamp), user_it(user) {}
    };

    std::unordered_map<Id, Locator> location_info;
};


#endif //BROWN_BELT_DATABASE_H
