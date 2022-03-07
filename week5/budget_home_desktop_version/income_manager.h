#ifndef BROWN_BELT_INCOME_MANAGER
#define BROWN_BELT_INCOME_MANAGER

#include "date.h"
#include <map>

using MoneyType = double;

class IncomeManager {
public:
	IncomeManager() = default;
	
	void Earn(const Date& from, const Date& to, MoneyType value);
	MoneyType ComputeIncome(const Date& from, const Date& to) const;
	void PayTax(const Date& from, const Date& to, double percent = 0.13); 

private:
	template<typename Callback>
	void IterateThrough(const Date& begin, const Date& end, Callback callback);

	template<typename Callback>
	void IterateThrougPeriod(const Date& begin, const Date& end, Callback callback) const;

private:
	std::map<Date, MoneyType> spendings_info_;
};


#endif
