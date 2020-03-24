#ifndef FPMath_hpp
#define FPMath_hpp

#include "FPValue.hpp"


struct FPMath
{
    /*! 自然対数の底eを求めます。 */
    static FPValue  LogBaseE(int dp);

    /*! サインを計算します。 */
    static FPValue  Sin(const FPValue& angle, int dp);

    /*! コサインを計算します。 */
    static FPValue  Cos(const FPValue& angle, int dp);

    /*! 数値baseをexponent乗した数値を計算します。 */
    static FPValue  Pow(const FPValue& base, const FPValue& exponent, int macCount);

};

#endif /* FPMath_hpp */