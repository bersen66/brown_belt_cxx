#include "budget_system.h"
#include "income.h"


void BudgetSystem::Earn(const Date& from, const Date& to, MoneyType value) {
    double daily_income = value / abs(ComputDaysDiff(to.NextDate(), from));
    IterateThrough(from, to,[daily_income](Income& keeper){
        keeper.Earn(daily_income);
    });
}

MoneyType BudgetSystem::ComputeIncome(const Date& from, const Date& to) const {
    MoneyType result = 0.0;
    IterateThrougPeriod(from, to, [&result](const Income& val){
        result += val.ComputeProfit();
    });
    return result;
}


void BudgetSystem::PayTax(const Date& from, const Date& to, size_t percent) {
    IterateThrough(from, to, [percent](Income& val){
        val.PayTax(percent);
    });
}

void BudgetSystem::Spend(const Date &from, const Date &to, MoneyType value) {
    double daily_spending = value / abs(ComputDaysDiff(to.NextDate(), from));
    daily_spending = daily_spending;
    IterateThrough(from, to,[daily_spending](Income& keeper){
        keeper.Spend(daily_spending);
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




