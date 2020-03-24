#include "FPMath.hpp"
#include "IntStringHelper.hpp"
#include <cassert>
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
        ret += FPValue::Div(FPValue("1"), fact, dp, true);
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

// 円周率
FPValue FPMath::Pi(int dp)
{
    assert(dp >= 0 && dp <= 1000);

    static const std::string piStr = "31415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989";

    return FPValue(1, piStr.substr(0, dp+1), dp);
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
FPValue FPMath::Pow(const FPValue& base, const FPValue& exponent, int dp)
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

    while (dimCount < dp - 1) {
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

