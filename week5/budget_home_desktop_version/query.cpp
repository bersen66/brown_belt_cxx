#include "query.h"
#include <sstream>

using namespace std;

QueryType IdentifyQueryType(const string& str) {
	if (str == "ComputeIncome") {
		return QueryType::COMPUTE_INCOME; 
	} else if (str == "Earn") {
		return QueryType::EARN;
	} else if (str == "PayTax") {
		return QueryType::PAY_TAX;
	}
}

Query ParseQuery(const std::string& line, DateFormat format) {
	Query result;

	stringstream in(line);
	string query_type, date_from, date_to;
	in >> query_type >> date_from >> date_to;

	result.type = IdentifyQueryType(query_type);
	result.from = ParseDate(date_from, format);
	result.to = ParseDate(date_to, format);

	switch(result.type) {
		case QueryType::EARN: {
			in >> result.value;
		} break;
	}
	
	return result;		
}
