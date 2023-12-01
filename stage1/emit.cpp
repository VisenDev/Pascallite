#include <stage1.h>

void emit(string label = "", string instruction = "", string operands = "",
          string comment = "");
void emitPrologue(string progName, string = "");
void emitEpilogue(string = "", string = "");
void emitStorage();
void emitReadCode(string operand, string = "");
void emitWriteCode(string operand, string = "");
void emitAssignCode(string operand1, string operand2);         // op2 = op1
void emitAdditionCode(string operand1, string operand2);       // op2 +  op1
void emitSubtractionCode(string operand1, string operand2);    // op2 -  op1
void emitMultiplicationCode(string operand1, string operand2); // op2 *  op1
void emitDivisionCode(string operand1, string operand2);       // op2 /  op1
void emitModuloCode(string operand1, string operand2);         // op2 %  op1
void emitNegationCode(string operand1, string = "");           // -op1
void emitNotCode(string operand1, string = "");                // !op1
void emitAndCode(string operand1, string operand2);            // op2 && op1
void emitOrCode(string operand1, string operand2);             // op2 || op1
void emitEqualityCode(string operand1, string operand2);       // op2 == op1
void emitInequalityCode(string operand1, string operand2);     // op2 != op1
void emitLessThanCode(string operand1, string operand2);       // op2 <  op1
void emitLessThanOrEqualToCode(string operand1, string operand2); // op2 <= op1
void emitGreaterThanCode(string operand1, string operand2);    // op2 >  op1
void emitGreaterThanOrEqualToCode(string operand1, string operand2); // op2 >= op1
