#include <cstdio>
#include <exception>
#include "FPValue.hpp"
#include "FPMath.hpp"
#include "IntStringHelper.hpp"


int main()
{
    try {
        FPValue value1 = "3";
        FPValue value2("1.5");

        printf("value1=%s\n", value1.c_str());
        printf("value2=%s\n", value2.c_str());

        // 2数の演算
        /*printf("value1+value2=[%s]\n", (value1 + value2).c_str());
        printf("value1-value2=[%s]\n", (value1 - value2).c_str());
        printf("value1*value2=[%s]\n", (value1 * value2).c_str());
        printf("value1/value2=[%s]\n", (value1 / value2).c_str());
        printf("value1/value2=[%s]\n", FPValue::Div(value1, value2, 20, true).c_str());
        printf("value1%%value2=[%s]\n", (value1 % value2).c_str());*/
        printf("value1^value2=%s\n", (value1 ^ value2).c_str());

        // 自然対数の底
        printf("e=%s\n", FPMath::LogBaseE(20).c_str());

        // サイン・コサインの計算
        FPValue angle("3.1415926");
        printf("sin=%s\n", FPMath::Sin(angle, 20).c_str());
        printf("cos=%s\n", FPMath::Cos(angle, 20).c_str());

    } catch (std::exception& e) {
        printf("Error: %s\n", e.what());
    }

    return 0;
}

