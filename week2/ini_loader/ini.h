//
// Created by ortur on 27.01.2022.
//

#ifndef BROWN_BELT_INI_H
#define BROWN_BELT_INI_H

#include <unordered_map>
#include <string>
#include <sstream>


namespace Ini {
    using Section = std::unordered_map<std::string, std::string>;

    class Document {
    public:
        Section &AddSection(const std::string& name);

        const Section &GetSection(const std::string &name) const;
        Section& GetSection(const std::string& name);


        size_t SectionCount() const;

    private:
        std::unordered_map <std::string, Section> sections;
    };

    std::string_view ParseHeader(std::istream& input);

    Document Load(std::istream &input);
}
#endif //BROWN_BELT_INI_H
