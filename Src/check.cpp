#include "check.h"
#include "initialize.h"

// 全局变量
ErrorType   errorFlag   = NO_ERROR;          // 错误标志
WarningType warningFlag = NO_WARNING;        // 警告标志
string      invalidNumber;                   // 非法数字
bool        irrational;                      // 无理数
bool        last_irrational;                 // 上次结果

/**
 * @brief 检查数字的正确性
 * @param num 数字
 * @param len 数字长度
 * @retval 是否正确
 * @note 传入此函数的num只包含数字和'.'
 */
bool checkNum(string num, int len) {
    int count = std::count(num.begin(), num.end(), '.');        // 统计字符串中'.'的个数
    if (count > 1 || num[len - 1] == '.') {
        invalidNumber = num;
        errorFlag     = INVALID_NUMBER;        // 非法数字
        return false;
    }
    return true;
}

/**
 * @brief 检查是否是运算符
 * @param c 操作符
 * @retval 是否为运算符
 * @note 运算符：+-/*^!
 */
bool isOp(char c) { return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '!'); }