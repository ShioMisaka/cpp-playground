#pragma once

#include "../vector.h"

namespace mystl
{
template <typename Iter>
Iter kmp_search(Iter s_start, Iter s_last, Iter patt_b, Iter patt_e) {
    size_t str_size = s_last - s_start;
    size_t patt_size = patt_e - patt_b;

    int *lps = new int[patt_size];

    for (size_t i = 1, len = 0; i < patt_size;) {
        if (*(patt_b + i) == *(patt_b + len)) {
            lps[i++] = ++len;
        } else if (len != 0) {
            len = lps[len - 1];
        } else {
            lps[i++] = 0;
        }
    }

    size_t i = 0;
    size_t j = 0;
    while (i < str_size) {
        if (*(s_start + i) == *(patt_b + j)) { 
            ++i; ++j;
            if (j == patt_size) return s_start + i - j;
        }
        else if (j > 0) { j = lps[j - 1]; }
        else { ++i; }
    }

    delete[] lps;
    return s_last;
}
}