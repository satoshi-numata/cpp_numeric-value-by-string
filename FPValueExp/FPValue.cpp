#include "FPValue.hpp"
#include "IntStringHelper.hpp"
#include "FPMath.hpp"

#include <cassert>
#include <cctype>
#include <cstdio>
#include <strstream>
#include <stdexcept>


/*!
    小数点の位置を合わせて、数値を表す文字列の長さを揃えます。
    @param vstr1    数値1の文字列
    @param dp1      数値1の小数点以下の数字の個数
    @param vstr2    数値2の文字列
    @param dp2      数値2の小数点以下の数字の個数
 */
static void AdjustValueStringLengths(std::string& vstr1, int& dp1, std::string& vstr2, int& dp2)
{
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
}

/*!
    数値を表す文字列の前後から、不要なゼロを削除します。
    @param vstr     数値文字列
    @param dp       小数点以下の数字の個数
 */
static void RemoveRedundantZeros(std::string& vstr, int& dp)
{
    // 小数点以下の不要な0の削除
    while (dp > 0) {
        if (vstr[vstr.length()-1] == '0') {
            vstr = vstr.substr(0, vstr.length()-1);
            dp--;
        } else {
            break;
        }
    }

    // 整数部の不要な0の削除
    int intLen = (int)vstr.length() - dp;
    for (int i = 0; i < intLen - 1; i++) {
        if (vstr[0] == '0') {
            vstr = vstr.substr(1);
        }
    }
}

// 2つの数値の絶対値の大小比較
int FPValue::AbsCompare(const FPValue& value1, const FPValue& value2)
{
    //printf("AbsCompare (%s, %s)\n", value1.to_s().c_str(), value2.to_s().c_str());

    // 文字列と小数点の位置を取得して長さを揃える
    std::string vstr1 = value1.vstr;
    std::string vstr2 = value2.vstr;
    int dp1 = value1.dp;
    int dp2 = value2.dp;
    AdjustValueStringLengths(vstr1, dp1, vstr2, dp2);

    // 上の桁から大小比較していく
    for (int i = 0; i < vstr1.length(); i++) {
        int v1 = (int)(vstr1[i] - '0');
        int v2 = (int)(vstr2[i] - '0');
        if (v1 != v2) {
            return (v1 > v2)? 1: -1;
        }
    }

    return 0;
}

// 2つの数値の大小比較
int FPValue::Compare(const FPValue& value1, const FPValue& value2)
{
    //printf("Compare (%s, %s)\n", value1.to_s().c_str(), value2.to_s().c_str());

    // 符号が異なる場合、プラスの数値の方が大きい
    if (value1.sign != value2.sign) {
        //printf("  Diff signs\n");
        return (value1.sign > 0)? 1: -1;
    }

    // 絶対値の大小比較結果を符号に合わせる
    return AbsCompare(value1, value2) * value1.sign;
}

// 2つの数値の足し算
FPValue FPValue::Add(const FPValue& value1, const FPValue& value2)
{
    //printf("Add (%s, %s)\n", value1.to_s().c_str(), value2.to_s().c_str());

    // どちらかがゼロならば、ゼロでない方の数値をそのままリターンする
    if (value1.IsZero()) {
        return value2;
    } else if (value2.IsZero()) {
        return value1;
    }
    
    // 符号が異なる場合は引き算として処理する
    if (value1.sign > 0 && value2.sign < 0) {
        //printf("  -> Sub\n");
        return FPValue::Sub(value1, value2.Negate());
    } else if (value1.sign < 0 && value2.sign > 0) {
        //printf("  -> Sub\n");
        return FPValue::Sub(value2, value1.Negate());
    }
    
    // 文字列と小数点の位置を取得して長さを揃える
    std::string vstr1 = value1.vstr;
    std::string vstr2 = value2.vstr;
    int dp1 = value1.dp;
    int dp2 = value2.dp;
    AdjustValueStringLengths(vstr1, dp1, vstr2, dp2);
    //printf("  vstr1=[%s], dp1=%d, vstr2=[%s], dp2=%d\n", vstr1.c_str(), dp1, vstr2.c_str(), dp2);

    // 足し算を計算する
    std::string result = IntString_Add(vstr1, vstr2);
    //printf("  result=[%s], dp1=%d\n", result.c_str(), dp1);

    // 前後の不要な0を取り除く
    RemoveRedundantZeros(result, dp1);
    //printf("  result=[%s], dp1=%d\n", result.c_str(), dp1);

    // 計算結果をリターンする
    return FPValue(value1.sign, result, dp1);
}

// 2つの数値の引き算
FPValue FPValue::Sub(const FPValue& minuend, const FPValue& subtrahend)
{
    //printf("Sub (%s, %s)\n", minuend.to_s().c_str(), subtrahend.to_s().c_str());

    // minuendが0ならsubtrahendの符号を反転させたものをリターンする
    if (minuend.IsZero()) {
        return subtrahend.Negate();
    }
    // subtrahendが0ならminuendをそのままリターンする
    else if (subtrahend.IsZero()) {
        return minuend;
    }

    // 符号が異なる場合は右辺の符号を反対にして足し算
    if (minuend.sign != subtrahend.sign) {
        //printf("  -> Add\n");
        return Add(minuend, subtrahend.Negate());
    }

    // v1を大きな数、v2を小さな数にする。同じ数の場合にはゼロをリターンする
    int sign = minuend.sign;
    FPValue v1(minuend);
    FPValue v2(subtrahend);
    int comp = AbsCompare(v1, v2);
    if (comp == 0) {
        return FPValue();
    } else if (comp < 0) {
        sign *= -1;
        v1 = subtrahend;
        v2 = minuend;
    }
    //printf("  v1=[%s], v2=[%s], sign=%d\n", v1.to_s().c_str(), v2.to_s().c_str(), sign);

    // 文字列と小数点の位置を取得して長さを揃える
    std::string vstr1 = v1.vstr;
    std::string vstr2 = v2.vstr;
    int dp1 = v1.dp;
    int dp2 = v2.dp;
    AdjustValueStringLengths(vstr1, dp1, vstr2, dp2);
    //printf("  vstr1=[%s], dp1=%d, vstr2=[%s], dp2=%d\n", vstr1.c_str(), dp1, vstr2.c_str(), dp2);

    // 引き算の計算
    std::string result = IntString_Sub(vstr1, vstr2);

    // 前後の不要な0を取り除いてリターン
    RemoveRedundantZeros(result, dp1);
    return FPValue(sign, result, dp1);
}

// 2つの数値の掛け算
FPValue FPValue::Mult(const FPValue& factor1, const FPValue& factor2)
{
    // どちらかがゼロならば、結果はゼロ
    if (factor1.IsZero() || factor2.IsZero()) {
        return FPValue();
    }

    // 結果の符号は、符号同士の掛け算
    int sign = factor1.sign * factor2.sign;

    // 掛け算自体は正の整数として計算する
    std::string result = IntString_Mult(factor1.vstr, factor2.vstr);

    // 小数点の数を足してリターンする
    return FPValue(sign, result, factor1.dp + factor2.dp);
}

// 2つの数値の割り算
FPValue FPValue::Div(const FPValue& dividend, const FPValue& divisor, int decimalPlace, bool roundLast)
{
    assert(decimalPlace >= 0);
    //printf("Div (%s, %s, dplace=%d)\n", dividend.to_s().c_str(), divisor.to_s().c_str(), decimalPlace);

    // ゼロ除算のチェック
    if (divisor.IsZero()) {
        throw std::runtime_error("Zero division is now allowed.");
    }

    // 数値文字列と小数点以下の数字の個数を取り出し、桁数を合わせる
    std::string remain_str = dividend.vstr;
    int remain_dp = dividend.dp;
    std::string dor_str = divisor.vstr;
    int dor_dp = divisor.dp;
    AdjustValueStringLengths(remain_str, remain_dp, dor_str, dor_dp);
    remain_str = IntString_Normalize(remain_str);
    dor_str = IntString_Normalize(dor_str);

    // 整数部の割り算
    std::string result = "";
    std::pair<std::string, std::string> div = IntString_Div(remain_str, dor_str);
    result = result + div.first;
    remain_str = div.second;

    // 小数部の割り算
    if (remain_str != "0" && decimalPlace > 0) {
        std::string roundStr = "0.";
        result = result + ".";
        for (int i = 0; i < decimalPlace+1; i++) {
            remain_str = remain_str + "0";
            div = IntString_Div(remain_str, dor_str);
            result = result + div.first;
            remain_str = div.second;
            roundStr = roundStr + ((i < decimalPlace)? "0": "5");
        }

        // 最後の数の丸め
        if (roundLast) {
            result = FPValue::Add(FPValue(result), FPValue(roundStr)).to_s();
            // TODO: 桁数がおかしい場合がある。1/22を10桁表示する場合など。
            result = result.substr(0, result.length() - 1);
        } else {
            result = result.substr(0, result.length()-1);
        }
    }

    // 符号をつける
    if (dividend.sign * divisor.sign < 0) {
        result = "-" + result;
    }

    return FPValue(result);
}


// デフォルトコンストラクタ
FPValue::FPValue()
    : sign(1), vstr("0"), dp(0)
{}

// コンストラクタ。"3.14159", "+3.14", "-2.6352"といった文字列を元に初期化する。
FPValue::FPValue(const char *cNormalValueExp)
{
    // 初期化
    sign = 1;
    dp = 0;
    std::string normalValueExp = cNormalValueExp;

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
                vstr += c;
                dp++;
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
                vstr += c;
            } else {
                std::strstream sstr;
                sstr << "Unknown character appeared (integer part): " << c << " (index=" << i << ")";
                throw std::runtime_error(sstr.str());
            }
        }
    }

    // 前後の不要な0を削除する
    RemoveRedundantZeros(vstr, dp);

    // マイナスの0は許容しない
    if (vstr == "0" && dp == 0) {
        sign = 1;
    }
}

// コンストラクタ。"3.14159", "+3.14", "-2.6352"といった文字列を元に初期化する。
FPValue::FPValue(const std::string& normalValueExp)
{
    // 初期化
    sign = 1;
    dp = 0;

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
                vstr += c;
                dp++;
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
                vstr += c;
            } else {
                std::strstream sstr;
                sstr << "Unknown character appeared (integer part): " << c << " (index=" << i << ")";
                throw std::runtime_error(sstr.str());
            }
        }
    }

    // 前後の不要な0を削除する
    RemoveRedundantZeros(vstr, dp);

    // マイナスの0は許容しない
    if (vstr == "0" && dp == 0) {
        sign = 1;
    }
}

// コンストラクタ。符号、数値文字列、小数点以下の数字の個数を元に初期化する。
FPValue::FPValue(int _sign, std::string _vstr, int _dp)
{
    // 値の検証
    assert(_sign != 0);
    assert(_vstr.length() > 0);
    for (int i = 0; i < _vstr.length(); i++) {
        assert(isdigit(_vstr[i]));
    }
    assert(_dp >= 0);

    // それぞれの値をメンバ変数にコピー
    sign = (_sign > 0)? 1: -1;
    vstr = _vstr;
    dp = _dp;
    while (dp > vstr.length()) {
        vstr = "0" + vstr;
    }

    // 小数点以下の不要な0の削除
    while (dp > 0) {
        if (vstr[vstr.length()-1] == '0') {
            vstr = vstr.substr(0, vstr.length()-1);
            dp--;
        } else {
            break;
        }
    }

    // 前後の不要な0を削除する
    RemoveRedundantZeros(vstr, dp);

    // マイナスの0は許容しない
    if (vstr == "0" && dp == 0) {
        sign = 1;
    }
}

// コピー・コンストラクタ
FPValue::FPValue(const FPValue& value)
    : sign(value.sign), vstr(value.vstr), dp(value.dp)
{}

// この数値がゼロかどうかを判定
bool FPValue::IsZero() const
{
    return (vstr == "0" && dp == 0);
}

// 符号を反転させた数値を作成する。
FPValue FPValue::Negate() const
{
    return FPValue((sign > 0)? -1: 1, vstr, dp);
}

// 代入演算子のオーバーロード
FPValue& FPValue::operator=(const FPValue& other)
{
    sign = other.sign;
    vstr = other.vstr;
    dp = other.dp;
    return *this;
}

// 単項プラス演算子のオーバーロード
FPValue FPValue::operator+() const
{
    return *this;
}

// 単項マイナス演算子のオーバーロード
FPValue FPValue::operator-() const
{
    return Negate();
}

// 2項加算演算子のオーバーロード
FPValue FPValue::operator+(const FPValue& other) const
{
    return FPValue::Add(*this, other);
}

// 2項減算演算子のオーバーロード
FPValue FPValue::operator-(const FPValue& other) const
{
    return FPValue::Sub(*this, other);
}

// 2項乗算演算子のオーバーロード
FPValue FPValue::operator*(const FPValue& other) const
{
    return FPValue::Mult(*this, other);
}

// 2項除算演算子のオーバーロード
FPValue FPValue::operator/(const FPValue& other) const
{
    return FPValue::Div(*this, other, 10, true);
}

// 剰余演算子のオーバーロード
FPValue FPValue::operator%(const FPValue& other) const
{
    FPValue quot = FPValue::Div(*this, other, 0, false);
    return (*this - other * quot);
}

// 累乗演算子のオーバーロード
FPValue FPValue::operator^(const FPValue& exponent) const
{
    return FPMath::Pow(*this, exponent, 5);
}

// 加算代入演算子のオーバーロード
FPValue& FPValue::operator+=(const FPValue& other)
{
    *this = FPValue::Add(*this, other);
    return *this;
}

// 減算代入演算子のオーバーロード
FPValue& FPValue::operator-=(const FPValue& other)
{
    *this = FPValue::Sub(*this, other);
    return *this;
}

// 乗算代入演算子のオーバーロード
FPValue& FPValue::operator*=(const FPValue& other)
{
    *this = FPValue::Mult(*this, other);
    return *this;
}

// 除算代入演算子のオーバーロード
FPValue& FPValue::operator/=(const FPValue& other)
{
    *this = *this / other;
    return *this;
}

// 累乗代入演算子のオーバーロード
FPValue& FPValue::operator^=(const FPValue& other)
{
    *this = *this ^ other;
    return *this;
}


// C言語文字列へのキャストのサポート
FPValue::operator const char *() const
{
    to_s();
    return str_buffer.c_str();
}

// C++文字列へのキャストのサポート
FPValue::operator std::string() const
{
    return to_s();
}

// FPValueを表す文字列表現に変換する。
std::string FPValue::to_s() const
{
    str_buffer = vstr;
    bool hasDecimalPoint = (dp > 0);
    if (hasDecimalPoint) {
        hasDecimalPoint = false;
        for (int i = (int)str_buffer.length() - dp; i < str_buffer.length(); i++) {
            if (str_buffer[i] != '0') {
                hasDecimalPoint = true;
                break;
            }
        }
    }
    if (hasDecimalPoint) {
        str_buffer.insert(str_buffer.length() - dp, ".");
    } else if (dp > 0) {
        str_buffer = str_buffer.substr(0, str_buffer.length() - dp);
    }
    if (str_buffer[0] == '.') {
        str_buffer = "0" + str_buffer;
    }
    if (sign < 0) {
        str_buffer = "-" + str_buffer;
    }

    return str_buffer;
}

// FPValueを表すC言語の文字列表現に変換する。
const char *FPValue::c_str() const
{
    to_s();
    return str_buffer.c_str();
}


