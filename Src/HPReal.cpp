#include "HPReal.h"
#include "check.h"
#include "initialize.h"        //包含精度

const HPReal zero("0");        // 零

/**
 * @brief 构造函数
 */
HPReal::HPReal() { mpq_init(fraction); }

HPReal::HPReal(const string &float_str) {
    mpq_init(fraction);
    size_t dot_pos = float_str.find('.');
    if (dot_pos == std::string::npos) {
        // 没有小数点，直接设置整数
        mpq_set_str(fraction, float_str.c_str(), 10);
        return;
    }

    // 整数部分
    string int_part = float_str.substr(0, dot_pos);
    // 小数部分
    string frac_part = float_str.substr(dot_pos + 1);

    // 计算分母
    mpz_t denominator;
    mpz_init(denominator);
    mpz_ui_pow_ui(denominator, 10, frac_part.length());

    // 计算分子
    mpz_t numerator;
    mpz_init(numerator);
    mpz_set_str(numerator, int_part.c_str(), 10);        // 整数部分
    mpz_mul(numerator, numerator, denominator);          // 整数部分乘以分母
    mpz_t frac_part_num;
    mpz_init(frac_part_num);
    mpz_set_str(frac_part_num, frac_part.c_str(), 10);        // 小数部分
    mpz_add(numerator, numerator, frac_part_num);             // 加上小数部分

    // 设置分数
    mpq_set_z(fraction, numerator);
    mpq_set_den(fraction, denominator);

    mpq_canonicalize(fraction);        // 约分为最简形式

    // 清理
    mpz_clear(denominator);
    mpz_clear(numerator);
    mpz_clear(frac_part_num);
}

/**
 * @brief 拷贝构造函数
 * @note 如果不利用深拷贝在堆区创建新内存，会导致浅拷贝带来的重复释放堆区问题
 */
HPReal::HPReal(const HPReal &other) {
    mpq_init(fraction);
    mpq_set(fraction, other.fraction);        // 复制值
}

/**
 * @brief 析构函数
 */
HPReal::~HPReal() { mpq_clear(fraction); }

/**
 * @brief 打印分数
 */
void HPReal::print() {
    mpq_canonicalize(fraction);             // 约分为最简形式
    if (this->isZ())                        // 如果是整数
        gmp_printf("%Qd", fraction);        // 这样输出的就是高精度的整数
    else {                                  // 如果不是整数
        // 先输出小数
        this->print_f();
        // 如果是有理数，再输出分数
        if (!irrational) {
            printf(" or ");
            gmp_printf("%Qd", fraction);
        }
    }
}

/**
 * @brief 打印分数的小数形式
 */
void HPReal::print_f() {
    mpf_t num;                                                             // 存储浮点形式
    mpf_init2(num, 100);                                                   // 设置浮点精度
    mpf_set_q(num, fraction);                                              // 把分数转换成小数
    mp_exp_t exp;                                                          // 用于保存小数点的位置
    char    *num_str = mpf_get_str(NULL, &exp, 10, precision, num);        // 转换为字符串
    size_t   len     = strlen(num_str);                                    // 获取字符串长度
    if (exp <= -precision || exp >= precision) {                           // 采用科学计数法表示
        cout << *num_str << '.' << (num_str + 1);                          // 第一个数字 + '.' + 后面的数字
        cout << 'e' << exp;                                                // 指数
    } else {                                                               // 采用小数形式表示
        if (exp <= 0) {                                                    // 小于1
            cout << "0.";
            for (int i = exp; i < 0; i++) cout << '0';        // 补0
            cout << num_str;
        } else {                                                         // 大于1
            for (int i = 0; i < exp; i++) cout << *(num_str + i);        // 输出整数部分
            cout << '.' << num_str + exp;                                // 输出小数部分
        }
    }
    // 释放内存
    free(num_str);
    // 清理
    mpf_clear(num);
}

/**
 * @brief 重载赋值运算符
 */
HPReal &HPReal::operator=(const HPReal &other) {
    if (this != &other) {                         // 避免自赋值
        mpq_set(fraction, other.fraction);        // 复制 other 的值到当前对象
        mpq_canonicalize(fraction);               // 约分为最简形式
    }
    return *this;        // 返回当前对象的引用
}

/**
 * @brief 重载加号
 */
HPReal HPReal::operator+(const HPReal &other) const {
    HPReal result;
    mpq_add(result.fraction, this->fraction, other.fraction);
    mpq_canonicalize(result.fraction);        // 约分为最简形式
    return result;
}

/**
 * @brief 重载减号
 */
HPReal HPReal::operator-(const HPReal &other) const {
    HPReal result;
    mpq_sub(result.fraction, this->fraction, other.fraction);
    mpq_canonicalize(result.fraction);        // 约分为最简形式
    return result;
};

/**
 * @brief 重载乘号
 */
HPReal HPReal::operator*(const HPReal &other) const {
    HPReal result;
    mpq_mul(result.fraction, this->fraction, other.fraction);
    mpq_canonicalize(result.fraction);        // 约分为最简形式
    return result;
};

/**
 * @brief 重载除号
 */
HPReal HPReal::operator/(const HPReal &other) const {
    HPReal result;
    mpq_div(result.fraction, this->fraction, other.fraction);
    mpq_canonicalize(result.fraction);        // 约分为最简形式
    return result;
}

/**
 * @brief 幂运算
 */
HPReal HPReal::power(const HPReal &exponent) const {
    HPReal result, exp = exponent;
    if (exp == zero) {
        mpq_set_ui(result.fraction, 1, 1);
        return result;
    } else if (exp.isZ() && exp.Compare_ui(0) > 0) {            // 指数为正整数
        mpz_t res_num, res_den, base_num, base_den, i_z;        // 定义底数和结果的分子分母。循环变量
        mpz_inits(base_num, base_den, NULL);                    // 初始化底数的分子分母
        mpz_init_set_ui(res_num, 1);                            // 初始化结果的分子为1
        mpz_init_set_ui(res_den, 1);                            // 初始化结果的分母为1
        mpq_get_num(base_num, this->fraction);                  // 底数的分子赋值给res_num
        mpq_get_den(base_den, this->fraction);                  // 底数的分母赋值给res_den

        mpz_t exp_z;                             // 定义指数
        mpz_init(exp_z);                         // 初始化指数
        mpq_get_num(exp_z, exp.fraction);        // 设置为分子
        for (mpz_init(i_z); mpz_cmp(i_z, exp_z) < 0; mpz_add_ui(i_z, i_z, 1)) {
            mpz_mul(res_num, res_num, base_num);        // res_num = res_num * base_num;
            mpz_mul(res_den, res_den, base_den);        // res_den = res_den * base_den
        }
        mpq_set_num(result.fraction, res_num);        // 设置分子
        mpq_set_den(result.fraction, res_den);        // 设置分母

        // 释放内存
        mpz_clears(res_num, res_den, base_num, base_den, i_z, exp_z, NULL);
    } else {
        double base_d = mpq_get_d(this->fraction);        // 获取double底数
        double pow_d  = mpq_get_d(exp.fraction);          // 获取double指数
        double res_d  = pow(base_d, pow_d);               // 使用pow函数计算double结果
        if (isnan(res_d)) {
            errorFlag = RESULT_NAN;
            return result;
        }
        if (isinf(res_d)) {
            errorFlag = RESULT_INF;
            return result;
        }
        mpq_set_d(result.fraction, res_d);        // 结果转换为mpq_t
        // 修改成double类型会使计算精度降为double精度
        if (precision >= 15) warningFlag = LOST_PRECISION;
    }
    mpq_canonicalize(result.fraction);        // 约分为最简形式
    return result;
}

/**
 * @brief 重载等号
 */
bool HPReal::operator==(const HPReal &other) const {
    // 使用GMP提供的函数比较两个有理数
    return mpq_equal(fraction, other.fraction);
}

/**
 * @brief 判断分数是否为整数
 */
bool HPReal::isZ() {
    mpz_t den;                             // 定义分母
    mpz_init(den);                         // 初始化
    mpq_get_den(den, fraction);            // 获取分母
    return mpz_cmp_ui(den, 1) == 0;        // 分母等于1
}

/**
 * @brief 比较分数和整数的大小
 */
int HPReal::Compare_ui(unsigned long int z) { return mpq_cmp_ui(fraction, z, 1); }
