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
        FPValue num = Pow(angle, base, 20);
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
        FPValue num = Pow(angle, base, 20);
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

// baseのexponent乗
FPValue FPMath::Pow(const FPValue& base, const FPValue& exponent, int macCount)
{
    //printf("Pow (base=%s, exp=%s)\n", base.c_str(), exponent.c_str());

    // ゼロ乗は1と定義する
    if (exponent.IsZero()) {
        return FPValue("1");
    }

    FPValue absexp = (exponent.sign > 0)? exponent: -exponent;

    // 整数乗の場合は普通に掛け算を計算する
    if (exponent.dp == 0) {
        FPValue pow("1");
        FPValue exp(absexp);
        while (!exp.IsZero()) {
            pow = pow * base;
            exp = exp - FPValue("1");
        }
        return (exponent.sign > 0)? pow: (FPValue("1") / pow);
    }

    // 小数乗の場合はマクローリン展開を計算する
    int dimCount = 0;
    FPValue pow("1");
    FPValue x = base - FPValue("1");
    FPValue alpha(absexp);
    FPValue dalpha("1");
    //printf("dimCount=%d, pow=%s\n", dimCount, pow.c_str());

    while (dimCount < macCount - 1) {
        FPValue fact("1");
        FPValue ffact("2");
        for (int i = 0; i < dimCount; i++) {
            fact = fact * ffact;
            ffact = ffact + FPValue("1");
        }
        FPValue p(x);
        for (int i = 0; i < dimCount; i++) {
            p = p * x;
        }
        //printf("  fact=%s, p=%s, alhpha=%s\n", fact.c_str(), p.c_str(), alpha.c_str());
        pow = pow + (alpha * p / fact);
        dimCount++;
        //printf("dimCount=%d, pow=%s\n", dimCount, pow.c_str());
        alpha = alpha * (absexp - dalpha);
        dalpha = dalpha + FPValue("1");
    }

    return (exponent.sign > 0)? pow: (FPValue("1") / pow);
}

