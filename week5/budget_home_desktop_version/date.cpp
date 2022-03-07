#include "date.h"
#include <tuple>
#include <sstream>
#include <ctime>


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