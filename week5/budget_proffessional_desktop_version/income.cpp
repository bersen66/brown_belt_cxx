#include "income.h"

void Income::Earn(MoneyType value) {
	income_ += value;
}

void Income::Spend(MoneyType value) {
	spended_ += value;
}

void Income::PayTax(size_t percent) {
	auto multiplier = (1.0 - static_cast<double>(percent/100.0));
	income_ *= multiplier;
}

MoneyType Income::ComputeProfit() const {
	return income_ - spended_;
}
