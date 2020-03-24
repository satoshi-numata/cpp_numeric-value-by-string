#include "FPMath.hpp"
#include "IntStringHelper.hpp"
#include <climits>


// 自然対数の底
FPValue FPMath::LogBaseE(int dp)
{
    std::string last_vstr("");
    int sameCount = 0;
    FPValue ret("0");
    std::string base = "0";
    for (int i = 0; i < INT_MAX; i++) {
        std::string fact = IntString_Fact(base);
        ret = ret + FPValue::Div(FPValue("1"), fact, dp, true);
        if (i >= 10 && ret.dp == dp) {
            if (last_vstr == ret.vstr) {
                sameCount++;
                if (sameCount > 3) {
                    break;
                }
            }
            last_vstr = ret.vstr;
        }
        base = IntString_Add(base, "1");
    }
    return ret;
}

// サインを計算する
FPValue FPMath::Sin(const FPValue& angle, int dp)
{
    std::string last_vstr("");
    int sameCount = 0;
    int last_dp = 0;
    int dpSameCount = 0;
    FPValue ret("0");
    std::string base("1");
    int sign = 1;
    for (int i = 0; i < INT_MAX; i++) {
        std::string fact = IntString_Fact(base);
        FPValue num = FPValue::Pow(angle, base, 20);
        FPValue p = FPValue::Div(num, fact, dp, true);
        ret = (sign > 0)? (ret + p): (ret - p);
        if (i >= 10 && ret.dp == dp) {
            if (last_vstr == ret.vstr) {
                sameCount++;
                if (sameCount > 3) {
                    break;
                }
            }
            last_vstr = ret.vstr;
        }
        if (i >= 10) {
            if (last_dp == ret.dp) {
                dpSameCount++;
                if (dpSameCount > 10) {
                    break;
                }
            }
            last_dp = ret.dp;
        }
        sign *= -1;
        base = IntString_Add(base, "2");
    }
    return ret;
}

// コサインを計算する
FPValue FPMath::Cos(const FPValue& angle, int dp)
{
    std::string last_vstr("");
    int sameCount = 0;
    int last_dp = 0;
    int dpSameCount = 0;
    FPValue ret("0");
    std::string base("0");
    int sign = 1;
    for (int i = 0; i < INT_MAX; i++) {
        std::string fact = IntString_Fact(base);
        FPValue num = FPValue::Pow(angle, base, 20);
        FPValue p = FPValue::Div(num, fact, dp, true);
        ret = (sign > 0)? (ret + p): (ret - p);
        if (i >= 10 && ret.dp == dp) {
            if (last_vstr == ret.vstr) {
                sameCount++;
                if (sameCount > 3) {
                    break;
                }
            }
            last_vstr = ret.vstr;
        }
        if (i >= 10) {
            if (last_dp == ret.dp) {
                dpSameCount++;
                if (dpSameCount > 10) {
                    break;
                }
            }
            last_dp = ret.dp;
        }
        sign *= -1;
        base = IntString_Add(base, "2");
    }
    return ret;
}
/*
{
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
*/
