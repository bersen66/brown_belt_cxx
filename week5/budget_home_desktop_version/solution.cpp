//
// Created by ortur on 07.03.2022.
//
#include <iostream>
#include <iomanip>
#include <cstddef>
#include <string>
#include <tuple>
#include <sstream>
#include <ctime>

using namespace std;



#define JANUARY     1
#define FEBRUARY    2
#define MARCH       3
#define APRIL       4
#define MAY         5
#define JUNE        6
#define JULY        7
#define AUGUST      8
#define SEPTEMBER   9
#define OCTOBER     10
#define NOVEMBER    11
#define DECEMBER    12


struct Day {
    size_t value;

    Day() = default;

    explicit Day(size_t value)
            : value(value){}
};

struct Month {
    size_t value;

    Month() = default;

    explicit Month(size_t value)
            : value(value){}

    Month NextMonth() const {
        if (value == DECEMBER) {
            return Month(JANUARY);
        }
        return Month(value+1);
    }
};

struct Year {
    size_t value;

    Year() = default;

    explicit Year(size_t value)
            : value(value){}

    bool IsLeap() const {return (value % 4 == 0 && (value % 400 == 0 || value % 100 != 0));}
};

bool operator<(const Day& lhs, const Day& rhs);
bool operator<(const Month& lhs, const Month& rhs);
bool operator<(const Year& lhs, const Year& rhs);

bool operator==(const Day& lhs, const Day& rhs);
bool operator==(const Month& lhs, const Month& rhs);
bool operator==(const Year& lhs, const Year& rhs);

bool operator!=(const Day& lhs, const Day& rhs);
bool operator!=(const Month& lhs, const Month& rhs);
bool operator!=(const Year& lhs, const Year& rhs);


enum class DateFormat {
    DMY,
    YMD,
};

class Date {
public:
    Date() = default;

    Date(const Date& other)
            : day_(other.day_), month_(other.month_), year_(other.year_){}

    Date(Day dd, Month mm, Year yy)
            : day_(dd), month_(mm), year_(yy){}

    Day GetDay() const;
    Month GetMonth() const;
    Year GetYear() const;

    void SetDayValue(size_t value);
    void SetMonthValue(size_t value);
    void SetYearValue(size_t value);

    void SetDay(Day day);
    void SetMonth(Month month);
    void SetYear(Year year);


    [[nodiscard]] std::string AsString(DateFormat format = DateFormat::DMY) const;

    time_t AsTimestamp() const;

    Date NextDate() const;

    friend std::ostream& operator << (std::ostream& out, const Date& date);
private:
    size_t DaysInMonth(Month m) const;
    bool BelongsToThisMonth(Day d) const;
    bool BelongsToThisYear(Day d) const;

private:
    Day day_;
    Month month_;
    Year year_;
};

bool operator<(const Date& lhs, const Date& rhs);
bool operator==(const Date& lhs, const Date& rhs);
bool operator!=(const Date& lhs, const Date& rhs);

Date ParseDate(const std::string& src, DateFormat format = DateFormat::DMY);

int ComputDaysDiff(const Date& lhs, const Date& rhs);

std::ostream& operator << (std::ostream& out, const Date& date);


using namespace std;

bool operator<(const Day& lhs, const Day& rhs) {
    return lhs.value < rhs.value;
}

bool operator<(const Month& lhs, const Month& rhs){
    return lhs.value < rhs.value;
}

bool operator<(const Year& lhs, const Year& rhs) {
    return lhs.value < rhs.value;
}



bool operator==(const Day& lhs, const Day& rhs) {
    return lhs.value == rhs.value;
}

bool operator==(const Month& lhs, const Month& rhs) {
    return lhs.value == rhs.value;
}

bool operator==(const Year& lhs, const Year& rhs) {
    return lhs.value == rhs.value;
}





bool operator!=(const Day& lhs, const Day& rhs) {
    return lhs.value != rhs.value;
}

bool operator!=(const Month& lhs, const Month& rhs) {
    return lhs.value != rhs.value;
}

bool operator!=(const Year& lhs, const Year& rhs) {
    return lhs.value != rhs.value;
}



Day Date::GetDay() const {
    return day_;
}

Month Date::GetMonth() const {
    return month_;
}

Year Date::GetYear() const {
    return year_;
}

void Date::SetDayValue(size_t value) {
    day_.value = value;
}

void Date::SetMonthValue(size_t value) {
    month_.value = value;
}

void Date::SetYearValue(size_t value) {
    year_.value = value;
}

void Date::SetDay(Day day) {
    day_ = day;
}

void Date::SetMonth(Month month) {
    month_ = month;
}

void Date::SetYear(Year year) {
    year_ = year;
}

time_t Date::AsTimestamp() const {
    std::tm t;
    t.tm_sec   = 0;
    t.tm_min   = 0;
    t.tm_hour  = 0;
    t.tm_mday  = day_.value;
    t.tm_mon   = month_.value - 1;
    t.tm_year  = year_.value - 1900;
    t.tm_isdst = 0;
    return mktime(&t);
}

[[nodiscard]] string Date::AsString(DateFormat format) const {
    stringstream result;

    switch (format) {
        case DateFormat::DMY: {
            result << day_.value << '-'
                   << month_.value << '-'
                   << year_.value;
        } break;

        case DateFormat::YMD: {
            result << year_.value << '-'
                   << month_.value << '-'
                   << day_.value;
        } break;
    }

    return result.str();
}

Date Date::NextDate() const {
    Date result;

    Day next_day = Day(day_.value + 1);
    if (BelongsToThisYear(next_day)) {
        if (BelongsToThisMonth(next_day)) {
            result.SetDay(next_day);
            result.SetMonth(month_);
            result.SetYear(year_);
        } else {
            result.SetDayValue(1);
            result.SetMonth(month_.NextMonth());
            result.SetYear(year_);
        }
    } else {
        result.SetYearValue(year_.value + 1);
        result.SetMonth(Month(JANUARY));
        result.SetDayValue(1);
    }


    return result;
}

size_t Date::DaysInMonth(Month m) const {

    if (m.value == FEBRUARY) {
        if (year_.IsLeap()) {
            return 29;
        } else {
            return 28;
        }
    }

    if (m.value == JANUARY || m.value == MARCH
        || m.value == MAY || m.value == JULY
        || m.value == AUGUST || m.value == OCTOBER
        || m.value == DECEMBER) {
        return 31;
    }

    return 30;
}

bool Date::BelongsToThisMonth(Day d) const {
    return d.value <= DaysInMonth(month_);
}

bool Date::BelongsToThisYear(Day d) const {
    if (month_.value < DECEMBER) {
        return true;
    }
    return d.value <= 31;
}


bool operator<(const Date& lhs, const Date& rhs) {
    return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) <
           make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool operator==(const Date& lhs, const Date& rhs) {
    return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) ==
           make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool operator!=(const Date& lhs, const Date& rhs) {
    return !(lhs == rhs);
}



Date ParseDate(const std::string& src, DateFormat format) {
    size_t year, month, day;
    stringstream ss(src);

    switch (format) {
        case DateFormat::DMY: {
            ss >> day;
            ss.ignore();
            ss >> month;
            ss.ignore();
            ss >> year;
        } break;

        case DateFormat::YMD: {
            ss >> year;
            ss.ignore();
            ss >> month;
            ss.ignore();
            ss >> day;
        } break;
    }

    return Date(Day(day), Month(month), Year(year));
}


int ComputDaysDiff(const Date& lhs, const Date& rhs) {
    const time_t timestamp_to = lhs.AsTimestamp();
    const time_t timestamp_from = rhs.AsTimestamp();
    static const int SECONDS_IN_DAY = 60 * 60 * 24;
    return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
}

std::ostream &operator<<(std::ostream &out, const Date &date) {
    out << date.AsString();
    return out;
}

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
    IterateThrough(from, to,[&percent](MoneyType& val){
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






int main() {
    IncomeManager im;
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
