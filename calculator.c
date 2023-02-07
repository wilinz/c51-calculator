#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "calculator.h"

char *p;

double parse_expr();
double parse_term();
double parse_factor();
double parse_number();

/*
 * parse_expr - 解析表达式
 *
 * 表达式的语法规则是：
 * 表达式 ::= 项 { '+' 项 | '-' 项 }
 */
double parse_expr()
{
    double result = parse_term();
    while (*p == '+' || *p == '-')
    {
        char op = *p++;
        double term = parse_term();
        if (op == '+')
        {
            result += term;
        }
        else
        {
            result -= term;
        }
    }
    return result;
}

/*
 * parse_term - 解析项
 *
 * 项的语法规则是：
 * 项 ::= 因子 { '*' 因子 | '/' 因子 }
 */
double parse_term()
{
    double result = parse_factor();
    while (*p == 'x' || *p == '/')
    {
        char op = *p++;
        double factor = parse_factor();
        if (op == 'x')
        {
            result *= factor;
        }
        else
        {
            result /= factor;
        }
    }
    return result;
}

/*
 * parse_factor - 解析因子
 *
 * 因子的语法规则是：
 * 因子 ::= 数字 | '(' 表达式 ')' | '+' 因子 | '-' 因子 | 函数 因子
 * 函数 ::= "sin" | "cos" | "tan" | "sqrt" | "log" | "exp"
 */
double parse_factor()
{
    double result = 0;
    double factor;
    if (*p == '(')
    {
        p++;
        result = parse_expr();
        p++;
    }
    else if (isdigit(*p) || *p == '.')
    {
        result = parse_number();
    }
    else if (*p == '+' || *p == '-')
    {
        char op = *p++;
        result = parse_factor();
        result = (op == '+') ? result : -result;
    }
    else if (isalpha(*p))
    {
        char func[10];
        int i = 0;
        while (isalpha(*p))
        {
            func[i++] = *p++;
        }
        func[i] = '\0';
        factor = parse_factor();
        if (strcmp(func, "sin") == 0)
        {
            result = sin(factor);
        }
        else if (strcmp(func, "cos") == 0)
        {
            result = cos(factor);
        }
        else if (strcmp(func, "tan") == 0)
        {
            result = tan(factor);
        }
        else if (strcmp(func, "sqrt") == 0)
        {
            result = sqrt(factor);
        }
        else if (strcmp(func, "ln") == 0)
        {
            result = log(factor);
        }
        else if (strcmp(func, "exp") == 0)
        {
            result = exp(factor);
        }
    }
    while (*p == '^')
    {
        double factor;
        p++;
        factor = parse_factor();
        result = pow(result, factor);
    }
    return result;
}

/*
 * parse_number - 解析数字
 *
 * 数字的语法规则是：
 * 数字 ::= 整数 { '.' [整数] }
 */
double parse_number()
{
    double result = 0;
    double fraction;
    while (isdigit(*p))
    {
        result = result * 10 + *p++ - '0';
    }
    if (*p == '.')
    {
        p++;
        fraction = 1;
        while (isdigit(*p))
        {
            fraction /= 10;
            result += (*p++ - '0') * fraction;
        }
    }
    return result;
}

double calculator(char *express)
{
    p = express;
    return parse_expr();
}