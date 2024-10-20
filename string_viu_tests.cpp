#include <stdio.h>

#define STRING_VIEW_IMPLEMENTATION
#define STRING_VIEW_CPP_HELPERS
#include "string_viu.h"

int main(int argc, char *argv[]) {
    fprintf(stdout, "String viu tests\n");

    StringViu view_1 = s_viu("str_viu");

    bool test_1 = view_1 == "str_viu" &&
        view_1 == s_viu("str_viu") &&
        view_1 != "str_viuBLAH" &&
        view_1 != "str_v" &&
        view_1 == std::string("str_viu") &&
        view_1 != std::string("str");

    if(test_1) {
        fprintf(stdout, "TEST 1: passed.\n");
    } else {
        fprintf(stderr, "TEST 1: failed.\n");
    }

    StringViu view_2 = s_viu("1234567776543210");
    StringViu view_2_1 = s_viu_advance(view_2, 7);
    StringViu view_2_2 = s_viu_truncate(view_2_1, 7);

    bool test_2 = view_2_2 == "77" && 
       s_viu_truncate(view_2_2, 1) == "7" &&
       s_viu_advance(view_2_1, 231).length == 0 &&
       s_viu_truncate(s_viu("12333"), 21346124).length == 0 &&
       s_viu_truncate(s_viu(""), 2).length == 0 &&
       s_viu_truncate(s_viu_advance(s_viu("X Y Z"), 2), 2) == "Y";

    if(test_2) {
        fprintf(stdout, "TEST 2: passed.\n");
    } else {
        fprintf(stderr, "TEST 2: failed.\n");
    }

    StringViu view_3_1 = s_viu("Hello, World!");
    StringViu view_3_2 = s_viu("aa");

    bool test_3 = s_viu_substr(view_3_1, 0, 4) == "Hello" &&
        s_viu_substr(view_3_1, 7, 12) == "World!" &&
        s_viu_substr(view_3_1, 2, 7) != "Hello, world!" &&
        s_viu_substr(view_3_1, 5, 6) == ", " &&
        s_viu_substr(view_3_2, 0, 1) == "aa" &&
        s_viu_substr(view_3_2, 1, 1) != "aa";

    if(test_3) {
        fprintf(stdout, "TEST 3: passed.\n");
    } else {
        fprintf(stderr, "TEST 3: failed.\n");
    }

    StringViu view_4 = s_viu("Abecxede");
    int32_t view_4_x_idx = s_viu_find_first(view_4, 'x');
    int32_t view_4_e1_idx = s_viu_find_first(view_4, 'e');
    int32_t view_4_e2_idx = s_viu_find_first(view_4, 'e', view_4_e1_idx + 1);
    int32_t view_4_113_idx = s_viu_find_last(view_4, 'e');
    int32_t view_4_114_idx = s_viu_find_last(view_4, 'e', 1); 
    
    bool test_4 = view_4_x_idx == 4 &&
        view_4.data[view_4_x_idx] == 'x' &&
        view_4_e1_idx == 2 &&
        view_4.data[view_4_e1_idx] == 'e' &&
        view_4_e2_idx == 5 &&
        view_4.data[view_4_e2_idx] == 'e' &&
        s_viu_find_first(s_viu("Blah Tes66t.: 22\n"), ":6", 2) == 8 &&
        s_viu_find_first(s_viu_zero(), "414", 3) == -1 &&
        s_viu_find_first(s_viu("12345"), "67890", 5, 3) == -1 &&
        view_4_113_idx == view_4.length - 1 &&
        view_4.data[view_4_113_idx] == 'e' &&
        view_4_114_idx == 5 &&
        view_4.data[view_4_114_idx] == 'e';

    if(test_4) {
        fprintf(stdout, "TEST 4: passed.\n");
    } else {
        fprintf(stderr, "TEST 4: failed.\n");
    }

    StringViu view_5 = s_viu("Test 5 this is lol");
    int32_t view_5_1_idx = s_viu_find(view_5, "5 this");
    int32_t view_5_2_idx = s_viu_find(view_5, "lole");
    int32_t view_5_3_idx = s_viu_find(view_5, "est 5 thi");
    int32_t view_5_4_idx = s_viu_find(view_5, "is", strlen("is"), s_viu_find(view_5, "is") + 2);
    int32_t view_5_5_idx = s_viu_find(view_5, "is", strlen("is"));

    bool test_5 = view_5_1_idx == 5 &&
        view_5_2_idx == -1 &&
        view_5_3_idx == 1 &&
        view_5_4_idx == 12 &&
        view_5_5_idx == 9;

    if(test_5) {
        fprintf(stdout, "TEST 5: passed.\n");
    } else {
        fprintf(stderr, "TEST 5: failed.\n");
    }

    StringViu view_6 = s_viu("Beginning of the string");
    StringViu view_6_1_rem = s_viu("");
    StringViu view_6_2_rem = s_viu("");

    bool test_6 = s_viu_begins_with(view_6, &view_6_1_rem, "Beginning") &&
        view_6_1_rem == " of the string" &&
        s_viu_begins_with(view_6, NULL, "Begi") &&
        !s_viu_begins_with(view_6, NULL, "inni") &&
        s_viu_begins_with(view_5, NULL, "Test ") &&
        !s_viu_begins_with(view_5, NULL, "est") &&
        s_viu_begins_with(view_4, NULL, "Abec", 2) &&
        !s_viu_begins_with(view_4, NULL, "abec", 4) &&
        s_viu_begins_with(view_3_1, NULL, "H") &&
        s_viu_ends_with(view_6, &view_6_2_rem, " string") &&
        !s_viu_ends_with(view_6, &view_6_2_rem, " hobb") &&
        view_6_2_rem == "Beginning of the";

    if(test_6) {
        fprintf(stdout, "TEST 6: passed.\n");
    } else {
        fprintf(stderr, "TEST 6: failed.\n");
    }

    StringViu view_7 = s_viu("@start !var1, !var2 end@");
    StringViu view_7_a = s_viu_advance(view_7, (int32_t)strlen("@start "));
    StringViu view_7_t = s_viu_truncate(view_7, (int32_t)strlen(" end@"));

    SV_ASSERT(s_viu_begins_with(view_7, &view_7, "@start "));
    SV_ASSERT(s_viu_ends_with(view_7,   &view_7, " end@"));

    StringViu view_7_var_1 = s_viu_zero();
    StringViu view_7_var_2 = s_viu_zero();

    int32_t view_7_comma_idx = s_viu_find_first(view_7, ',');
    if(view_7_comma_idx != -1) {
        SV_ASSERT(view_7_comma_idx != 0 && view_7_comma_idx != view_7.length - 1, "Comma at the edge of view");
        view_7_var_1 = s_viu_substr(view_7, 0, view_7_comma_idx - 1);
        view_7_var_2 = s_viu_substr(view_7, view_7_comma_idx + 1, view_7.length - 1);
    }

    bool test_7 = view_7_a == "!var1, !var2 end@" &&
        view_7_t == "@start !var1, !var2" &&
        !s_viu_is_empty(view_7_var_1) &&
        !s_viu_is_empty(view_7_var_2);

    if(test_7) {
        fprintf(stdout, "TEST 7: passed.\n");
    } else {
        fprintf(stderr, "TEST 7: failed.\n");
    }

    StringViu view_8 = s_viu("        5534311                fff               ");
    StringViu view_8_2 = s_viu_trim(view_8);
    StringViu view_8_3 = s_viu(" 55  34  311         fff   \t\t    \n");
    StringViu view_8_4 = s_viu_trim(view_8_3);

    bool test_8 = view_8_2 == "5534311                fff" &&
        view_8_4 == "55  34  311         fff   \t\t    \n" &&
        s_viu_trim(s_viu("")) == "" &&
        s_viu_trim(s_viu("xxxxxxxxx")) == s_viu("xxxxxxxxx");

    if(test_8) {
        fprintf(stdout, "TEST 8: passed.\n");
    } else {
        fprintf(stderr, "TEST 8: failed.\n");
    }

    return 0;
}
