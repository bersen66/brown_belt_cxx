#include "income_manager.h"

#include <vector>
#include <iostream>


void IncomeManager::Earn(const Date& from, const Date& to, MoneyType value) {
    double daily_income = value / abs(ComputDaysDiff(to.NextDate(), from));
    IterateThrough(from, to,[daily_income](MoneyType& val){
        val += daily_income;
    });
}

MoneyType IncomeManager::ComputeIncome(const Date& from, const Date& to) const {
    MoneyType result = 0.0;
    IterateThrougPeriod(from, to, [&result](const MoneyType& val){
        result += val;
    });
    result = result;
    return result;
}


void IncomeManager::PayTax(const Date& from, const Date& to, double percent) {
    IterateThrough(from, to, [percent](MoneyType& val){
        val *= (1.0 - percent);
    });
}

	
template<typename Callback>
void IncomeManager::IterateThrough(const Date& begin, const Date& end, Callback callback) { 
    for (auto current = begin; current < end.NextDate(); current = current.NextDate()) {
        callback(spendings_info_[current]);
    }
}

template<typename Callback>
void IncomeManager::IterateThrougPeriod(const Date& begin, const Date& end, Callback callback) const {
    auto start = spendings_info_.lower_bound(begin);
    auto finish = spendings_info_.upper_bound(end);

    for (auto it = start; it != finish; it++) {
        auto& [date, value] = *it;
        callback(value);
    }
}


