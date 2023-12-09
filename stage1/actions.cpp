#include "stage1.h"

void   Compiler::pushOperator(string op)
{
	operatorStk.push(op);
}
string Compiler::popOperator()
{
	if (operatorStk.empty())
		processError("compiler error; operator stack underflow");
	string temp = operatorStk.top();
	operatorStk.pop();
	return temp;
}
void   Compiler::pushOperand(string op)
{
	operandStk.push(op);
}
string Compiler::popOperand()
{
	if (operandStk.empty())
		processError("compiler error; operand stack underflow");
	string temp = operandStk.top();
	operandStk.pop();
	return temp;
}
