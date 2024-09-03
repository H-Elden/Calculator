#ifndef __INITIALIZE_H
#define __INITIALIZE_H

#include "HPReal.h"
#include <gmpxx.h>
#include <iostream>
#include <stack>

using namespace std;
/* 全局变量的声明 */
extern stack<HPReal> h;                // 后缀表达式运算栈
extern stack<char>   op;               // 中缀表达式的符号栈，只存+-*/^(
extern string        s;                // 中缀表达式
extern HPReal        ans;              // 上次计算结果
extern mp_exp_t      precision;        // 计算精度
extern int           base;             // 进制
/* 全局常量的声明 */
extern const HPReal e;           // 自然常数
extern const HPReal Pi;          // 圆周率
extern const HPReal zero;        // 零

void Init(void);
int  pushNum(int i);

#endif /* __INITIALIZE_H */