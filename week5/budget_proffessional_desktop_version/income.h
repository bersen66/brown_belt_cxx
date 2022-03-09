#ifndef BROWN_BELT_INCOME
#define BROWN_BELT_INCOME

#include <cstddef>

using MoneyType = double;

class Income { 
public: 
	void Earn(MoneyType value);
	void Spend(MoneyType value);
	void PayTax(size_t percent);
    MoneyType ComputeProfit() const;
private:
	MoneyType income_;
	MoneyType spended_;
};

#endif
