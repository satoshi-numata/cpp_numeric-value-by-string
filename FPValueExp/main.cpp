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

        printf("value1^value2=%s\n", FPValue::Pow(value1, value2, 9).c_str());

        // 自然対数の底を求めてみる
        FPValue e("1");
        FPValue fact("1");
        FPValue dfact("2");
        for (int i = 0; i < 40; i++) {
            e = e + FPValue::Div(FPValue("1"), fact, 40, true);
            printf("%d: e=%s\n", i, e.c_str());
            fact = fact * dfact;
            dfact = dfact + FPValue("1");
        }
        printf("e=%s\n", e.c_str());

    } catch (std::exception& e) {
        printf("Error: %s\n", e.what());
    }

    return 0;
}

