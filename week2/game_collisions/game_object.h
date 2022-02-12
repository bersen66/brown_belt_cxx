//
// Created by ortur on 29.01.2022.
//

#ifndef BROWN_BELT_GAME_OBJECT_H
#define BROWN_BELT_GAME_OBJECT_H
#pragma once

class Unit;
class Building;
class Tower;
class Fence;

struct GameObject {
    virtual ~GameObject() = default;

    virtual bool Collide(const GameObject& that) const = 0;
    virtual bool CollideWith(const Unit& that) const = 0;
    virtual bool CollideWith(const Building& that) const = 0;
    virtual bool CollideWith(const Tower& that) const = 0;
    virtual bool CollideWith(const Fence& that) const = 0;
};

bool Collide(const GameObject& first, const GameObject& second);
#endif //BROWN_BELT_GAME_OBJECT_H
