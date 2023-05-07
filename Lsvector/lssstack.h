#include"Lsvector.h"
#include<ctype.h>
#include<iostream>
#include<math.h>
#pragma warning(disable:4996)

#define N_OPTR 9
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator;
template <typename T>class Stack :public Vector<T> {
public: //原有接口一概沿用
	void push(T const& e) { this->insert(e); } //入栈：等效于将新元素作为向量的末元素插入
	T pop() { return this->remove(this->size() - 1); } //出栈：等效于删除向量的末元素
	T& top() { return (*this)[this->size() - 1]; } //取顶：直接返回向量的末元素
};
//逆序输出
void convert(Stack<char>& S, _int64 n, int base) {
	static char digit[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	while (n > 0) {
		S.push(digit[n % base]);
		n /= base;
	}
}
//递归嵌套（括号匹配问题）
bool paren(const char exp[], int lo,int hi) {
	Stack<char>S;
	for (int i = lo; i < hi; i++) 
		switch (exp[i])
		{
		case'(':case'[':case'{' : S.push(exp[i]); break;
		case')':if ((S.empty()) || ('(' != S.pop()))return false; break;
		case']':if ((S.empty()) || ('[' != S.pop()))return false; break;
		case'}':if ((S.empty()) || ('{' != S.pop()))return false; break;
		default:
			break;
		}
	return S.empty();
}
//延迟缓冲


//优先级表	
const char pri[N_OPTR][N_OPTR] =
{ //运算符优先等级 [栈顶] [当前]
   /*              |-------------------- 当 前 运 算 符 --------------------| */
   /*              +      -      *      /      ^      !      (      )      \0 */
	/* --  + */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
	/* |   - */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',  //值得一提的是，左括号在栈顶时最有最低的优先级（乐于接纳任何常规运算符）
	/* 栈  * */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',//作为回报，左括号在当前时具有最高的优先级（无论栈顶是谁，对方都会乐于接纳它入栈）
	/* 顶  / */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
	/* 运  ^ */    '>',   '>',   '>',   '>',   '>',   '<',   '<',   '>',   '>',
	/* 算  ! */    '>',   '>',   '>',   '>',   '>',   '>',   ' ',   '>',   '>',//阶乘号是优先级最高的，只要出现在栈顶，无论下一个运算符是谁，阶乘号都会优先运算
	/* 符  ( */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   '=',   ' ',//（左括号一律都是小于号）栈顶！如果是出现在当前的位置，所有也是小于号，左括号直接入栈
	/* |   ) */    ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',
	/* -- \0 */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   ' ',   '='//只要是运算符，所有的都是小于号（小于号一律存储）左括号和/0一旦出现在栈顶，下一项直接入栈
};
//通过优先级表和枚举变量判断优先级
Operator optr2rank(char op)//使用operator前必须先定义
{//由操作符转译出编号
	switch (op)
	{
	case'+': return ADD;//加  在上面我们已经给出了ADD作为枚举变量
	case'-': return SUB;//减
	case'*': return MUL;//乘
	case'/': return DIV;//除
	case'^': return POW;//乘方
	case'!': return FAC;//阶乘
	case'(': return L_P;//左括号
	case')': return R_P;//右括号
	case'\0': return EOE;//起始符与终止符
	default: exit(-1);//位置运算符
	}//事实上，这主要是为了C++所谓的可读性。我们完全可以把这些枚举变量用数字代替，但是为了可读性，我们做了一系列调整。包括上面的重载运算符
}
char orderBetween(char op1, char op2)//比较两个操作符之间的优先级
{
	return pri[optr2rank(op1)][optr2rank(op2)];
}
//解析当前操作数
void readNumber(char*& p, Stack<double>& stk) { //定义函数read number
	stk.push((double)(*p - '0'));
	while (isdigit(*(++p)))//取出指针p的值，如果后续有多位数字（多位整数）
		stk.push(stk.pop() * 10 + (*p - '0'));//此处*p-'0'将字符型数字转化为数字，否则会输出数字的ASCII码.pop函数会弹出顶端的值，因此可以直接取出后操作再放入
	if ('.' == *p) {//如果是个小数
		double fraction = 1;
		while (isdigit(*(++p)))
			stk.push(stk.pop() + (*p - '0') * (fraction /= 10));
	}
}
//具体的计算函数
__int64 facI(int n)
{
	__int64 f = 1;
	while (n > 1)
	{
		f *= n--;
	}
	return f;
} //阶乘运算（迭代版）
double calcu(double a, char op, double b)
{ //执行二元运算
	switch (op)
	{
	case '+':
	{
		return a + b;
	}
	case '-':
	{
		return a - b;
	}
	case '*':
	{
		return a * b;
	}
	case '/':
	{
		if (0 == b)
		{
			exit(-1);
		}
		else
		{
			return a / b; //注意：如此判浮点数为零可能不安全
		}
	}
	case '^':
	{
		return pow(a, b);
	}
	default:
	{
		exit(-1);
	}
	}
}
double calcu(char op, double b)
{ //执行一元运算
	switch (op)
	{
	case '!':
	{
		return (double)facI((int)b); //目前仅有阶乘，可照此方式添加
	}
	default:
	{
		exit(-1);
	}
	}
}
//将当前操作数或操作符号连接到RPN
void append(char*& rpn, double opnd)
{ //将操作数接至RPN末尾
	char buf[64];
	if (0.0 < opnd - (int)opnd)
	{
		sprintf(buf, "%f \0", opnd); //浮点格式，或
	}
	else
	{
		sprintf(buf, "%d \0", (int)opnd); //整数格式
	}
	rpn = (char*)realloc(rpn, sizeof(char) * (strlen(rpn) + strlen(buf) + 1)); //扩展空间
	strcat(rpn, buf); //RPN加长
}
void append(char*& rpn, char optr)
{ //将运算符接至RPN末尾
	int n = strlen(rpn); //RPN当前长度（以'\0'结尾，长度n + 1）
	rpn = (char*)realloc(rpn, sizeof(char) * (n + 3)); //扩展空间
	sprintf(rpn + n, "%c ", optr);
	rpn[n + 2] = '\0'; //接入指定的运算符
}
//对表达式求值，并转化为逆波兰表达式
double evaluate(char* S)
{ //对（已剔除白空格的）表达式S求值，并转换为逆波兰式RPN
	Stack<double> opnd;
	Stack<char> optr; //运算数栈、运算符栈 /*DSA*/任何时刻，其中每对相邻元素之间均大小一致
	char* expr = S;
	optr.push('\0'); //尾哨兵'\0'也作为头哨兵首先入栈
	while (!optr.empty())
	{ //在运算符栈非空之前，逐个处理表达式中各字符
		if (isdigit(*S))
		{ //若当前字符为操作数，则
			readNumber(S, opnd);
			
		}
		else //若当前字符为运算符，则
		{
			switch (orderBetween(optr.top(), *S))
			{ //视其与栈顶运算符之间优先级高低分别处理
			case '<': //栈顶运算符优先级更低时
			{
				optr.push(*S); S++; //计算推迟，当前运算符进栈
				break;
			}
			case '=': //优先级相等（当前运算符为右括号或者尾部哨兵'\0'）时
			{
				optr.pop(); S++; //脱括号并接收下一个字符
				break;
			}
			case '>':
			{ //栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入栈
				char op = optr.pop(); //栈顶运算符出栈并续接至RPN末尾
				if ('!' == op)
				{ //若属于一元运算符
					double pOpnd = opnd.pop(); //只需取出一个操作数，并
					opnd.push(calcu(op, pOpnd)); //实施一元计算，结果入栈
				}
				else
				{ //对于其它（二元）运算符
					double pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //取出后、前操作数 /*DSA*/提问：
					//可否省去两个临时变量？
					opnd.push(calcu(pOpnd1, op, pOpnd2)); //实施二元计算，结果入栈
				}
				break;
			}
			default:
			{
				exit(-1); //逢语法错误，不做处理直接退出
			}
			}

		}
	}
	return opnd.pop(); //弹出并返回最后的计算结果
}
//逆波兰表达式求值算法
double rpnEvaluation(char* expr) {//对RPN表达式的求值
	Stack<double> Stk;//存放操作数的栈
	while ((*expr) != '\0') {//expr尚未扫描完毕，即不等于结束符号，这里的结束符号是'\0'
		if ((*expr) == ' ') {//这里的分隔符号是空格
			expr++;//跳过分隔符号
			continue;
		}
		if (isdigit(*expr))//
		{ //若当前字符为操作数，则
			readNumber(expr, Stk);//将操作数入栈
		}
		else //若当前字符为运算符，则
		{
			char op = (*expr);//计算当前操作符
			expr++;//运算完成之后计算表达式
			if ('!' == op) {//如果是一元运算
				expr++;//运算完成之后计算表达式
				double number = Stk.top();
				Stk.pop();
				Stk.push(calcu(op, number)); //实施一元计算，结果入栈
			}
			else {
				double pOpnd2 = Stk.top();
				Stk.pop();
				double pOpnd1 = Stk.top();
				Stk.pop();
				Stk.push(calcu(pOpnd1, op, pOpnd2)); //实施二元计算，结果入栈
			}

		}
	}
	return Stk.top();
}

