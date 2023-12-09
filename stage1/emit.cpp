#include "stage1.h"

#define EXISTS(operand) \
   (symbolTable.find(operand) != symbolTable.end())
#define DATATYPE(operand) \
   (symbolTable.find(operand)->second.getDataType())
#define NAME(operand) \
   (symbolTable.find(operand)->second.getInternalName())

//void Compiler::emit(string label, string instruction, string operands,
//         string comment){}
//void Compiler::emitPrologue(string progName, string){}
//void Compiler::emitEpilogue(string, string){}
//void Compiler::emitStorage(){}
void Compiler::emitReadCode(string operand, string)
{
	string name = "";
   while (operand != "")
   {
      name.clear();
      while ((operand != "") && (operand.front() != ','))
      {
         name += operand.front();
         operand.erase(0,1);
      }
      if ((operand != "") && (operand.front() == ','))
         operand.erase(0,1);
	  
	auto itr = symbolTable.find(name);
	if (itr == symbolTable.end())
		processError("Symbol "+name+" is undefined");
	if (itr->second.getDataType() != INTEGER)
		processError("can't read variables of this type");
	if (itr->second.getMode() != VARIABLE)
		processError("attempting to read to a read-only location");
	   
	emit("", "call", "ReadInt", "; read int; value placed in eax");
	emit("", "mov", "["+itr->second.getInternalName()+"],eax", "; store eax at "+name);
   }
	
}
void Compiler::emitWriteCode(string operand, string)
{
	string name = "";
   while (operand != "")
   {
      name.clear();
      while ((operand != "") && (operand.front() != ','))
      {
         name += operand.front();
         operand.erase(0,1);
      }
      if ((operand != "") && (operand.front() == ','))
         operand.erase(0,1);
	  
	auto itr = symbolTable.find(name);
	if (itr == symbolTable.end())
		processError("Symbol "+name+" is undefined");
	if (contentsOfAReg != name)
	{
		emit("", "mov", "eax, ["+itr->second.getInternalName()+"]", "; load "+name+" into eax");
		contentsOfAReg = name;
	}
	if ((itr->second.getDataType() == INTEGER) or (itr->second.getDataType() == BOOLEAN))
		emit("", "call", "WriteInt",  "; write int in eax to standard out");
	emit("", "call", "Crlf", "; write \\r\\n to standard out");
   }
}

void Compiler::emitAssignCode(string operand1, string operand2)         // op2 = op1
{
	if (symbolTable.find(operand1)->second.getDataType() != symbolTable.find(operand2)->second.getDataType())
		processError("operand types must match");
	if (symbolTable.find(operand2)->second.getMode()!= VARIABLE)
		processError("symbol on left-hand side of assignment must have a storage mode of VARIABLE");
	if (operand1 == operand2)
		return;
	if (operand1 != contentsOfAReg)
		emit("", "mov", "eax, ["+symbolTable.find(operand1)->second.getInternalName()+"]", "; AReg = "+operand1);
	emit("", "mov", "["+symbolTable.find(operand2)->second.getInternalName()+"], eax", "; operand2 = AReg");
	contentsOfAReg = operand2;
	if (isTemporary(operand1))
		freeTemp();
}

void Compiler::emitAdditionCode(string operand1, string operand2){
	if ((symbolTable.find(operand1) == symbolTable.end()) or (symbolTable.find(operand2) == symbolTable.end()))
		processError("undefined operands");
   if((symbolTable.find(operand1)->second.getDataType() != INTEGER) or (symbolTable.find(operand2)->second.getDataType() != INTEGER)) {
      processError("illegal type, expected integer");
   }
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"], eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and contentsOfAReg != operand1 and contentsOfAReg != operand2) {
      contentsOfAReg = "";
   }  if(contentsOfAReg != operand1 and contentsOfAReg != operand2) {
      	emit("", "mov", "eax, ["+symbolTable.find(operand2)->second.getInternalName()+"]", "; put "+operand2+" into eax");
	   contentsOfAReg = operand2;
   }
	if (contentsOfAReg == operand2)
		emit("", "add", "eax, ["+symbolTable.find(operand1)->second.getInternalName()+"]", "; AReg = " + operand2+" + "+operand1);
	else if (contentsOfAReg == operand1)
		emit("", "add", "eax, ["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = " + operand1+" + "+operand2);
	else
		cout << "ERROR IN EMIT ADDITION LOGIC" << endl; //remove this at some point
	
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(INTEGER);
	pushOperand(contentsOfAReg);
}       // op2 +  op1

void Compiler::emitSubtractionCode(string operand1, string operand2){
	if ((symbolTable.find(operand1) == symbolTable.end()) or (symbolTable.find(operand2) == symbolTable.end()))
		processError("undefined operands");
   if((symbolTable.find(operand1)->second.getDataType() != INTEGER) or (symbolTable.find(operand2)->second.getDataType() != INTEGER)) {
      processError("illegal type, expected integer");
   }
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"], eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand2)) {
      contentsOfAReg = "";
   }  if(contentsOfAReg != operand2) {
      	emit("", "mov", "eax, ["+symbolTable.find(operand2)->second.getInternalName()+"]", "; put "+operand2+" into eax");
	   contentsOfAReg = operand2;
   }
	if (contentsOfAReg == operand2)
		emit("", "sub", "eax, ["+symbolTable.find(operand1)->second.getInternalName()+"]", "; AReg = " + operand2+" + "+operand1);
	else
		cout << "LOGIC ERROR IN SUBTRACTION" << endl;  //remove this later
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(INTEGER);
	pushOperand(contentsOfAReg);
}    // op2 -  op1
void Compiler::emitMultiplicationCode(string operand1, string operand2) // op2 *  op1
{
	if ((symbolTable.find(operand1) == symbolTable.end()) or (symbolTable.find(operand2) == symbolTable.end()))
		processError("undefined operands");
	if((symbolTable.find(operand1)->second.getDataType() != INTEGER) or (symbolTable.find(operand2)->second.getDataType() != INTEGER)) {
      processError("illegal type, expected integer");
   }
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"], eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      contentsOfAReg = "";
   }  if((contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      	emit("", "mov", "eax, ["+symbolTable.find(operand2)->second.getInternalName()+"]", "; put "+operand2+" into eax");
	   contentsOfAReg = operand2;
   }
	if (contentsOfAReg == operand2)
		emit("", "imul", "dword ["+symbolTable.find(operand1)->second.getInternalName()+"]", "; AReg = " + operand2+" * "+operand1);
	else if (contentsOfAReg == operand1)
		emit("", "imul", "dword ["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = " + operand1+" * "+operand2);
	else
		cout << "ERROR IN EMIT ADDITION LOGIC" << endl; //remove this at some point
	
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(INTEGER);
	pushOperand(contentsOfAReg);
}
void Compiler::emitDivisionCode(string operand1, string operand2){}       // op2 /  op1
void Compiler::emitModuloCode(string operand1, string operand2){}         // op2 %  op1
void Compiler::emitNegationCode(string operand1, string)           // -op1
{
	if (symbolTable.find(operand1)->second.getDataType() != INTEGER)
		processError("illegal argument");
	if (isTemporary(contentsOfAReg) and (contentsOfAReg != operand1))
	{
		emit("", "mov", "["+contentsOfAReg+"], eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
	}
	if (contentsOfAReg != operand1)
	{
		emit("", "mov", "eax, ["+symbolTable.find(operand1)->second.getInternalName()+"]", "; place "+operand1+" into eax");
	}
	emit ("", "neg", "eax", "; AReg = -AReg");
}
void Compiler::emitNotCode(string operand1, string){}                // !op1
void Compiler::emitAndCode(string operand1, string operand2){}            // op2 && op1
void Compiler::emitOrCode(string operand1, string operand2){}             // op2 || op1
                                                                          
void Compiler::emitEqualityCode(string operand1, string operand2){
	if (!EXISTS(operand1) or !EXISTS(operand2)){
		processError("undefined operands");
   } else if(DATATYPE(operand1) != INTEGER or (DATATYPE(operand2) != INTEGER)) { //TODO do these need to be integer
		processError("operands to = much be INTEGER");
   }

   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"], eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   }
   
}       // op2 == op1
void Compiler::emitInequalityCode(string operand1, string operand2){}     // op2 != op1
void Compiler::emitLessThanCode(string operand1, string operand2){}       // op2 <  op1
void Compiler::emitLessThanOrEqualToCode(string operand1, string operand2){} // op2 <= op1
void Compiler::emitGreaterThanCode(string operand1, string operand2){}    // op2 >  op1
void Compiler::emitGreaterThanOrEqualToCode(string operand1, string operand2){} // op2 >= op1
