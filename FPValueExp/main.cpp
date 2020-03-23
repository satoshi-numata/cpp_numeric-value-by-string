#include <cstdio>
#include <exception>
#include "FPValue.hpp"
#include "IntStringHelper.hpp"


int main()
{
    try {
        FPValue value1 = "3";
        FPValue value2("1.5");

        printf("value1=[%s]\n", value1.c_str());
        printf("value2=[%s]\n", value2.c_str());

        // 2数の演算
        /*printf("value1+value2=[%s]\n", (value1 + value2).c_str());
        printf("value1-value2=[%s]\n", (value1 - value2).c_str());
        printf("value1*value2=[%s]\n", (value1 * value2).c_str());
        printf("value1/value2=[%s]\n", (value1 / value2).c_str());
        printf("value1/value2=[%s]\n", FPValue::Div(value1, value2, 20, true).c_str());
        printf("value1%%value2=[%s]\n", (value1 % value2).c_str());*/

        printf("value1^value2=%s\n", FPValue::Pow(value1, value2, 100).c_str());

    } catch (std::exception& e) {
        printf("Error: %s\n", e.what());
    }

    return 0;
}

