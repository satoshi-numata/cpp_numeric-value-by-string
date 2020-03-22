#include <cstdio>
#include <exception>
#include "FPValue.hpp"


int main()
{
    try {
        FPValue value1("2");
        FPValue value2("20");

        printf("value1=[%s]\n", value1.to_s().c_str());
        printf("value2=[%s]\n", value2.to_s().c_str());

        // 2数の演算
        //printf("value1+value2=[%s]\n", (FPValue::Add(value1, value2)).to_s().c_str());
        //printf("value1-value2=[%s]\n", (FPValue::Sub(value1, value2)).to_s().c_str());
        //printf("value1*value2=[%s]\n", (FPValue::Mult(value1, value2)).to_s().c_str());

        // 割り算は、割られる数・割る数・商の小数点以下の桁数
        auto div = FPValue::Div(value1, value2, 10);
        printf("value1/value2=[%s]...[%s]\n", div.first.to_s().c_str(), div.second.to_s().c_str());
    } catch (std::exception& e) {
        printf("Error: %s\n", e.what());
    }

    return 0;
}

