#include "IntStringHelper.hpp"


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

// 正の整数を表す文字列同士で、割り算を計算する。
// 商(quotient)をfirst, 余り(remainder)をsecondにしたFPValueのペアをリターンする。
std::pair<std::string, std::string> IntString_Div(const std::string& dend_istr_n, const std::string& dor_istr_n)
{
    // ゼロ除算のチェック
    if (dor_istr_n == "0") {
        throw std::runtime_error("Zero division");
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


