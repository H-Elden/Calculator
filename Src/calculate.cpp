#include "calculate.h"
#include "HPReal.h"
#include "check.h"

/**
 * @brief 运算符的优先级
 * @param c 运算符
 * @retval 优先级0-4
 */
int OpScore(char c) {        // 优先级
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    if (c == '^') return 3;
    if (c == '!') return 4;
    return 0;        //'('
}

/**
 * @brief 弹栈运算
 * @note 运算op.top()进入后缀表达式，并弹栈op
 */
void Cal() {
    char r = op.top();
    op.pop();
    if (h.empty()) {
        errorFlag = TOO_MANY_OPERATORS;
        return;
    }
    HPReal b = h.top();        // 使用拷贝构造函数构造
    h.pop();                   // 弹栈

    if (r == '!') {                                   // 阶乘只需要前面有一个数字，单独处理
        if (b.isZ() && b.Compare_ui(0) >= 0) {        // 自然数
            HPReal one("1");
            if (b == zero) {        // 特判：0! = 1
                h.push(one);
            } else if (b.Compare_ui(10000) <= 0) {        // b是正数
                HPReal res("1");
                while (!(b == zero)) {
                    res = res * b;
                    b   = b - one;
                }
                h.push(res);
            } else        // 太大了
                errorFlag = FACTORIAL_INF;
        } else        // 非自然数不能求阶乘
            errorFlag = FACTORIAL_ERROR;
        return;
    }

    if (h.empty()) {
        errorFlag = TOO_MANY_OPERATORS;
        return;
    }
    HPReal a = h.top();        // 使用拷贝构造函数构造
    h.pop();                   // 弹栈

    switch (r) {
        case '+': h.push(a + b); break;
        case '-': h.push(a - b); break;
        case '*': h.push(a * b); break;
        case '/':
            if (b == zero) {
                errorFlag = DIVISION_BY_ZERO;
                return;
            }
            h.push(a / b);
            break;
        case '^':
            if (a == zero) {
                if (b == zero) {        // 0^0无意义
                    errorFlag = POWER_ERROR;
                    return;
                }
                h.push(zero);
            }
            h.push(a.power(b));
            break;
    }
}

/**
 * @brief 处理输入的表达式
 * @param s 表达式
 */
void handleInput(const string &s) {
    if (s.empty())
        setError(INPUT_EMPTY);
    else {
        int begin = 0;
        if (s[0] == '-') {        // 处理第一个数字是负数的情况
            h.push(zero);
            op.push('-');
            begin = 1;
        }
        processExpression(s, begin);
        finalizeCalculation();
    }
    outputResult();
}

/**
 * @brief 处理过程中的表达式
 * @param s 原表达式
 * @param begin 开始处理的位
 */
void processExpression(const string &s, int begin) {
    int len = s.size();
    for (int i = begin; i < len; i++) {
        if (s[i] == ' ') continue;        // 输入的空格都将被忽略
        if (isdigit(s[i]))
            i = pushNum(i);
        else if (s[i] == 'e') {
            irrational = true;        // 存在无理数
            h.push(e);
        } else if (i + 1 < len && s[i] == 'P' && s[i + 1] == 'i') {
            irrational = true;        // 存在无理数
            h.push(Pi);
            i++;
        } else if (i + 2 < len && s[i] == 'a' && s[i + 1] == 'n' && s[i + 2] == 's') {
            h.push(ans);
            irrational  = last_irrational;
            i          += 2;
        } else if (s[i] == '(') {
            op.push('(');
            if (i < len - 1 && s[i + 1] == '-') {        // 处理负数
                h.push(zero);
                op.push('-');
                i++;
            }
        } else if (s[i] == ')')
            handleClosingParenthesis();
        else if (isOp(s[i]))
            handleOperator(s[i]);
        else
            setError(INVALID_OPERATOR);
        if (errorFlag) break;
    }
}

/***
 * @brief 处理操作符
 * @param opChar 操作符
 * @note
 */
void handleOperator(char opChar) {
    if (op.empty() || op.top() == '(' || OpScore(opChar) > OpScore(op.top())) {
        op.push(opChar);
    } else {
        while (!errorFlag && !op.empty() && isOp(op.top()) && OpScore(opChar) <= OpScore(op.top())) { Cal(); }
        if (!errorFlag) { op.push(opChar); }
    }
}

/***
 * @brief 处理右括号
 * @note
 */
void handleClosingParenthesis() {
    while (!errorFlag && !op.empty() && op.top() != '(') Cal();
    if (op.empty()) errorFlag = LOST_LEFT_PARE;        // 还没有弹栈就空栈，说明缺少'('
    if (!errorFlag) op.pop();                          // 把'('弹栈
}

/***
 * @brief 最终计算
 */
void finalizeCalculation() {
    if (errorFlag) return;
    while (!op.empty()) {
        Cal();
        if (errorFlag) return;
    }

    // 错误检查
    if (h.empty()) {        // 空输入
        setError(INPUT_EMPTY);
    } else if (h.size() == 1) {        // 正常情况
        ans = h.top();                 // 计算正确，记录历史
    } else if (h.size() == 2)
        setError(ONE_MORE_NUMBER);        // 多输入1个数字
    else
        setError(TOO_MANY_NUMBERS);        // 输入过多数字或过少运算符
}

/***
 * @brief 输出结果
 */
void outputResult() {
    switch (errorFlag) {
        case NO_ERROR:
            h.top().print();
            last_irrational = irrational;
            break;        // 输出计算结果
        case INVALID_NUMBER:     printf("ERROR: The number \"%s\" is invalid.\n", invalidNumber.c_str()); break;
        case INVALID_OPERATOR:   puts("ERROR: Please check your input format. Perhaps you have entered invalid operators."); break;
        case INPUT_EMPTY:        puts("ERROR: Input cannot be empty."); break;
        case ONE_MORE_NUMBER:    puts("ERROR: Perhaps you entered one more number or one less operator"); break;
        case TOO_MANY_NUMBERS:   printf("ERROR: Perhaps you entered %lld more numbers or %lld less operators\n", h.size() - 1, h.size() - 1); break;
        case TOO_MANY_OPERATORS: puts("ERROR: Please check your input format. Perhaps you have entered too many operators."); break;
        case DIVISION_BY_ZERO:   puts("ERROR: The divisor cannot be 0."); break;
        case FACTORIAL_ERROR:    puts("ERROR: Only natural numbers (non negative integers) can calculate factorial."); break;
        case FACTORIAL_INF:      puts("ERROR: The factorial operation is too large."); break;
        case POWER_ERROR:        puts("ERROR: The power exponent of 0 cannot be 0."); break;
        case RESULT_NAN:         puts("ERROR: The result of power operation is NaN (Not a Number)."); break;
        case RESULT_INF:         puts("ERROR: The result of power operation is too big."); break;
        case LOST_LEFT_PARE:     puts("ERROR: The expression is missing a left parenthesis"); break;
        case UNKNOWN_ERROR:
        default:                 puts("ERROR: Unknown error."); break;
    }
    // 如果没有报错才输出警告
    if (!errorFlag) {
        switch (warningFlag) {
            case NO_WARNING:     cout << endl; break;
            case LOST_PRECISION: puts("\t(Lost precision!)"); break;
            default:             break;
        }
    }
}