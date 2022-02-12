//
// Created by ortur on 30.01.2022.
//

#include <sstream>

#include "http_response.h"

using namespace std;

HttpResponse::HttpResponse(HttpCode code) : answer_code_(code) {
    SetComment();
}

HttpResponse &HttpResponse::AddHeader(std::string name, std::string value) {
    headers_.insert({name, value});
}


HttpResponse &HttpResponse::SetContent(std::string a_content) {
    content_ = move(a_content);
}

HttpResponse &HttpResponse::SetCode(HttpCode a_code) {
    answer_code_ = a_code;
    SetCode(answer_code_);
}

ostream& operator<<(ostream& output, const HttpResponse &resp) {
    stringstream result;

    result << "HTTP/1.1 " << static_cast<int>(resp.answer_code_) << " " << resp.comment << "\n";
    for (const auto& [header, value] : resp.headers_) {
        result << header << ": " << value << "\n";
    }
    result << "\n";
    result << resp.content_ << "\n";
    result << resp.content_.size();

    output << result.str();
    return output;
}

HttpResponse& HttpResponse::SetComment(HttpCode code) {
    switch (code) {
        case HttpCode::Ok : {comment = "Ok";} break;
        case HttpCode::NotFound : {comment = "Not found";} break;
        case HttpCode::Found : {comment = "Not found";} break;
    }
    return *this;
}
