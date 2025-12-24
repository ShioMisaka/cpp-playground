#pragma once

#include <string>
#include <vector>

std::vector<int> build_lps(const std::string& patt)
{
    std::vector<int> lps(patt.size(), 0);
    for (size_t i = 1, len = 0; i < patt.size();) {
        if (patt[i] == patt[len]) {
            lps[i++] = ++len;
        } else if (len != 0) {
            len = lps[len - 1];
        } else {
            lps[i++] = 0;
        }
    }
    return lps;
}

size_t kmp_search(const std::string& str, const std::string& patt)
{
    auto lps = build_lps(patt);

    size_t i = 0;
    size_t j = 0;
    while (i < str.size()) {
        if (str[i] == patt[j]) { 
            ++i; ++j;
            if (j == patt.size()) return i - j;
        }
        else if (j > 0) { j = lps[j - 1]; }
        else { ++i; }
    }
    return static_cast<size_t>(-1);
}
