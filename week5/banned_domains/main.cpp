#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <deque>

using namespace std;

using DomainList = deque<string>;

DomainList ReadDomains() {
    size_t len;
    cin >> len;

    deque<string> result;

    for (size_t i = 0; i < len; i++) {
        string domain;
        cin >> domain;
        result.push_back(move(domain));
    }

    return result;
}

bool IsSubdomainOf(string_view subdomain, string_view domain) {
    int i = 0;
    int j = 0;
    while (i < subdomain.size() && j < domain.size()) {
        if (subdomain[i++] != domain[j++]) {
            return false;
        }
    }
    if (subdomain.size() != domain.size()) {
        return (j == domain.size() && subdomain[i] == '.') ||
               (i == subdomain.size() && domain[j] == '.');
    }
    return true;
}

bool BelongsToBannedList(const string& domain, const DomainList& banned_domains) {
    auto it =
            upper_bound(banned_domains.begin(), banned_domains.end(), domain);
    return it != banned_domains.begin()
           && IsSubdomainOf(domain, *prev(it));
}

void ReverseAllStrings(DomainList& domains) {
    for (auto& domain : domains) {
        reverse(domain.begin(), domain.end());
    }
}



int main() {
    // init by IILE
    const DomainList banned_domains =[]{
        DomainList result = ReadDomains();
        ReverseAllStrings(result);
        sort(result.begin(), result.end());

        size_t insert_pos = 0;
        for (string& domain : result) {
            if (insert_pos == 0 || !IsSubdomainOf(domain, result[insert_pos - 1])) {
                swap(result[insert_pos++], domain);
            }
        }
        result.resize(insert_pos);

        return result;
    }();


    auto domains_to_check = []{
        DomainList domains = ReadDomains();
        ReverseAllStrings(domains);
        return domains;
    }();

    for (auto& domain : domains_to_check) {
        if (BelongsToBannedList(domain, banned_domains)) {
            cout << "Bad\n";
        } else {
            cout << "Good\n";
        }
    }

    return 0;
}