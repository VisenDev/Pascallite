#include "stage1.h"

//void Compiler::emit(string label, string instruction, string operands,
//         string comment){}
//void Compiler::emitPrologue(string progName, string){}
//void Compiler::emitEpilogue(string, string){}
//void Compiler::emitStorage(){}
void Compiler::emitReadCode(string operand, string){}
void Compiler::emitWriteCode(string operand, string){}
void Compiler::emitAssignCode(string operand1, string operand2){}         // op2 = op1
void Compiler::emitAdditionCode(string operand1, string operand2){}       // op2 +  op1
void Compiler::emitSubtractionCode(string operand1, string operand2){}    // op2 -  op1
void Compiler::emitMultiplicationCode(string operand1, string operand2){} // op2 *  op1
void Compiler::emitDivisionCode(string operand1, string operand2){}       // op2 /  op1
void Compiler::emitModuloCode(string operand1, string operand2){}         // op2 %  op1
void Compiler::emitNegationCode(string operand1, string){}           // -op1
void Compiler::emitNotCode(string operand1, string){}                // !op1
void Compiler::emitAndCode(string operand1, string operand2){}            // op2 && op1
void Compiler::emitOrCode(string operand1, string operand2){}             // op2 || op1
void Compiler::emitEqualityCode(string operand1, string operand2){}       // op2 == op1
void Compiler::emitInequalityCode(string operand1, string operand2){}     // op2 != op1
void Compiler::emitLessThanCode(string operand1, string operand2){}       // op2 <  op1
void Compiler::emitLessThanOrEqualToCode(string operand1, string operand2){} // op2 <= op1
void Compiler::emitGreaterThanCode(string operand1, string operand2){}    // op2 >  op1
void Compiler::emitGreaterThanOrEqualToCode(string operand1, string operand2){} // op2 >= op1
