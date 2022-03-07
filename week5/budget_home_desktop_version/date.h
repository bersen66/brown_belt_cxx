#ifndef BROWN_BELT_BDGT_H
#define BROWN_BELT_BDGT_H

#include <cstddef>
#include <string>

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

#endif
