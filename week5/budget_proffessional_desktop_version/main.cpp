#include <iostream>
#include <iomanip>

#include "date.h"
#include "query.h"
#include "budget_system.h"

using namespace std;

int main() {
    BudgetSystem bs;
    int Q;
    cin >> Q;
    std::cout.precision(25);
    for (int i = 0; i < Q; i++) {

        string line;
        while (line.empty()) {
            getline(cin, line);
        }

        auto query = ParseQuery(line);

        switch(query.type) {
            case QueryType::COMPUTE_INCOME: {
                cout << setprecision(25) << bs.ComputeIncome(query.from, query.to) << endl;
            } break;

            case QueryType::PAY_TAX: {
                bs.PayTax(query.from, query.to, query.percentage);
            } break;

            case QueryType::EARN: {
                bs.Earn(query.from, query.to, query.value);
            } break;

            case QueryType::SPEND: {
                bs.Spend(query.from, query.to, query.value);
            } break;

            default: {} break;
        }

    }
	return 0;
}
