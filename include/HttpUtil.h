#ifndef HTTPUTILITY_H
#define HTTPUTILITY_H

#include <sstream>
#include <bits/stdc++.h>
#include <string>
#include <cctype>

namespace web {
    inline char fromHex(char ch) {
        return std::isdigit(ch) ? ch - '0' : std::tolower(ch) - 'a' + 10;
    }

    std::string urlDecode(const std::string& text) {
        std::ostringstream escaped;
        escaped.fill('0');
        for (char c : text) {
            if (c == '%') {
                if (text.size() > 2) {
                    char h = fromHex(text[1]) << 4 | fromHex(text[2]);
                    escaped << h;
                    text.erase(0, 3); 
                }
            } else if (c == '+') {
                escaped << ' ';
            } else {
                escaped << c;
            }
        }
        return escaped.str();
    }
}

#endif
Change