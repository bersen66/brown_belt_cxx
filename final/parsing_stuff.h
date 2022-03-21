#ifndef BROWN_BELT_FINAL_PARSING_STUFF_H
#define BROWN_BELT_FINAL_PARSING_STUFF_H

#include <string_view>
#include <optional>
#include <vector>
#include <string>

std::pair<std::string_view, std::optional<std::string_view>> SplitTwoStrict(std::string_view line_to_split,
                                                                            std::string_view delimeter = " ");

std::vector<std::string_view> SplitLineIntoTokensViews(std::string_view line, std::string_view delimeter = ", ");

std::vector<std::string> SplitLineIntoTokens(std::string_view line, std::string_view delimeter = ", ");

std::pair<std::string_view, std::string_view> SplitTwo(std::string_view s, std::string_view delimiter = " ");

std::string_view ReadToken(std::string_view& s, std::string_view delimiter = " ");

double ConvertToDouble(std::string_view line);

#endif //BROWN_BELT_FINAL_PARSING_STUFF_H
