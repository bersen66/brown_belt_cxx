//
// Created by ortur on 21.01.2022.
//

#ifndef BROWN_BELT_RECORD_H
#define BROWN_BELT_RECORD_H

#include <string>

using Id = std::string;
using User = std::string;
using Karma = int;
using Timestamp = int;

struct Record {
    std::string id;
    std::string title;
    std::string user;
    int timestamp;
    int karma;
};

#endif //BROWN_BELT_RECORD_H
