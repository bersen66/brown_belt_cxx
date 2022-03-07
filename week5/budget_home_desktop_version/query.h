#ifndef BROWN_BELT_QUERY
#define BROWN_BELT_QUERY

#include "date.h"

enum class QueryType {
	COMPUTE_INCOME,
	EARN,
	PAY_TAX,
};

struct Query {
	QueryType type;
	Date from;
	Date to;
	double value;
};


QueryType IdentifyQueryType(const std::string& str);
Query ParseQuery(const std::string& line, DateFormat format = DateFormat::YMD);
		
#endif
