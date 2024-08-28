#ifndef __INITIALIZE_H
#define __INITIALIZE_H

#include "check.h"
#include <iostream>
#include <stack>

using namespace std;
/* 全局变量的声明 */
extern stack<double> h;          // 后缀表达式运算栈
extern stack<char>   op;         // 中缀表达式的符号栈，只存+-*/^(
extern string        s;          // 中缀表达式
extern double        ans;        // 上次计算结果
/* 全局常量的声明 */
extern const double e;         // 自然常数
extern const double Pi;        // 圆周率

void Init(void);
int  pushNum(int i);

#endif /* __INITIALIZE_H */