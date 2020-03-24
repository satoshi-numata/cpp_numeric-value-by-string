#ifndef FPValue_hpp
#define FPValue_hpp

#include <string>


struct FPMath;


enum RoundMode {
    /*! 数値をもっとも近い値に丸めます。真ん中の値は0から遠い方に丸められます。いわゆる四捨五入です。 */
    RoundMode_HalfUp,

    /*! 数値をもっとも近い値に丸めます。真ん中の値は0に近い方に丸められます。 */
    RoundMode_HalfDown,

    /*! 数値をその値以上の最小の値に丸めます。 */
    RoundMode_Ceil,

    /*! 数値をその値以下の最大の値に丸めます。 */
    RoundMode_Floor,

    /*! 数値を0に近づけるように丸めます。 */
    RoundMode_Truncate,
};

/*!
    文字列を使用して、できるだけ正確に数値を表現するためのクラスです。
 */
class FPValue
{
    /*! 符号を表す数値。1か-1 */
    int         sign;

    /*! 数値を表す10進数の数字のみからなる文字列 */
    std::string vstr;

    /*! 小数点以下の数字の個数 */
    int         dp;

    /*! to_s()サポートのための文字列 */
    mutable std::string str_buffer;

public:
    /*! 2つの数値の絶対値の大小比較を行います。|value1|>|value2|のときは正の数を、同じ数であれば0を、|value1|<|value2|のときは負の数をリターンします。 */
    static int AbsCompare(const FPValue& value1, const FPValue& value2);

    /*! 2つの数値の大小比較を行います。value1>value2のときは正の数を、同じ数であれば0を、value1<value2のときは負の数をリターンします。 */
    static int Compare(const FPValue& value1, const FPValue& value2);

    /*! 2つの数値の足し算を計算します。 */
    static FPValue Add(const FPValue& value1, const FPValue& value2);

    /*! 2つの数値の引き算を計算します。 */
    static FPValue Sub(const FPValue& minuend, const FPValue& subtrahend);

    /*! 2つの数値の掛け算を計算します。 */
    static FPValue Mult(const FPValue& factor1, const FPValue& factor2);

    /*!
        2つの数値の割り算を計算します。
        dividendをdivisorで、小数点以下decimalPlace桁まで割り算した結果を計算します。
        @param dividend 割られる数
        @param divisor  割る数
        @param decimalPlace 商を小数点以下何桁まで計算するか
        @param roundLast    最後の数を丸めるかどうか。trueならば四捨五入します。
        @return 割り算の結果
     */
    static FPValue Div(const FPValue& dividend, const FPValue& divisor, int decimalPlace, bool roundLast);

public:
    /*! デフォルトコンストラクタ。数値を0で初期化します。 */
    FPValue();

    /*!
        コンストラクタ。
        符号・数値・小数点を含む"3.14159", "+3.14", "-2.6352"といった文字列を元に、この数値を初期化します。
        @param normalValueExp 符号・数値・小数点を含む数値を表す文字列
     */
    FPValue(const char *normalValueExp);

    /*!
        コンストラクタ。
        符号・数値・小数点を含む"3.14159", "+3.14", "-2.6352"といった文字列を元に、この数値を初期化します。
        @param normalValueExp 符号・数値・小数点を含む数値を表す文字列
     */
    FPValue(const std::string& normalValueExp);

    /*!
        コンストラクタ。
        符号・数値文字列・小数点の位置をそれぞれ個別に指定して、この数値を初期化します。
        @param sign 正の数なら0より大きい値を、負の数なら0より小さい値を指定します。
        @param vstr 数値文字列。符号や小数点を含みません。
        @param dp   小数点以下の数字の数。3.14の場合は2、整数123の場合は0となります。
     */
    FPValue(int sign, std::string vstr, int dp);

    /*! コピー・コンストラクタ */
    FPValue(const FPValue& value);

public:
    /*! この数値がゼロかどうかを判定します。 */
    bool IsZero() const;

    /*! 符号を反転させた数値を作成します。 */
    FPValue Negate() const;

    /*!
        この数値を丸めた数値を作成します。
        @param precision    丸めた結果の小数点以下の桁数（デフォルト値は0）
        @param mode         丸め方法（デフォルト値は通常の四捨五入を表すRoundMode_HalfUp）
     */
    FPValue Round(int precision = 0, RoundMode mode = RoundMode_HalfUp) const;

public:
    /*! 数値部の整数箇所を表す文字列を取得します。 */
    std::string IntegerPart() const;

    /*! 数値部の小数箇所を表す文字列を取得します。 */
    std::string DecimalPart() const;

public:
    /*! 代入演算子のオーバーロード */
    FPValue& operator=(const FPValue& other);

    /*! 単項プラス演算子のオーバーロード */
    FPValue operator+() const;

    /*! 単項マイナス演算子のオーバーロード */
    FPValue operator-() const;

    /*! 加算演算子のオーバーロード */
    FPValue operator+(const FPValue& other) const;

    /*! 減算演算子のオーバーロード */
    FPValue operator-(const FPValue& other) const;

    /*! 乗算演算子のオーバーロード */
    FPValue operator*(const FPValue& other) const;

    /*! 除算演算子のオーバーロード */
    FPValue operator/(const FPValue& other) const;

    /*! 剰余演算子のオーバーロード */
    FPValue operator%(const FPValue& other) const;

    /*! C言語文字列へのキャストのサポート */
    operator const char *() const;

    /*! C++文字列へのキャストのサポート */
    operator std::string() const;

public:
    /*! この数値を表す文字列を、符号・数値・小数点を含む"3.14159", "+3.14", "-2.6352"といった形式でリターンします。 */
    std::string to_s() const;

    /*! この数値を表すC言語文字列を、符号・数値・小数点を含む"3.14159", "+3.14", "-2.6352"といった形式でリターンします。 */
    const char *c_str() const;

public:
    friend FPMath;

};

#endif /* FPValue_hpp */
