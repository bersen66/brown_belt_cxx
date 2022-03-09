#ifndef BROWN_BELT_INCOME_MANAGER
#define BROWN_BELT_INCOME_MANAGER

#include "date.h"
#include "income.h"

#include <map>

class BudgetSystem {
public:
	BudgetSystem() = default;
	
	void Earn(const Date& from, const Date& to, MoneyType value);
	MoneyType ComputeIncome(const Date& from, const Date& to) const;
	void PayTax(const Date& from, const Date& to, size_t percent);
    void Spend(const Date& from, const Date& to, MoneyType value);
private:
	template<typename Callback>
	void IterateThrough(const Date& begin, const Date& end, Callback callback);

	template<typename Callback>
	void IterateThrougPeriod(const Date& begin, const Date& end, Callback callback) const;

private:
	std::map<Date, Income> budget_info_;

};


#endif
