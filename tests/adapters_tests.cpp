#include <views.cpp>
#include <gtest/gtest.h>
#include <vector>
#include <set>
#include <map>
#include <string>

TEST(viewsTestSuite, TransformTest1) {
    std::vector<int> a = {1, 2 , 3}, b;
    for (auto c : a | transform([](int x){return x * x;}))
        b.push_back(c);
    
    std::vector<int> c = {1, 4, 9};
    ASSERT_EQ(c, b);
}

TEST(viewsTestSuite, FilterTest1) {
    std::vector<int> a = {1, 2, 3, 4, 5}, b;
    for (auto c : a | filter([](int x){return x > 2;}))
        b.push_back(c);
    
    std::vector<int> c = {3, 4, 5};
    ASSERT_EQ(c, b);
}


TEST(viewsTestSuite, FilterTest2) {
    std::vector<int> a = {1, 2, 3, 4, 5}, b;
    for (auto c : a | filter([](int x){return x > 2;}) | transform([](int x){return x * x;}))
        b.push_back(c);
    
    std::vector<int> c = {9, 16, 25};
    ASSERT_EQ(c, b);
}

TEST(viewsTestSuite, TakeDropTest1) {
    std::vector<int> a = {1, 2, 3, 4, 5, 6, 7}, b;
    for (auto c : a | drop(2) | take(3))
        b.push_back(c);
    
    std::vector<int> c = {3, 4, 5};
    ASSERT_EQ(c, b);
}

TEST(viewsTestSuite, TakeDropTest2) {
    std::vector<int> a = {1, 2, 3, 4, 5, 6, 7};
    std::vector<std::string>  b;
    for (auto c : a | take(3) | transform([](int x){return (std::string("qwe")).substr(x - 1);}))
        b.push_back(c);
    
    std::vector<std::string> c = {"qwe", "we", "e"};
    ASSERT_EQ(c, b);
}

TEST(viewsTestSuite, ReverseTest1) {
    std::vector<int> a = {1, 2, 3}, b;
    for (auto c : a | reverse())
        b.push_back(c);
    
    std::vector<int> c = {3, 2, 1};
    ASSERT_EQ(c, b);
}

TEST(viewsTestSuite, KeysValuesTest1) {
    std::map<std::string, int> a;
    a["qwe"] = 123;
    a["asd"] = 456;
    std::vector<std::string> k;
    std::vector<int> v;
    for (auto c : a | keys())
        k.push_back(c);
    for (auto c : a | values())
        v.push_back(c);
    
    std::vector<std::string> q = {"asd", "qwe"};
    std::vector<int> w = {456, 123};
    ASSERT_EQ(k, q);
    ASSERT_EQ(v, w);
}