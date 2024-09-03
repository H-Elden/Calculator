#ifndef __HPREAL_H
#define __HPREAL_H

#include <cmath>        //用于使用pow()函数
#include <gmpxx.h>
#include <iostream>
#include <limits.h>        // 用于获取unsigned long int的最大值

using namespace std;

/**
 * @class 高精度整数类
 */
class HPReal {
  public:
    // 构造函数
    HPReal();
    HPReal(const string &float_str);
    // 拷贝构造函数
    HPReal(const HPReal &other);
    // 析构函数
    ~HPReal();
    // 赋值
    HPReal &operator=(const HPReal &other);
    // 基本操作：加、减、乘、除
    HPReal operator+(const HPReal &other) const;
    HPReal operator-(const HPReal &other) const;
    HPReal operator*(const HPReal &other) const;
    HPReal operator/(const HPReal &other) const;
    // 特殊计算：幂
    HPReal power(const HPReal &exponent) const;
    // 比较
    bool operator==(const HPReal &other) const;
    bool isZ();
    bool Positive();
    int  Compare_ui(unsigned long int z);
    // 打印函数
    void print();
    void print_f();

  private:
    mpq_t fraction;
};

extern const HPReal zero;

#endif /* __HPREAL_H */