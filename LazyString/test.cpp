#include "gtest/gtest.h"
#include "lazy_string.h"

TEST(correctness, ConstructorDestructor) {
    lazy_string l;
    lazy_string l1(std::string("abc"));
    lazy_string l2(l1);
}

namespace {
    bool isEqual(const lazy_string l, std::string s) {
        bool result = true;
        for (int i = 0; i < s.length(); i++) {
            if (l[i] != s[i])
                result = false;
        }
        return result;
    }
}

TEST(correctness, ReadChars) {
    std::string str = "abcdef";
    lazy_string l(str);
    EXPECT_TRUE(isEqual(l, str));
}

TEST(correctness, Addition) {
    std::string s1 = "abc", s2 = "def", s3 = s1 + s2;
    lazy_string l1(s1), l2(s2);
    const lazy_string l3 = l1 + l2;
    EXPECT_TRUE(isEqual(l3, s3));
}

TEST(correctness, AdditionAndCopy) {
    std::string s1 = "abc", s2 = "def", s3 = s1 + s2;
    lazy_string l1(s1), l2(s2);
    lazy_string l3 = l1;
    l3 += l2;
    EXPECT_TRUE(isEqual(l3, s3));
    EXPECT_FALSE(isEqual(l3, l2.str()));
}

TEST(correctness, ChangeChar) {
    std::string s1 = "abc", s2 = "abe";
    lazy_string l1(s1), l2(s1);
    l1[2];
    EXPECT_TRUE(isEqual(l1, l2.str()));
    l2[2] = s2[2];
    EXPECT_FALSE(isEqual(l1, l2.str()));
}
