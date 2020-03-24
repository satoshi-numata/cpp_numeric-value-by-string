#ifndef IntStringHelper_hpp
#define IntStringHelper_hpp

#include <string>
#include <utility>

/*!
    正の整数を表す文字列を、不要なゼロが付いていない形式に正規化します。
 */
std::string IntString_Normalize(const std::string& istr);

/*!
    正の整数を表す文字列（正規化済み）の大小比較を行います。
    @return istr_n_1がistr_n_2よりも大きければ1、小さければ-1、等しければ0
 */
int IntString_Compare(const std::string& istr_n_1, const std::string& istr_n_2);

/*!
    正の整数を表す文字列同士で、足し算を計算します。
 */
std::string IntString_Add(const std::string& istr_n_1, const std::string& istr_n_2);

/*!
    正の整数を表す文字列同士で、引き算を計算します。
 */
std::string IntString_Sub(const std::string& minuend_istr_n, const std::string& subtrahend_istr_n);

/*!
    正の整数を表す文字列同士で、掛け算を計算します。
 */
std::string IntString_Mult(const std::string& istr_n_1, const std::string& istr_n_2);

/*!
    正の整数を表す文字列同士で、割り算を計算します。
    @param  dend_istr_n 割られる数を表す正規化された整数文字列 (dividend)
    @param  dor_istr_n  割る数を表す正規化された整数文字列 (divisor)
    @return 商(quotient)をfirst, 余り(remainder)をsecondにしたFPValueのペア
 */
std::pair<std::string, std::string> IntString_Div(const std::string& dend_istr_n, const std::string& dor_istr_n);

/*!
    正の整数の階乗を計算します。
 */
std::string IntString_Fact(const std::string& istr_n);

/*!
    正の整数baseを、正の整数exponent乗した数値を計算します。
 */
std::string IntString_Pow(const std::string& base, const std::string& exponent);

#endif /* IntStringHelper_hpp */
