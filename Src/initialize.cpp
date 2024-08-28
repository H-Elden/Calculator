#include "initialize.h"

/* 全局变量 */
stack<double> h;          // 后缀表达式运算栈
stack<char>   op;         // 中缀表达式的符号栈，只存+-*/^(
double        ans;        // 上次计算结果
/* 全局常量 */
const double e  = 2.718281828;        // 自然常数
const double Pi = 3.141592653;        // 圆周率

/**
 * @brief 初始化栈和错误标志
 */
void Init() {
    errorFlag = NO_ERROR;
    while (!h.empty()) h.pop();
    while (!op.empty()) op.pop();
}

/**
 * @brief 将数字入栈
 * @param i 数字的起始下标
 * @retval 数字最后一位字符的下标
 */
int pushNum(int i) {
    int j, len = s.size();
    for (j = i; j < len; j++)
        if (!isdigit(s[j]) && s[j] != '.')        // 非数字或小数点
            break;
    string t = s.substr(i, j - i);                  // 截取数字子串
    if (checkNum(t, j - i)) h.push(stod(t));        // 转为double型，入栈
    return j - 1;                                   // 返回这个数字最后一位字符的下标
}