#include "FPValue.hpp"
#include <cassert>
#include <cctype>
#include <cstdio>
#include <strstream>
#include <stdexcept>


// 2つの数値の足し算
FPValue FPValue::Add(const FPValue& value1, const FPValue& value2)
{
    //printf("Add\n");
    
    // TODO: 足し算を引き算として処理する場合への対処
    
    // 文字列と小数点の位置の取得
    std::string vstr1 = value1.str;
    std::string vstr2 = value2.str;
    int dp1 = value1.decimalPointIndex;
    int dp2 = value2.decimalPointIndex;
    //printf("  vstr1=[%s], dp1=%d, vstr2=[%s], dp2=%d\n", vstr1.c_str(), dp1, vstr2.c_str(), dp2);

    // 文字列の長さを合わせる
    while (dp1 < dp2) {
        vstr1 = vstr1 + "0";
        dp1++;
    }
    while (dp2 < dp1) {
        vstr2 = vstr2 + "0";
        dp2++;
    }
    while (vstr1.length() < vstr2.length()) {
        vstr1 = "0" + vstr1;
    }
    while (vstr2.length() < vstr1.length()) {
        vstr2 = "0" + vstr2;
    }
    //printf("  vstr1=[%s], dp1=%d, vstr2=[%s], dp2=%d\n", vstr1.c_str(), dp1, vstr2.c_str(), dp2);

    // 足し算を計算する
    std::string result = "";
    int len = (int)vstr1.length();
    int overflow = 0;
    for (int i = 0; i < len; i++) {
        int v1 = vstr1[len-i-1] - '0';
        int v2 = vstr2[len-i-1] - '0';
        int v3 = v1 + v2 + overflow;
        //printf("  %d + %d (+ %d) => %d\n", v1, v2, overflow, v3);
        overflow = v3 / 10;
        //printf("    overflow = %d\n", overflow);
        result = (char)((v3 % 10) + '0') + result;
        //printf("    result = [%s]\n", result.c_str());
    }
    if (overflow > 0) {
        result = (char)(overflow + '0') + result;
    }
    //printf("  result = [%s]\n", result.c_str());
    
    // 右端の0を取り除く
    while (dp1 > 0 && result[result.length()-1] == '0') {
        result = result.substr(0, result.length()-1);
        dp1--;
    }
    //printf("  result = [%s]\n", result.c_str());

    // 計算結果をリターンする
    return FPValue(value1.sign, result, dp1);
}

// 2つの数値の引き算
FPValue FPValue::Sub(const FPValue& minuend, const FPValue& subtrahend)
{
    throw std::runtime_error("Not implemented: FPValue::Sub()");
    return FPValue("-9999");
}

// 2つの数値の掛け算
FPValue FPValue::Mult(const FPValue& factor1, const FPValue& factor2)
{
    throw std::runtime_error("Not implemented: FPValue::Mult()");
    return FPValue("-9999");
}

// 2つの数値の割り算
std::pair<FPValue, FPValue> FPValue::Div(const FPValue& dividend, const FPValue& divisor, int decimalPlace)
{
    throw std::runtime_error("Not implemented: FPValue::Div()");
    return std::pair<FPValue, FPValue>(FPValue(), FPValue());
}

// baseのexponent乗
FPValue FPValue::Pow(const FPValue& base, const FPValue& exponent)
{
    throw std::runtime_error("Not implemented: FPValue::Pow()");
    return FPValue("-9999");
}


// デフォルトコンストラクタ
FPValue::FPValue()
    : sign(1), str("0"), decimalPointIndex(0)
{}

// コンストラクタ。"3.14159", "+3.14", "-2.6352"といった文字列を元に初期化する。
FPValue::FPValue(const std::string& normalValueExp)
{
    // 初期化
    sign = 1;
    decimalPointIndex = 0;

    // 文字列のパース
    bool hasDecimalPointAppeared = false;
    for (int i = 0; i < normalValueExp.length(); i++) {
        char c = normalValueExp[i];
        // 符号
        if (i == 0 && (c == '+' || c == '-')) {
            sign = (c == '+')? 1: -1;
        }
        // 小数点以下
        else if (hasDecimalPointAppeared) {
            if (c == '.') {
                std::strstream sstr;
                sstr << "Redundant dot appeared: index=" << i;
                throw std::runtime_error(sstr.str());
            } else if (isdigit(c)) {
                str += c;
                decimalPointIndex++;
            } else {
                std::strstream sstr;
                sstr << "Unknown character appeared (decimal part): " << c << " (index=" << i << ")";
                throw std::runtime_error(sstr.str());
            }
        }
        // 整数部
        else {
            if (c == '.') {
                hasDecimalPointAppeared = true;
            } else if (isdigit(c)) {
                str += c;
            } else {
                std::strstream sstr;
                sstr << "Unknown character appeared (integer part): " << c << " (index=" << i << ")";
                throw std::runtime_error(sstr.str());
            }
        }
    }

    // 小数点以下の不要な0の削除
    while (decimalPointIndex > 0) {
        if (str[str.length()-1] == '0') {
            str = str.substr(0, str.length()-1);
            decimalPointIndex--;
        } else {
            break;
        }
    }
}

// コンストラクタ。符号、数値文字列、数値文字列右端からの小数点の位置を元に初期化する。
FPValue::FPValue(int _sign, std::string _valueStr, int _decimalPointIndex)
{
    assert(_sign != 0);
    assert(_valueStr.length() > 0);
    for (int i = 0; i < _valueStr.length(); i++) {
        assert(isdigit(_valueStr[i]));
    }
    assert(_decimalPointIndex >= 0 && _decimalPointIndex <= _valueStr.length());

    sign = (_sign > 0)? 1: -1;
    str = _valueStr;
    decimalPointIndex = _decimalPointIndex;

    // 小数点以下の不要な0の削除
    while (decimalPointIndex > 0) {
        if (str[str.length()-1] == '0') {
            str = str.substr(0, str.length()-1);
            decimalPointIndex--;
        } else {
            break;
        }
    }
}

// コピー・コンストラクタ
FPValue::FPValue(const FPValue& value)
    : sign(value.sign), str(value.str), decimalPointIndex(value.decimalPointIndex)
{}


// 符号を反転させた数値を作成する。
FPValue FPValue::Negate() const
{
    return FPValue((sign > 0)? -1: 1, str, decimalPointIndex);
}


// FPValueを表す数値に変換する。
std::string FPValue::to_s() const
{
    std::string ret = str;
    bool hasDecimalPoint = (decimalPointIndex > 0);
    if (hasDecimalPoint) {
        hasDecimalPoint = false;
        for (int i = (int)ret.length() - decimalPointIndex; i < ret.length(); i++) {
            if (ret[i] != '0') {
                hasDecimalPoint = true;
                break;
            }
        }
    }
    if (hasDecimalPoint) {
        ret.insert(ret.length() - decimalPointIndex, ".");
    } else if (decimalPointIndex > 0) {
        ret = ret.substr(0, ret.length() - decimalPointIndex);
    }
    if (ret[0] == '.') {
        ret = "0" + ret;
    }
    if (sign < 0) {
        ret = "-" + ret;
    }

    return ret;
}

