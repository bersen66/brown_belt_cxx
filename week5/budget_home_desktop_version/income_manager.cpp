#include "income_manager.h"

#include <vector>
#include <iostream>


void BudgetSystem::Earn(const Date& from, const Date& to, MoneyType value) {
    double daily_income = value / abs(ComputDaysDiff(to.NextDate(), from));
    IterateThrough(from, to,[daily_income](MoneyType& val){
        val += daily_income;
    });
}

MoneyType BudgetSystem::ComputeIncome(const Date& from, const Date& to) const {
    MoneyType result = 0.0;
    IterateThrougPeriod(from, to, [&result](const MoneyType& val){
        result += val;
    });
    return result;
}


void BudgetSystem::PayTax(const Date& from, const Date& to, double percent) {
    IterateThrough(from, to, [percent](MoneyType& val){
        val *= (1.0 - percent);
    });
}

	
template<typename Callback>
void BudgetSystem::IterateThrough(const Date& begin, const Date& end, Callback callback) {
    for (auto current = begin; current < end.NextDate(); current = current.NextDate()) {
        callback(budget_info_[current]);
    }
}

template<typename Callback>
void BudgetSystem::IterateThrougPeriod(const Date& begin, const Date& end, Callback callback) const {
    auto start = budget_info_.lower_bound(begin);
    auto finish = budget_info_.upper_bound(end);

    for (auto it = start; it != finish; it++) {
        auto& [date, value] = *it;
        callback(value);
    }
}


