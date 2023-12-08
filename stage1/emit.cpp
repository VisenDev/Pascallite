#include "stage1.h"

//void Compiler::emit(string label, string instruction, string operands,
//         string comment){}
//void Compiler::emitPrologue(string progName, string){}
//void Compiler::emitEpilogue(string, string){}
//void Compiler::emitStorage(){}
void Compiler::emitReadCode(string operand, string)
{
	auto itr = symbolTable.find(operand);
	if (itr == symbolTable.end())
		processError("Symbol "+operand+" is undefined");
	emit("", "call", "ReadInt", "read int; value placed in eax");
	
	if (itr->second.getDataType() != INTEGER)
		processError("can't read variables of this type");
	
	emit("", "mov", "["+itr->second.getInternalName()+"],eax", "store eax at "+operand);
	
}
void Compiler::emitWriteCode(string operand, string){}
void Compiler::emitAssignCode(string operand1, string operand2){}         // op2 = op1
                                                                          // 
void Compiler::emitAdditionCode(string operand1, string operand2){
   if(!isInteger(operand1) or !isInteger(operand2)) {
      processError("illegal type, expected integer");
   }
   if(isTemporary(contentsOfAReg) and contentsOfAReg != operand1 and contentsOfAReg != operand2) {
      // emit code to store that contentsOfAReg into memory
      // change the allocate entry for the temp in the symbol table to yes
      // deassign it
   } else if (isTemporary(contentsOfAReg)) {
      // if the A register holds a non-temp not operand1 nor operand2 then deassign it
      contentsOfAReg = "";
   } else if(contentsOfAReg != operand1 and contentsOfAReg != operand2) {
      // emit code to load operand2 into the A register
      // emit code to perform register-memory addition
   }
   //TODO
   // deassign all temporaries involved in the addition and free those names for reuse
   // A Register = next available temporary name an

}       // op2 +  op1

void Compiler::emitSubtractionCode(string operand1, string operand2){
   if(!isInteger(operand1) or !isInteger(operand2)) {
      processError("illegal type, expected integer");
   }
   if(isTemporary(contentsOfAReg) and contentsOfAReg != operand1 and contentsOfAReg != operand2) {
      // emit code to store that contentsOfAReg into memory
      // change the allocate entry for the temp in the symbol table to yes
      // deassign it
   } else if (isTemporary(contentsOfAReg)) {
      // if the A register holds a non-temp not operand1 nor operand2 then deassign it
      contentsOfAReg = "";
   } else if(contentsOfAReg != operand1 and contentsOfAReg != operand2) {
      // emit code to load operand2 into the A register
      // emit code to perform register-memory subtraction 
   }
   //TODO
   // deassign all temporaries involved in the subtraction and free those names for reuse
   // A Register = next available temporary name an


}    // op2 -  op1
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
