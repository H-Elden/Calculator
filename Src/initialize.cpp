#include "initialize.h"
#include "check.h"

/* 全局变量 */
stack<HPReal> h;                     // 后缀表达式运算栈
stack<char>   op;                    // 中缀表达式的符号栈，只存+-*/^(
HPReal        ans;                   // 上次计算结果
mp_exp_t      precision = 10;        // 计算精度，默认10位有效数字
int           base      = 10;        // 进制，默认10进制
/* 全局常量 */
const HPReal e("2.718281828");         // 自然常数
const HPReal Pi("3.141592653");        // 圆周率

/**
 * @brief 初始化栈和错误标志
 */
void Init() {
    errorFlag   = NO_ERROR;
    warningFlag = NO_WARNING;
    irrational  = false;
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
    string t = s.substr(i, j - i);        // 截取数字子串
    if (checkNum(t, j - i)) {
        HPReal fraction(t);        // 转为HPReal型
        h.push(fraction);          // 入栈
    }
    return j - 1;        // 返回这个数字最后一位字符的下标
}
