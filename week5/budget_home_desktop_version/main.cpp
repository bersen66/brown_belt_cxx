#include <iostream>
#include <iomanip>

#include "date.h"
#include "query.h"
#include "income_manager.h"

using namespace std;

int main() {
    BudgetSystem im;
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
                cout << setprecision(25) <<im.ComputeIncome(query.from, query.to) << endl;
            } break;

            case QueryType::PAY_TAX: {
                im.PayTax(query.from, query.to);
            } break;

            case QueryType::EARN: {
                im.Earn(query.from, query.to, query.value);
            } break;

            default: {} break;
        }

    }
	return 0;
}
