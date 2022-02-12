//
// Created by ortur on 08.02.2022.
//

#ifndef BROWN_BELT_ANIMALS_H
#define BROWN_BELT_ANIMALS_H

#include <string>

class Animal {
public:
    virtual std::string Voice() const {
        return "Not implemented yet";
    }
    virtual ~Animal() = default;
};


class Tiger: public Animal {
    std::string Voice() const override {
        return "Rrrr";
    }
};


class Wolf: public Animal {
    std::string Voice() const override {
        return "Wooo";
    }
};


class Fox: public Animal {
    std::string Voice() const override {
        return "Tyaf";
    }
};

#endif //BROWN_BELT_ANIMALS_H
