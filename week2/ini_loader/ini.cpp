#include "ini.h"

namespace Ini {
    Section &Document::AddSection(const std::string& name) {
        sections.insert(std::move(make_pair(name, Section())));
        return sections.at(name);
    }

    const Section &Document::GetSection(const std::string &name) const {
        return sections.at(name);
    }

    size_t Document::SectionCount() const {
        return sections.size();
    }

    Section &Document::GetSection(const std::string &name) {
        return sections.at(name);
    }


    std::pair<std::string_view, std::string_view> Split(std::string_view line, char by) {
        size_t pos = line.find(by);
        std::string_view left = line.substr(0, pos);

        if (pos < line.size() && pos + 1 < line.size()) {
            return {left, line.substr(pos + 1)};
        } else {
            return {left, std::string_view()};
        }
    }

    std::string_view Unquote(std::string_view value, char left_quote, char right_quote) {
        if (!value.empty() && value.front() == left_quote) {
            value.remove_prefix(1);
        }
        if (!value.empty() && value.back() == right_quote) {
            value.remove_suffix(1);
        }
        return value;
    }

    Document Load(std::istream &input) {
        Document result;
        std::string current_section;
        for (std::string line; getline(input, line);) {
            if (line.empty())
                continue;
            if (line.front() == '[') {
                current_section = Unquote(line, '[',']');
                result.AddSection(current_section);
            } else {
                result.GetSection(current_section).insert( Split(line, '='));
            }
        }
        return result;
    }
}

