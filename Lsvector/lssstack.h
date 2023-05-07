#include"Lsvector.h"
#include<ctype.h>
#include<iostream>
#include<math.h>
#pragma warning(disable:4996)

#define N_OPTR 9
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator;
template <typename T>class Stack :public Vector<T> {
public: //ԭ�нӿ�һ������
	void push(T const& e) { this->insert(e); } //��ջ����Ч�ڽ���Ԫ����Ϊ������ĩԪ�ز���
	T pop() { return this->remove(this->size() - 1); } //��ջ����Ч��ɾ��������ĩԪ��
	T& top() { return (*this)[this->size() - 1]; } //ȡ����ֱ�ӷ���������ĩԪ��
};
//�������
void convert(Stack<char>& S, _int64 n, int base) {
	static char digit[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	while (n > 0) {
		S.push(digit[n % base]);
		n /= base;
	}
}
//�ݹ�Ƕ�ף�����ƥ�����⣩
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
//�ӳٻ���


//���ȼ���	
const char pri[N_OPTR][N_OPTR] =
{ //��������ȵȼ� [ջ��] [��ǰ]
   /*              |-------------------- �� ǰ �� �� �� --------------------| */
   /*              +      -      *      /      ^      !      (      )      \0 */
	/* --  + */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
	/* |   - */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',  //ֵ��һ����ǣ���������ջ��ʱ������͵����ȼ������ڽ����κγ����������
	/* ջ  * */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',//��Ϊ�ر����������ڵ�ǰʱ������ߵ����ȼ�������ջ����˭���Է��������ڽ�������ջ��
	/* ��  / */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
	/* ��  ^ */    '>',   '>',   '>',   '>',   '>',   '<',   '<',   '>',   '>',
	/* ��  ! */    '>',   '>',   '>',   '>',   '>',   '>',   ' ',   '>',   '>',//�׳˺������ȼ���ߵģ�ֻҪ������ջ����������һ���������˭���׳˺Ŷ�����������
	/* ��  ( */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   '=',   ' ',//��������һ�ɶ���С�ںţ�ջ��������ǳ����ڵ�ǰ��λ�ã�����Ҳ��С�ںţ�������ֱ����ջ
	/* |   ) */    ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',
	/* -- \0 */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   ' ',   '='//ֻҪ������������еĶ���С�ںţ�С�ں�һ�ɴ洢�������ź�/0һ��������ջ������һ��ֱ����ջ
};
//ͨ�����ȼ����ö�ٱ����ж����ȼ�
Operator optr2rank(char op)//ʹ��operatorǰ�����ȶ���
{//�ɲ�����ת������
	switch (op)
	{
	case'+': return ADD;//��  �����������Ѿ�������ADD��Ϊö�ٱ���
	case'-': return SUB;//��
	case'*': return MUL;//��
	case'/': return DIV;//��
	case'^': return POW;//�˷�
	case'!': return FAC;//�׳�
	case'(': return L_P;//������
	case')': return R_P;//������
	case'\0': return EOE;//��ʼ������ֹ��
	default: exit(-1);//λ�������
	}//��ʵ�ϣ�����Ҫ��Ϊ��C++��ν�Ŀɶ��ԡ�������ȫ���԰���Щö�ٱ��������ִ��棬����Ϊ�˿ɶ��ԣ���������һϵ�е�����������������������
}
char orderBetween(char op1, char op2)//�Ƚ�����������֮������ȼ�
{
	return pri[optr2rank(op1)][optr2rank(op2)];
}
//������ǰ������
void readNumber(char*& p, Stack<double>& stk) { //���庯��read number
	stk.push((double)(*p - '0'));
	while (isdigit(*(++p)))//ȡ��ָ��p��ֵ����������ж�λ���֣���λ������
		stk.push(stk.pop() * 10 + (*p - '0'));//�˴�*p-'0'���ַ�������ת��Ϊ���֣������������ֵ�ASCII��.pop�����ᵯ�����˵�ֵ����˿���ֱ��ȡ��������ٷ���
	if ('.' == *p) {//����Ǹ�С��
		double fraction = 1;
		while (isdigit(*(++p)))
			stk.push(stk.pop() + (*p - '0') * (fraction /= 10));
	}
}
//����ļ��㺯��
__int64 facI(int n)
{
	__int64 f = 1;
	while (n > 1)
	{
		f *= n--;
	}
	return f;
} //�׳����㣨�����棩
double calcu(double a, char op, double b)
{ //ִ�ж�Ԫ����
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
			return a / b; //ע�⣺����и�����Ϊ����ܲ���ȫ
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
{ //ִ��һԪ����
	switch (op)
	{
	case '!':
	{
		return (double)facI((int)b); //Ŀǰ���н׳ˣ����մ˷�ʽ���
	}
	default:
	{
		exit(-1);
	}
	}
}
//����ǰ������������������ӵ�RPN
void append(char*& rpn, double opnd)
{ //������������RPNĩβ
	char buf[64];
	if (0.0 < opnd - (int)opnd)
	{
		sprintf(buf, "%f \0", opnd); //�����ʽ����
	}
	else
	{
		sprintf(buf, "%d \0", (int)opnd); //������ʽ
	}
	rpn = (char*)realloc(rpn, sizeof(char) * (strlen(rpn) + strlen(buf) + 1)); //��չ�ռ�
	strcat(rpn, buf); //RPN�ӳ�
}
void append(char*& rpn, char optr)
{ //�����������RPNĩβ
	int n = strlen(rpn); //RPN��ǰ���ȣ���'\0'��β������n + 1��
	rpn = (char*)realloc(rpn, sizeof(char) * (n + 3)); //��չ�ռ�
	sprintf(rpn + n, "%c ", optr);
	rpn[n + 2] = '\0'; //����ָ���������
}
//�Ա��ʽ��ֵ����ת��Ϊ�沨�����ʽ
double evaluate(char* S)
{ //�ԣ����޳��׿ո�ģ����ʽS��ֵ����ת��Ϊ�沨��ʽRPN
	Stack<double> opnd;
	Stack<char> optr; //������ջ�������ջ /*DSA*/�κ�ʱ�̣�����ÿ������Ԫ��֮�����Сһ��
	char* expr = S;
	optr.push('\0'); //β�ڱ�'\0'Ҳ��Ϊͷ�ڱ�������ջ
	while (!optr.empty())
	{ //�������ջ�ǿ�֮ǰ�����������ʽ�и��ַ�
		if (isdigit(*S))
		{ //����ǰ�ַ�Ϊ����������
			readNumber(S, opnd);
			
		}
		else //����ǰ�ַ�Ϊ���������
		{
			switch (orderBetween(optr.top(), *S))
			{ //������ջ�������֮�����ȼ��ߵͷֱ���
			case '<': //ջ����������ȼ�����ʱ
			{
				optr.push(*S); S++; //�����Ƴ٣���ǰ�������ջ
				break;
			}
			case '=': //���ȼ���ȣ���ǰ�����Ϊ�����Ż���β���ڱ�'\0'��ʱ
			{
				optr.pop(); S++; //�����Ų�������һ���ַ�
				break;
			}
			case '>':
			{ //ջ����������ȼ�����ʱ����ʵʩ��Ӧ�ļ��㣬�������������ջ
				char op = optr.pop(); //ջ���������ջ��������RPNĩβ
				if ('!' == op)
				{ //������һԪ�����
					double pOpnd = opnd.pop(); //ֻ��ȡ��һ������������
					opnd.push(calcu(op, pOpnd)); //ʵʩһԪ���㣬�����ջ
				}
				else
				{ //������������Ԫ�������
					double pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //ȡ����ǰ������ /*DSA*/���ʣ�
					//�ɷ�ʡȥ������ʱ������
					opnd.push(calcu(pOpnd1, op, pOpnd2)); //ʵʩ��Ԫ���㣬�����ջ
				}
				break;
			}
			default:
			{
				exit(-1); //���﷨���󣬲�������ֱ���˳�
			}
			}

		}
	}
	return opnd.pop(); //�������������ļ�����
}
//�沨�����ʽ��ֵ�㷨
double rpnEvaluation(char* expr) {//��RPN���ʽ����ֵ
	Stack<double> Stk;//��Ų�������ջ
	while ((*expr) != '\0') {//expr��δɨ����ϣ��������ڽ������ţ�����Ľ���������'\0'
		if ((*expr) == ' ') {//����ķָ������ǿո�
			expr++;//�����ָ�����
			continue;
		}
		if (isdigit(*expr))//
		{ //����ǰ�ַ�Ϊ����������
			readNumber(expr, Stk);//����������ջ
		}
		else //����ǰ�ַ�Ϊ���������
		{
			char op = (*expr);//���㵱ǰ������
			expr++;//�������֮�������ʽ
			if ('!' == op) {//�����һԪ����
				expr++;//�������֮�������ʽ
				double number = Stk.top();
				Stk.pop();
				Stk.push(calcu(op, number)); //ʵʩһԪ���㣬�����ջ
			}
			else {
				double pOpnd2 = Stk.top();
				Stk.pop();
				double pOpnd1 = Stk.top();
				Stk.pop();
				Stk.push(calcu(pOpnd1, op, pOpnd2)); //ʵʩ��Ԫ���㣬�����ջ
			}

		}
	}
	return Stk.top();
}

