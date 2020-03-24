#include "IntStringHelper.hpp"
#include <vector>


// 正の整数を表す文字列を、不要なゼロが付いていない形式に正規化する。
std::string IntString_Normalize(const std::string& istr)
{
    if (istr.length() == 0) {
        return "0";
    }
    std::string ret = istr;
    while (ret.length() > 1 && ret[0] == '0') {
        ret = ret.substr(1);
    }
    return ret;
}

// 正の整数を表す文字列（正規化済み）の大小比較
int IntString_Compare(const std::string& istr_n_1, const std::string& istr_n_2)
{
    int len1 = (int)istr_n_1.length();
    int len2 = (int)istr_n_2.length();

    // 桁数が異なる場合、桁数の大小で結果が決まる
    if (len1 != len2) {
        return (len1 > len2)? 1: -1;
    }

    // 桁数が同じ場合、上の桁から比較していく
    for (int i = 0; i < len1; i++) {
        int v1 = (int)(istr_n_1[i] - '0');
        int v2 = (int)(istr_n_2[i] - '0');
        if (v1 != v2) {
            return (v1 > v2)? 1: -1;
        }
    }
    return 0;
}

// 正の整数を表す文字列同士で、足し算を計算する。
std::string IntString_Add(const std::string& istr_n_1, const std::string& istr_n_2)
{
    // 桁数を合わせる
    std::string str1 = istr_n_1;
    std::string str2 = istr_n_2;
    int len1 = (int)str1.length();
    int len2 = (int)str2.length();
    while (len1 < len2) {
        str1 = "0" + str1;
        len1++;
    }
    while (len2 < len1) {
        str2 = "0" + str2;
        len2++;
    }

    // 足し算を計算する
    std::string result = "";
    int overflow = 0;
    for (int i = 0; i < len1; i++) {
        int v1 = str1[len1-i-1] - '0';
        int v2 = str2[len1-i-1] - '0';
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

    // 正規化した値をリターンする
    return IntString_Normalize(result);
}


// 正の整数を表す文字列同士で、引き算を計算する。
std::string IntString_Sub(const std::string& minuend_istr_n, const std::string& subtrahend_istr_n)
{
    // 引かれる数が引く数よりも小さい場合、"0"をリターン
    if (IntString_Compare(minuend_istr_n, subtrahend_istr_n) <= 0) {
        return "0";
    }

    // 桁数を合わせる
    std::string subtrahend_istr = subtrahend_istr_n;
    int len = (int)minuend_istr_n.length();
    int len_diff = len - (int)subtrahend_istr_n.length();
    for (int i = 0; i < len_diff; i++) {
        subtrahend_istr = "0" + subtrahend_istr;
    }

    // 引き算を計算する
    std::string result = "";
    bool underflow = false;
    for (int i = 0; i < len; i++) {
        int v1 = (int)(minuend_istr_n[len-i-1] - '0');
        int v2 = (int)(subtrahend_istr[len-i-1] - '0');
        //printf("  %d - %d (underflow=%d) ", v1, v2, (int)underflow);
        if (underflow) {
            v1 -= 1;
        }
        int v3 = v1 - v2;
        underflow = (v3 < 0);
        if (underflow) {
            v3 += 10;
        }
        //printf("=> %d\n", v3);
        result = (char)(v3 + '0') + result;
    }

    // 正規化してリターン
    return IntString_Normalize(result);
}

// 正の整数を表す文字列同士で、掛け算を計算する。
std::string IntString_Mult(const std::string& istr_n_1, const std::string& istr_n_2)
{
    // 各桁ごとに掛け算を計算
    std::vector<std::string> results;
    int len2 = (int)istr_n_2.length();
    for (int i = 0; i < len2; i++) {
        std::string result = "";
        for (int j = 0; j < i; j++) {
            result = result + "0";
        }
        int v2 = istr_n_2[len2-i-1] - '0';
        int overflow = 0;
        int len1 = (int)istr_n_1.length();
        for (int j = 0; j < len1; j++) {
            int v1 = istr_n_1[len1-j-1] - '0';
            int v3 = v1 * v2 + overflow;
            overflow = v3 / 10;
            v3 -= overflow * 10;
            result = (char)(v3 + '0') + result;
        }
        if (overflow > 0) {
            result = (char)(overflow + '0') + result;
        }
        results.push_back(result);
    }

    // 結果をすべて足し合わせる
    std::string resultValue = "0";
    for (int i = 0; i < results.size(); i++) {
        resultValue = IntString_Add(resultValue, results[i]);
    }

    // 正規化してリターンする
    return IntString_Normalize(resultValue);
}

// 正の整数を表す文字列同士で、割り算を計算する。
// 商(quotient)をfirst, 余り(remainder)をsecondにしたFPValueのペアをリターンする。
std::pair<std::string, std::string> IntString_Div(const std::string& dend_istr_n, const std::string& dor_istr_n)
{
    // ゼロ除算のチェック
    if (dor_istr_n == "0") {
        throw std::runtime_error("Zero division is now allowed.");
    }

    // 割り算を計算する
    std::string result_istr = "";
    int dend_len = (int)dend_istr_n.length();
    int div_pos = 0;
    std::string remain_istr = "";
    while (div_pos < dend_len) {
        // 次の割られる数をセット
        remain_istr = (remain_istr != "0"? remain_istr: "") + dend_istr_n.substr(div_pos, 1);
        //printf("1: remain_istr=%s\n", remain_istr.c_str());

        // 割る数を引ける回数をカウントして商とする
        int sub_count = 0;
        while (IntString_Compare(remain_istr, dor_istr_n) >= 0) {
            remain_istr = IntString_Sub(remain_istr, dor_istr_n);
            sub_count++;
        }
        //printf("2: sub_count=%d, remain_istr=%s\n", sub_count, remain_istr.c_str());
        result_istr = result_istr + (char)(sub_count + '0');
        //printf("3: result_istr=%s\n", result_istr.c_str());
        div_pos++;
    }

    // 正規化してリターン
    result_istr = IntString_Normalize(result_istr);
    return std::make_pair(result_istr, remain_istr);
}

// 正の整数の階乗
std::string IntString_Fact(const std::string& istr_n)
{
    std::string ret = "1";

    // ゼロの階乗は1とする
    if (istr_n == "0") {
        return ret;
    }

    // 階乗の計算
    std::string n = istr_n;
    while (n != "1") {
        ret = IntString_Mult(ret, n);
        n = IntString_Sub(n, "1");
    }
    return ret;
}

// 整数baseのexponent乗
std::string IntString_Pow(const std::string& base, const std::string& exponent)
{
    if (exponent == "0") {
        return "1";
    }
    std::string ret = "1";
    std::string exp = exponent;
    while (exp != "0") {
        ret = IntString_Mult(ret, base);
        exp = IntString_Sub(exp, "1");
    }
    return ret;
}
