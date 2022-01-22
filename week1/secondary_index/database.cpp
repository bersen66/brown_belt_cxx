//
// Created by ortur on 21.01.2022.
//

#include "database.h"
#include "record.h"


bool Database::Put(const Record &record) {

    if (records.count(record.id) != 0) {
        return UNSUCCESS_FLAG;
    }

    auto[node_iterator, is_inserted] = records.try_emplace(record.id, record);
    if (is_inserted) {
        auto v = node_iterator->second;
        location_info.emplace(
                make_pair(node_iterator->first,
                          Locator(
                                  karma_index.insert({node_iterator->second.karma, &node_iterator->second}),
                                  timestamp_index.insert({node_iterator->second.timestamp, &node_iterator->second}),
                                  user_index.insert({node_iterator->second.user, &node_iterator->second})
                          )
                ));
        return SUCCESS_FLAG;
    }
    return UNSUCCESS_FLAG;

}

const Record *Database::GetById(const std::string &id) const {
    auto record = records.find(id);
    if (record == records.end()) { return nullptr; }
    return &((record)->second);
}

bool Database::Erase(const std::string &id) {
    auto resolved_record = records.find(id);

    if (resolved_record == records.end())
        return UNSUCCESS_FLAG;

    auto locator_it = location_info.find(id);
    if (locator_it != location_info.end()) {
        auto&[identidier, locator] = *locator_it;

        user_index.erase(locator.user_it);
        timestamp_index.erase(locator.timestamp_it);
        karma_index.erase(locator.karma_it);
        location_info.erase(id);
        records.erase(id);
    }


    return SUCCESS_FLAG;
}
