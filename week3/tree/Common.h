//
// Created by ortur on 12.02.2022.
//

#ifndef BROWN_BELT_COMMON_H
#define BROWN_BELT_COMMON_H
#pragma once

#include <memory>
#include <string>

// Базовый класс арифметического выражения
class Expression {
public:
    virtual ~Expression() = default;
    // Вычисляет значение выражения
    virtual int Evaluate() const = 0;

    // Форматирует выражение как строку
    // Каждый узел берётся в скобки, независимо от приоритета
    virtual std::string ToString() const = 0;

};

using ExpressionPtr = std::unique_ptr<Expression>;


// Функции для формирования выражения
ExpressionPtr Value(int value);
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right);
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right);

#endif //BROWN_BELT_COMMON_H
