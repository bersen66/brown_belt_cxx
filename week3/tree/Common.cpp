#include "Common.h"
#include <string>

std::string AddBraces(const std::string& val,
                      char left_brace = '(',
                      char right_brace = ')' ) {
    return left_brace + val + right_brace;
}


class ValueNode : public Expression {
public:

    explicit ValueNode(int value)
        : value_(value){}

    explicit ValueNode(ExpressionPtr val)
        : subtree_(std::move(val)) {}

    [[nodiscard]] int Evaluate() const override {
        return value_;

    }

    [[nodiscard]] std::string ToString() const override {
        return std::to_string(value_);
    }

private:

    int value_;
    ExpressionPtr subtree_ = nullptr;
};


class SumNode : public Expression {
public:

    SumNode(ExpressionPtr lhs, ExpressionPtr rhs)
        : left_son(std::move(lhs)), right_son(std::move(rhs)) {}

    int Evaluate() const override {
        return right_son -> Evaluate() +
               left_son -> Evaluate();
    }

    [[nodiscard]] std::string ToString() const override {
        return AddBraces(left_son ->ToString()) + '+' +
               AddBraces(right_son -> ToString());
    }

private:
    ExpressionPtr left_son;
    ExpressionPtr right_son;
};

class ProductNode : public Expression {
public:

    ProductNode(ExpressionPtr lhs, ExpressionPtr rhs)
    : left_son(std::move(lhs)), right_son(std::move(rhs)) {}

    int Evaluate() const override {
        return left_son -> Evaluate() *
               right_son -> Evaluate();
    }

    [[nodiscard]] std::string ToString() const override {
        return AddBraces(left_son ->ToString()) + '*' +
               AddBraces(right_son -> ToString());
    }
private:
    ExpressionPtr left_son;
    ExpressionPtr right_son;
};

ExpressionPtr Value(int value) {
    return std::make_unique<ValueNode>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<SumNode>(move(left), move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<ProductNode>(move(left), move(right));

}
