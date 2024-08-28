#ifndef __CHECK_H
#define __CHECK_H

#include "initialize.h"
#include <algorithm>        // 包含 std::count
#include <iostream>

using namespace std;

enum ErrorType {
    NO_ERROR,                  // 无错误
    INVALID_NUMBER,            // 非法数字
    INVALID_OPERATOR,          // 非法操作符
    INPUT_EMPTY,               // 输入为空
    ONE_MORE_NUMBER,           // 多一个数字
    TOO_MANY_NUMBERS,          // 太多数字
    TOO_MANY_OPERATORS,        // 太多操作符
    DIVISION_BY_ZERO,          // 除数为0
    FACTORIAL_ERROR,           // 阶乘错误
    POWER_ERROR,               // 指数运算错误
    RESULT_NAN,                // 结果非数字
    RESULT_INF,                // 结果无穷大
    UNKNOWN_ERROR              // 未知错误
};

extern ErrorType errorFlag;
extern string    invalidNumber;

inline void setError(ErrorType type) { errorFlag = type; }

bool checkNum(string num, int len);
bool isOp(char c);

#endif /* __CHECK_H */
