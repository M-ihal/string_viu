#ifndef _STRING_VIU_H
#define _STRING_VIU_H

/*
 *  Definitions:
 *      - STRING_VIU_IMPLEMENTATION
 *      - STRING_VIU_CPP_HELPERS
 */

#include <stdint.h>
#include <assert.h>

#define SV_COMBINE__(l, r) l##r
#define SV_COMBINE_(l, r) SV_COMBINE__(l, r)
#define SV_COMBINE(l, r) SV_COMBINE_(l, r)

#define SV_ASSERT(exp, ...) {\
    const bool SV_COMBINE(_assert, __LINE__) = exp;\
    if(!SV_COMBINE(_assert, __LINE__)) {\
        fprintf(stderr, "Assertion_failed: "#__VA_ARGS__);\
    }\
    assert(SV_COMBINE(_assert, __LINE__));\
}

#define SV_ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))

#if 0
/* @temp so editor doesn't gray out... */
#define STRING_VIU_IMPLEMENTATION
#define STRING_VIU_CPP_HELPERS
#endif

/* StringViu doesn't own the data pointer.
 * The data pointer must stay valid when using the functions.
 * Arguments passed to functions need to be valid.
 * length is signed int, for simplicity, may change.
 */
struct StringViu {
    const char *data;
    int32_t     length;
};

inline StringViu s_viu_zero(void);
inline bool      s_viu_is_empty(const StringViu &view);
StringViu s_viu(const char *cstr);
StringViu s_viu(const char *mem, int32_t length);
bool      s_viu_compare(const StringViu &view_a, const StringViu &view_b);
bool      s_viu_compare(const StringViu &view, const char *cstr);

StringViu s_viu_advance(const StringViu &view, int32_t num);
StringViu s_viu_truncate(const StringViu &view, int32_t num);
StringViu s_viu_substr(const StringViu &view, int32_t idx_l, int32_t idx_r);
StringViu s_viu_trim(const StringViu &view);

void s_viu_split(const StringViu &view, int32_t idx, StringViu &view_l, StringViu &view_r);

int32_t s_viu_find_first(const StringViu &view, const char *chars, int32_t count, int32_t offset = 0);
int32_t s_viu_find_first(const StringViu &view, char _char, int32_t offset = 0);
int32_t s_viu_find_first_not(const StringViu &view, const char *chars, int32_t count, int32_t offset = 0);
int32_t s_viu_find_first_not(const StringViu &view, char _char, int32_t offset = 0);
int32_t s_viu_find_last(const StringViu &view, const char *chars, int32_t count, int32_t offset = 0);
int32_t s_viu_find_last(const StringViu &view, char _char, int32_t offset = 0);
int32_t s_viu_find_last_not(const StringViu &view, const char *chars, int32_t count, int32_t offset = 0);
int32_t s_viu_find_last_not(const StringViu &view, char _char, int32_t offset = 0);
int32_t s_viu_find(const StringViu &view, const char *cstr, int32_t cstr_len = 0, int32_t offset = 0);

bool s_viu_begins_with(const StringViu &view, StringViu *opt_rem, const char *cstr, int32_t cstr_len = 0);
bool s_viu_ends_with(const StringViu &view, StringViu *opt_rem, const char *cstr, int32_t cstr_len = 0);

#ifdef STRING_VIU_CPP_HELPERS
#include <string>
bool s_viu_compare(const StringViu &view, const std::string &string);
bool operator == (const StringViu &view_a, const StringViu &view_b);
bool operator != (const StringViu &view_a, const StringViu &view_b);
bool operator == (const StringViu &view, const char *cstr);
bool operator != (const StringViu &view, const char *cstr);
bool operator == (const StringViu &view, const std::string &string);
bool operator != (const StringViu &view, const std::string &string);
std::string s_viu_to_std_string(const StringViu &view);
#endif /* STRING_VIU_CPP_HELPERS */

#ifdef STRING_VIU_IMPLEMENTATION

inline StringViu s_viu_zero(void) {
    return StringViu{
        .data = NULL,
        .length = 0
    };
}

inline bool s_viu_is_empty(const StringViu &view) {
    return view.length <= 0;
}

StringViu s_viu(const char *cstr) {
    return StringViu{
        .data   = cstr,
        .length = (int32_t)strlen(cstr)
    };
}

StringViu s_viu(const char *mem, int32_t length) {
    return StringViu{
        .data   = mem,
        .length = length
    };
}

bool s_viu_compare(const StringViu &view_a, const StringViu &view_b) {
    if(view_a.length != view_b.length) {
        return false;
    }

    for(int32_t index = 0; index < view_a.length; ++index) {
        if(view_a.data[index] != view_b.data[index]) {
            return false;
        }
    }

    return true;
}

bool s_viu_compare(const StringViu &view, const char *cstr) {
    return s_viu_compare(view, s_viu(cstr));
}

StringViu s_viu_advance(const StringViu &view, int32_t num) {
    SV_ASSERT(num >= 0);

    StringViu res = view;
    while(res.length && num) {
        ++res.data;
        --res.length;
        --num;
    }
    return res;
}

StringViu s_viu_truncate(const StringViu &view, int32_t num) {
    SV_ASSERT(num >= 0);

    StringViu res = view;
    while(res.length && num) {
        --res.length;
        --num;
    }
    return res;
}

StringViu s_viu_substr(const StringViu &view, int32_t idx_l, int32_t idx_r) {
    SV_ASSERT(idx_l <= idx_r, "Left index must be lower or equal to right");
    SV_ASSERT(idx_l >= 0 && idx_l < view.length);
    SV_ASSERT(idx_r >= 0 && idx_r < view.length);

    StringViu res = s_viu_advance(view, idx_l);
    return s_viu_truncate(res, view.length - idx_r - 1);
}

StringViu s_viu_trim(const StringViu &view) {
    const char chars[] = " \t";

    int32_t idx_l = s_viu_find_first_not(view, chars, SV_ARRAY_COUNT(chars));
    if(idx_l == -1) {
        /* Whole string is whitespace */
        return s_viu_zero();
    }

    int32_t idx_r = s_viu_find_last_not(view, chars, SV_ARRAY_COUNT(chars));
    SV_ASSERT(idx_r != -1); // If idx_l has been find, idx_r must be too...

    return s_viu_substr(view, idx_l, idx_r);
}

void s_viu_split(const StringViu &view, int32_t idx, StringViu &view_l, StringViu &view_r) {
    if(!view.length) {
        /* Nothing to split */
        view_l = s_viu_zero();
        view_r = s_viu_zero();
    }

    SV_ASSERT(idx >= 0 && idx < view.length && (idx + 1));

    if(idx == 0) {
        view_l = s_viu_zero();
    } else {
        view_l = s_viu_substr(view, 0, idx - 1);
    }

    if(idx == view.length - 1) {
        view_r = s_viu_zero();
    } else {
        view_r = s_viu_substr(view, idx + 1, view.length - 1);
    }
}

inline bool _s_viu_is_char_any_of(const char _char, const char *chars, int32_t count) {
    for(int32_t index = 0; index < count; ++index) {
        if(_char == chars[index]) {
            return true;
        }
    }
    return false;
}

int32_t s_viu_find_first(const StringViu &view, const char *chars, int32_t count, int32_t offset) {
    SV_ASSERT(offset >= 0);
    SV_ASSERT(count  >= 0);

    if(!view.length || offset >= view.length) {
        return -1;
    }

    for(int32_t index = offset; index < view.length; ++index) {
        const char _char = view.data[index];
        if(_s_viu_is_char_any_of(_char, chars, count)) {
            return index;
        }
    }
    return -1;
}

int32_t s_viu_find_first(const StringViu &view, char _char, int32_t offset) {
    return s_viu_find_first(view, &_char, 1, offset);
}

int32_t s_viu_find_first_not(const StringViu &view, const char *chars, int32_t count, int32_t offset) {
    SV_ASSERT(offset >= 0);
    SV_ASSERT(count  >= 0);

    if(!view.length || offset >= view.length) {
        return -1;
    }

    for(int32_t index = offset; index < view.length; ++index) {
        const char _char = view.data[index];
        if(!_s_viu_is_char_any_of(_char, chars, count)) {
            return index;
        }
    }
    return -1;
}

int32_t s_viu_find_first_not(const StringViu &view, char _char, int32_t offset) {
    return s_viu_find_first_not(view, &_char, 1, offset);
}

int32_t s_viu_find_last(const StringViu &view, const char *chars, int32_t count, int32_t offset) {
    SV_ASSERT(offset >= 0);
    SV_ASSERT(count  >= 0);

    if(!view.length || offset >= view.length) {
        return -1;
    }

    for(int32_t index = view.length - offset - 1; index >= 0; --index) {
        const char _char = view.data[index];
        if(_s_viu_is_char_any_of(_char, chars, count)) {
            return index;
        }
    }
    return -1;
}

int32_t s_viu_find_last(const StringViu &view, char _char, int32_t offset) {
    return s_viu_find_last(view, &_char, 1, offset);
}

int32_t s_viu_find_last_not(const StringViu &view, const char *chars, int32_t count, int32_t offset) {
    SV_ASSERT(offset >= 0);
    SV_ASSERT(count  >= 0);

    if(!view.length || offset >= view.length) {
        return -1;
    }

    for(int32_t index = view.length - offset - 1; index >= 0; --index) {
        const char _char = view.data[index];
        if(!_s_viu_is_char_any_of(_char, chars, count)) {
            return index;
        }
    }
    return -1;
}

int32_t s_viu_find_last_not(const StringViu &view, char _char, int32_t offset) {
    return s_viu_find_last_not(view, &_char, 1, offset);
}

inline bool _s_viu_begins_with(const StringViu &view, const char *cstr, int32_t cstr_len) {
   /*
    *  No checks because callers check stuff
    */

    if(cstr_len > view.length) {
        return false;
    }

    for(int32_t index = 0; index < cstr_len; ++index) {
        if(view.data[index] != cstr[index]) {
            return false;
        }
    }

    return true;
}

int32_t s_viu_find(const StringViu &view, const char *cstr, int32_t cstr_len, int32_t offset) {
    SV_ASSERT(offset >= 0);
    SV_ASSERT(cstr_len >= 0);

    if(offset >= view.length) {
        return -1;
    }

    StringViu iter = s_viu_advance(view, offset);
    if(!iter.length) {
        return -1;
    }

    const int32_t length = cstr_len ? cstr_len : strlen(cstr);
    if(!length || length > iter.length) {
        return -1;
    }

    const int32_t range = iter.length - length;
    for(int32_t index = 0; index < range; ++index) {
        if(_s_viu_begins_with(iter, cstr, length)) {
            return index + offset;
        } else {
            iter = s_viu_advance(iter, 1);
        }
    }
    return -1;
}

bool s_viu_begins_with(const StringViu &view, StringViu *opt_rem, const char *cstr, int32_t cstr_len) {
    const int32_t length = cstr_len ? cstr_len : strlen(cstr);
    if(!length || length > view.length) {
        return false;
    } else {
        SV_ASSERT(length > 0);
    }

    bool result = _s_viu_begins_with(view, cstr, length);

    /* Optionally set view to be past the matched string */
    if(result && opt_rem) {
        *opt_rem = s_viu_advance(view, length);
    }

    return result;
}

bool s_viu_ends_with(const StringViu &view, StringViu *opt_rem, const char *cstr, int32_t cstr_len) {
    const int32_t length = cstr_len ? cstr_len : strlen(cstr);
    if(!length || view.length < length) {
        return false;
    } else {
        SV_ASSERT(length > 0);
    }

    bool result = _s_viu_begins_with(s_viu_advance(view, view.length - length), cstr, length);

    /* Optionally set view to be past the matched string */
    if(result && opt_rem) {
        *opt_rem = s_viu_truncate(view, length);
    }

    return result;
}

#ifdef STRING_VIU_CPP_HELPERS

bool s_viu_compare(const StringViu &view, const std::string &string) {
    return s_viu_compare(view, string.c_str());
}

bool operator == (const StringViu &view_a, const StringViu &view_b) {
    return s_viu_compare(view_a, view_b);
}

bool operator != (const StringViu &view_a, const StringViu &view_b) {
    return !(view_a == view_b);
}

bool operator == (const StringViu &view, const char *cstr) {
    return s_viu_compare(view, cstr);
}

bool operator != (const StringViu &view, const char *cstr) {
    return !(view == cstr);
}

bool operator == (const StringViu &view, const std::string &string) {
    return s_viu_compare(view, string);
}

bool operator != (const StringViu &view, const std::string &string) {
    return !(view == string);
}

std::string s_viu_to_std_string(const StringViu &view) {
    std::string string;
    string.assign(view.data, view.length);
    return string;
}

#endif /* STRING_VIU_CPP_HELPERS */
#endif /* STRING_VIU_IMPLEMENTATION */
#endif /* _STRING_VIU_H */
