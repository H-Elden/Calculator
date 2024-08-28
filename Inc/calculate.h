#ifndef __CALCULATE_H
#define __CALCULATE_H

#include "initialize.h"
#include <cmath>

/* 计算函数的声明 */
int  OpScore(char c);
void Cal(void);
void handleInput(const string &s);
void processExpression(const string &s, int begin);
void handleOperator(char op);
void handleClosingParenthesis();
void finalizeCalculation();
void outputResult();

#endif /* __CALCULATE_H */