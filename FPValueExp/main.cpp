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
        {
            FPValue e("1");
            FPValue fact("1");
            FPValue dfact("2");
            for (int i = 0; i < 10; i++) {
                e = e + FPValue::Div(FPValue("1"), fact, 40, true);
                //printf("%d: e=%s\n", i, e.c_str());
                fact = fact * dfact;
                dfact = dfact + FPValue("1");
            }
            printf("e=%s\n", e.c_str());
        }

        // サインを求めてみる
        {
            FPValue angle("0.5");
            FPValue s("0");
            FPValue fact("1");
            FPValue dfact("2");
            FPValue exp("1");
            int sign = 1;
            for (int i = 0; i < 10; i++) {
                FPValue num = FPValue::Pow(angle, exp, 20);
                if (sign > 0) {
                    s = s + num / fact;
                } else {
                    s = s - num / fact;
                }
                //printf("%d: num=%s, fact=%s, s=%s\n", i, num.c_str(), fact.c_str(), s.c_str());
                sign *= -1;
                fact = fact * dfact;
                dfact = dfact + FPValue("1");
                fact = fact * dfact;
                dfact = dfact + FPValue("1");
                exp = exp + FPValue("2");
            }
            printf("s=%s\n", s.c_str());
        }

        // コサインを求めてみる
        {
            FPValue angle("3.14");
            FPValue c("0");
            FPValue fact("1");
            FPValue dfact("2");
            FPValue exp("0");
            int sign = 1;
            for (int i = 0; i < 20; i++) {
                FPValue num = FPValue::Pow(angle, exp, 20);
                if (sign > 0) {
                    c = c + num / fact;
                } else {
                    c = c - num / fact;
                }
                //printf("%d: num=%s, fact=%s, s=%s\n", i, num.c_str(), fact.c_str(), s.c_str());
                sign *= -1;
                fact = fact * dfact;
                dfact = dfact + FPValue("1");
                if (i > 0) {
                    fact = fact * dfact;
                    dfact = dfact + FPValue("1");
                }
                exp = exp + FPValue("2");
            }
            printf("c=%s\n", c.c_str());
        }

    } catch (std::exception& e) {
        printf("Error: %s\n", e.what());
    }

    return 0;
}

