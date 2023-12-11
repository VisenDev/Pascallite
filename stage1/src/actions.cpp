#include "stage1.h"

void Compiler::pushOperator(string op) {
   operatorStk.push(op);
}

string Compiler::popOperator() {
   if (operatorStk.empty()) {
      processError("compiler error; operator stack underflow");
   }
   string temp = operatorStk.top();
   operatorStk.pop();
   return temp;
}

void Compiler::pushOperand(string op) {
	if (isInteger(op) and (symbolTable.find(op) == symbolTable.end())) {
      //cout << "inserting " << op << " in pushOperand" << endl;
		insert(op, whichType(op), CONSTANT, op, YES, 1);
	}
	if (isBoolean(op) and (symbolTable.find(op) == symbolTable.end()))
	{
		if (op == "true")
      		symbolTable.insert(pair<string, SymbolTableEntry>(op, SymbolTableEntry("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1)));
		else if (op == "false")
	      	symbolTable.insert(pair<string, SymbolTableEntry>(op, SymbolTableEntry("FALSE", BOOLEAN, CONSTANT, "0", YES, 1)));

	}
   operandStk.push(op);
}

string Compiler::popOperand() {
   if (operandStk.empty()){
      processError("compiler error; operand stack underflow");
   }
   string temp = operandStk.top();
   operandStk.pop();
   return temp;
}
