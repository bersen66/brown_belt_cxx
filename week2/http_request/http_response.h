//
// Created by ortur on 30.01.2022.
//

#ifndef BROWN_BELT_HTTP_RESPONSE_H
#define BROWN_BELT_HTTP_RESPONSE_H

#include <string>
#include <unordered_map>

enum class HttpCode {
    Ok = 200,
    NotFound = 404,
    Found = 302,
};

class HttpResponse {
public:
    explicit HttpResponse(HttpCode code);

    HttpResponse& AddHeader(std::string name, std::string value);
    HttpResponse& SetContent(std::string a_content);
    HttpResponse& SetCode(HttpCode a_code = HttpCode::Ok);

    friend std::ostream& operator << (std::ostream& output, const HttpResponse& resp);
private:
    HttpResponse& SetComment(HttpCode code = HttpCode::Ok);
    std::unordered_map<std::string, std::string> headers_;
    std::string content_;
    std::string comment;
    HttpCode answer_code_;
};

std::ostream& operator << (std::ostream& output, const HttpResponse& resp);
#endif //BROWN_BELT_HTTP_RESPONSE_H
