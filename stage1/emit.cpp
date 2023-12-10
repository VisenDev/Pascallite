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
	contentsOfAReg = name;
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
		emit("", "mov", "eax,["+itr->second.getInternalName()+"]", "; load "+name+" in eax");
		contentsOfAReg = name;
	}
	if ((itr->second.getDataType() == INTEGER) or (itr->second.getDataType() == BOOLEAN))
		emit("", "call", "WriteInt",  "; write int in eax to standard out");
	emit("", "call", "Crlf", "; write \\r\\n to standard out");
   }
}

void Compiler::emitAssignCode(string operand1, string operand2)         // op2 = op1
{
   
	if (symbolTable.find(operand1) == symbolTable.end() or  symbolTable.find(operand2) == symbolTable.end()) {
      processError("Internal compiler error: emitAssignCode called with invalid arguments");
   } else
	if (symbolTable.find(operand1)->second.getDataType() != symbolTable.find(operand2)->second.getDataType())
		processError("operand types must match, " + operand1 + " is of type " 
            + (DATATYPE(operand1) == INTEGER ? "INTEGER" : DATATYPE(operand1) == BOOLEAN ? "BOOLEAN" : "UNKNOWN")
            + " " + operand2 + " is of type "
            + (DATATYPE(operand2) == INTEGER ? "INTEGER" : DATATYPE(operand2) == BOOLEAN ? "BOOLEAN" : "UNKNOWN")
      );
	if (symbolTable.find(operand2)->second.getMode()!= VARIABLE)
		processError("symbol on left-hand side of assignment must have a storage mode of VARIABLE");
	if (operand1 == operand2)
		return;
	if (operand1 != contentsOfAReg)
		emit("", "mov", "eax,["+symbolTable.find(operand1)->second.getInternalName()+"]", "; AReg = "+operand1);
	emit("", "mov", "["+symbolTable.find(operand2)->second.getInternalName()+"],eax", "; "+operand2+" = AReg");
	contentsOfAReg = operand2;
	if (isTemporary(operand1))
		freeTemp();
}

void Compiler::emitAdditionCode(string operand1, string operand2){
	if ((symbolTable.find(operand1) == symbolTable.end()) or (symbolTable.find(operand2) == symbolTable.end()))
		processError("undefined operands");
   if((symbolTable.find(operand1)->second.getDataType() != INTEGER) or (symbolTable.find(operand2)->second.getDataType() != INTEGER)) {
      processError("binary \'+\' requires integer operands");
   }
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and contentsOfAReg != operand1 and contentsOfAReg != operand2) {
      contentsOfAReg = "";
   }  if(contentsOfAReg != operand1 and contentsOfAReg != operand2) {
      	emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
	   contentsOfAReg = operand2;
   }
	if (contentsOfAReg == operand2)
		emit("", "add", "eax,["+symbolTable.find(operand1)->second.getInternalName()+"]", "; AReg = " + operand2+" + "+operand1);
	else if (contentsOfAReg == operand1)
		emit("", "add", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = " + operand1+" + "+operand2);
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
      emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand2)) {
      contentsOfAReg = "";
   }  if(contentsOfAReg != operand2) {
      	emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
	   contentsOfAReg = operand2;
   }
	if (contentsOfAReg == operand2)
		emit("", "sub", "eax,["+symbolTable.find(operand1)->second.getInternalName()+"]", "; AReg = " + operand2+" - "+operand1);
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
      emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      contentsOfAReg = "";
   }  if((contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      	emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
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
void Compiler::emitDivisionCode(string operand1, string operand2)      // op2 /  op1
{
	if (!EXISTS(operand1) or !EXISTS(operand2))
		processError("undefined arguments");
	if ((DATATYPE(operand1) != INTEGER) or (DATATYPE(operand2) != INTEGER))
		processError("illegal types, needs integer arguments");
	if (isTemporary(contentsOfAReg) and (contentsOfAReg != operand2))
	{
		emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
	}
	else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand2))
	{
		contentsOfAReg = "";
	}
	if (contentsOfAReg != operand2)
	{
		emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
	   contentsOfAReg = operand2;
	}
	if (contentsOfAReg == operand2)
	{
		emit("", "cdq", "", "; sign extend dividend from eax to edx:eax");
		emit("", "idiv", "dword ["+NAME(operand1)+"]", "; AReg = " +operand2+" div "+operand1);
	}
	
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(INTEGER);
	pushOperand(contentsOfAReg);	
}
void Compiler::emitModuloCode(string operand1, string operand2)         // op2 %  op1
{
	if (!EXISTS(operand1) or !EXISTS(operand2))
		processError("undefined arguments");
	if ((DATATYPE(operand1) != INTEGER) or (DATATYPE(operand2) != INTEGER))
		processError("illegal types, needs integer arguments");
	if (isTemporary(contentsOfAReg) and (contentsOfAReg != operand2))
	{
		emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
	}
	else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand2))
	{
		contentsOfAReg = "";
	}
	if (contentsOfAReg != operand2)
	{
		emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
	   contentsOfAReg = operand2;
	}
	if (contentsOfAReg == operand2)
	{
		emit("", "cdq", "", "; sign extend dividend from eax to edx:eax");
		emit("", "idiv", "dword ["+NAME(operand1)+"]", "; AReg = " +operand2+" div "+operand1);
		emit("", "xchg", "eax,edx", "; exchange quotient and remainder");
	}
	
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(INTEGER);
	pushOperand(contentsOfAReg);
}
void Compiler::emitNegationCode(string operand1, string)           // -op1
{
	if (!EXISTS(operand1))
		processError("undefined arguments");
	if (symbolTable.find(operand1)->second.getDataType() != INTEGER)
		processError("illegal argument");
	if (isTemporary(contentsOfAReg) and (contentsOfAReg != operand1))
	{
		emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
	}
	if (contentsOfAReg != operand1)
	{
		emit("", "mov", "eax,["+symbolTable.find(operand1)->second.getInternalName()+"]", "; AReg = "+operand1);
	}
	emit ("", "neg", "eax", "; AReg = -AReg");
	if (isTemporary(operand1))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(INTEGER);
	pushOperand(contentsOfAReg);
}
void Compiler::emitNotCode(string operand1, string)                // !op1
{
	if (!EXISTS(operand1))
		processError("undefined operand");
	if ((DATATYPE(operand1) != BOOLEAN))
      processError("illegal type, expected boolean");
   
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand1)) {
      emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand1)) {
      contentsOfAReg = "";
   }  if((contentsOfAReg != operand1)) {
      	emit("", "mov", "eax,["+symbolTable.find(operand1)->second.getInternalName()+"]", "; AReg = "+operand1);
	   contentsOfAReg = operand1;
   }
	if (contentsOfAReg == operand1)
	{
		emit("", "not", "eax", "; AReg = !AReg");
	}
	else
		processError("compiler error: emit not logic");
	
	if (isTemporary(operand1))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(BOOLEAN);
	pushOperand(contentsOfAReg);
}
void Compiler::emitAndCode(string operand1, string operand2)            // op2 && op1
{
	if (!EXISTS(operand1) or !EXISTS(operand2))
		processError("undefined operands");
	if ((DATATYPE(operand1) != BOOLEAN) or (DATATYPE(operand2) != BOOLEAN))
      processError("binary \'and\' requires boolean operands");
   
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      contentsOfAReg = "";
   }  if((contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      	emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
	   contentsOfAReg = operand2;
   }
	if(contentsOfAReg == operand2)
		emit("", "and", "eax,["+NAME(operand1)+"]", "; AReg = "+operand2+" and "+operand1);
	else if(contentsOfAReg == operand1)
		emit("", "and", "eax,["+NAME(operand2)+"]", "; AReg = "+operand1+" and "+operand2);
	
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(BOOLEAN);
	pushOperand(contentsOfAReg);
}
void Compiler::emitOrCode(string operand1, string operand2)            // op2 || op1
{
	if (!EXISTS(operand1) or !EXISTS(operand2))
		processError("undefined operands");
	if ((DATATYPE(operand1) != BOOLEAN) or (DATATYPE(operand2) != BOOLEAN))
      processError("illegal type, expected boolean");
  
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      contentsOfAReg = "";
   }  if((contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      	emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
	   contentsOfAReg = operand2;
   }
	if(contentsOfAReg == operand2)
		emit("", "or", "eax,["+NAME(operand1)+"]", "; AReg = "+operand2+" or "+operand1);
	else if(contentsOfAReg == operand1)
		emit("", "or", "eax,["+NAME(operand2)+"]", "; AReg = "+operand1+" or "+operand2);
	
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(BOOLEAN);
	pushOperand(contentsOfAReg);
}
                                                                          
void Compiler::emitEqualityCode(string operand1, string operand2)
{
	if (!EXISTS(operand1) or !EXISTS(operand2))
		processError("undefined operands");
	if (!( ((DATATYPE(operand1) == INTEGER) and (DATATYPE(operand2) == INTEGER)) or ((DATATYPE(operand1) == BOOLEAN) and (DATATYPE(operand2) == BOOLEAN))))
      processError("illegal type, expected both boolean or both integer");
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      contentsOfAReg = "";
   }  if((contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      	emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
	   contentsOfAReg = operand2;
   }
   
   //compare register to memory
	if (contentsOfAReg == operand1)
   		emit("", "cmp", "eax,["+NAME(operand2)+"]", "; compare "+operand1+" and "+operand2);
	else if(contentsOfAReg == operand2)
   		emit("", "cmp", "eax,["+NAME(operand1)+"]", "; compare "+operand2+" and "+operand1);
	else
		processError("compiler error: error in equality logic");
	
   const auto label_equal = getLabel(); 
   emit("", "je", label_equal, "; if "+operand2+" = "+operand1+" then jump to set eax to TRUE");
   
   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
   
   const auto label_false = getLabel();
   emit("", "jmp", label_false, "; unconditionally jump");
   emit(label_equal+":", "", "", "");
   
   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
   
            //symbolTable.insert(pair<string, SymbolTableEntry>(name, SymbolTableEntry(name,inType,inMode,inValue,inAlloc,inUnits)));
   
   emit(label_false+":", "", "", "");
   
//  emit code to label the next instruction with the second acquired label L(n+1)
//  deassign all temporaries involved and free those names for reuse
//  A Register = next available temporary name and change type of its symbol table entry to boolean
	
	//insert function logic here
	
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(BOOLEAN);
	pushOperand(contentsOfAReg);
}
  
   
//   void emitEqualityCode(string operand1,string operand2) //test whether operand2 equals operand1
//{ if types of operands are not the same
//    processError(incompatible types)
//  if the A Register holds a temp not operand1 nor operand2 then
//     emit code to store that temp into memory
//     change the allocate entry for it in the symbol table to yes
//     deassign it
//  if the A register holds a non-temp not operand2 nor operand1 then deassign it
//  if neither operand is in the A register then
//    emit code to load operand2 into the A register
//  emit code to perform a register-memory compare
//  emit code to jump if equal to the next available Ln (call getLabel)
//  emit code to load FALSE into the A register
//  insert FALSE in symbol table with value 0 and external name false
//  emit code to perform an unconditional jump to the next label (call getLabel should be L(n+1))
//  emit code to label the next instruction with the first acquired label Ln
//  emit code to load TRUE into A register
//  insert TRUE in symbol table with value -1 and external name true
//  emit code to label the next instruction with the second acquired label L(n+1)
//  deassign all temporaries involved and free those names for reuse
//  A Register = next available temporary name and change type of its symbol table entry to boolean
//} p
         // op2 == op1
void Compiler::emitInequalityCode(string operand1, string operand2)    // op2 != op1
{
	if (!EXISTS(operand1) or !EXISTS(operand2))
		processError("undefined operands");
	if (!( ((DATATYPE(operand1) == INTEGER) and (DATATYPE(operand2) == INTEGER)) or ((DATATYPE(operand1) == BOOLEAN) and (DATATYPE(operand2) == BOOLEAN))))
      processError("illegal type, expected both boolean or both integer");
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      contentsOfAReg = "";
   }  if((contentsOfAReg != operand1) and (contentsOfAReg != operand2)) {
      	emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
	   contentsOfAReg = operand2;
   }
	
	//insert function logic here
	if (contentsOfAReg == operand1)
   		emit("", "cmp", "eax,["+NAME(operand2)+"]", "; compare "+operand1+" and "+operand2);
	else if(contentsOfAReg == operand2)
		emit("", "cmp", "eax,["+NAME(operand1)+"]", "; compare "+operand2+" and "+operand1);
	
   const auto label_equal = getLabel(); 
   emit("", "jne", label_equal, "; je to " + label_equal);
   
   emit("", "mov", "eax,[FALSE]", "; Areg = FALSE");
   
   const auto label_false = getLabel();
   emit("", "jmp", label_equal, "; unconditionally jump");
   emit(label_equal+":", "", "", "");
   
   emit("", "mov", "eax,[TRUE]", "; Areg = TRUE");
   
   emit(label_false+":", "", "", "");
 
	
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(BOOLEAN);
	pushOperand(contentsOfAReg);
}
void Compiler::emitLessThanCode(string operand1, string operand2)       // op2 <  op1
{
	if (!EXISTS(operand1) or !EXISTS(operand2))
		processError("undefined operands");
	if (!( ((DATATYPE(operand1) == INTEGER) and (DATATYPE(operand2) == INTEGER))))
      processError("illegal type, expected both integer");
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand2)) {
      contentsOfAReg = "";
   }  if(contentsOfAReg != operand2) {
      	emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
	   contentsOfAReg = operand2;
   }
   
   //compare register to memory
	else if(contentsOfAReg == operand2)
   		emit("", "cmp", "eax,["+NAME(operand1)+"]", "; compare "+operand2+" and "+operand1);
	else
		processError("compiler error: error in less than logic");
	
   const auto label_equal = getLabel(); 
   emit("", "jl", label_equal, "; if "+operand2+" < "+operand1+" then jump to set eax to TRUE");
   
   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
   
   const auto label_false = getLabel();
   emit("", "jmp", label_false, "; unconditionally jump");
   emit(label_equal+":", "", "", "");
   
   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
   
            //symbolTable.insert(pair<string, SymbolTableEntry>(name, SymbolTableEntry(name,inType,inMode,inValue,inAlloc,inUnits)));
   
   emit(label_false+":", "", "", "");
   
//  emit code to label the next instruction with the second acquired label L(n+1)
//  deassign all temporaries involved and free those names for reuse
//  A Register = next available temporary name and change type of its symbol table entry to boolean
	
	//insert function logic here
	
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(BOOLEAN);
	pushOperand(contentsOfAReg);
}
void Compiler::emitLessThanOrEqualToCode(string operand1, string operand2) // op2 <= op1
{
	if (!EXISTS(operand1) or !EXISTS(operand2))
		processError("undefined operands");
	if (!( ((DATATYPE(operand1) == INTEGER) and (DATATYPE(operand2) == INTEGER))))
      processError("illegal type, expected both integer");
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand2)) {
      contentsOfAReg = "";
   }  if(contentsOfAReg != operand2) {
      	emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
	   contentsOfAReg = operand2;
   }
   
   //compare register to memory
	else if(contentsOfAReg == operand2)
   		emit("", "cmp", "eax,["+NAME(operand1)+"]", "; compare "+operand2+" and "+operand1);
	else
		processError("compiler error: error in less than logic");
	
   const auto label_equal = getLabel(); 
   emit("", "jle", label_equal, "; if "+operand2+" <= "+operand1+" then jump to set eax to TRUE");
   
   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
   
   const auto label_false = getLabel();
   emit("", "jmp", label_false, "; unconditionally jump");
   emit(label_equal+":", "", "", "");
   
   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
   
            //symbolTable.insert(pair<string, SymbolTableEntry>(name, SymbolTableEntry(name,inType,inMode,inValue,inAlloc,inUnits)));
   
   emit(label_false+":", "", "", "");
   
//  emit code to label the next instruction with the second acquired label L(n+1)
//  deassign all temporaries involved and free those names for reuse
//  A Register = next available temporary name and change type of its symbol table entry to boolean
	
	//insert function logic here
	
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(BOOLEAN);
	pushOperand(contentsOfAReg);
}
void Compiler::emitGreaterThanCode(string operand1, string operand2)    // op2 >  op1
{
	if (!EXISTS(operand1) or !EXISTS(operand2))
		processError("undefined operands");
	if (!( ((DATATYPE(operand1) == INTEGER) and (DATATYPE(operand2) == INTEGER))))
      processError("illegal type, expected both integer");
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand2)) {
      contentsOfAReg = "";
   }  if(contentsOfAReg != operand2) {
      	emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
	   contentsOfAReg = operand2;
   }
   
   //compare register to memory
	if(contentsOfAReg == operand2)
   		emit("", "cmp", "eax,["+NAME(operand1)+"]", "; compare "+operand2+" and "+operand1);
	else
		processError("compiler error: error in less than logic");
	
   const auto label_equal = getLabel(); 
   emit("", "jg", label_equal, "; if "+operand2+" > "+operand1+" then jump to set eax to TRUE");
   
   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
   
   const auto label_false = getLabel();
   emit("", "jmp", label_false, "; unconditionally jump");
   emit(label_equal+":", "", "", "");
   
   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
   
            //symbolTable.insert(pair<string, SymbolTableEntry>(name, SymbolTableEntry(name,inType,inMode,inValue,inAlloc,inUnits)));
   
   emit(label_false+":", "", "", "");
   
//  emit code to label the next instruction with the second acquired label L(n+1)
//  deassign all temporaries involved and free those names for reuse
//  A Register = next available temporary name and change type of its symbol table entry to boolean
	
	//insert function logic here
	
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(BOOLEAN);
	pushOperand(contentsOfAReg);
}
void Compiler::emitGreaterThanOrEqualToCode(string operand1, string operand2) // op2 >= op1
{
	if (!EXISTS(operand1) or !EXISTS(operand2))
		processError("undefined operands");
	if (!(((DATATYPE(operand1) == INTEGER) and (DATATYPE(operand2) == INTEGER))))
      processError("illegal type, expected both integer");
   if(isTemporary(contentsOfAReg) and (contentsOfAReg != operand2)) {
      emit("", "mov", "["+contentsOfAReg+"],eax", "; deassign AReg");
	   symbolTable.find(contentsOfAReg)->second.setAlloc(YES);
	   contentsOfAReg = ""; //May need to change, no clue what it means to deassign the AReg
   } else if (!isTemporary(contentsOfAReg) and (contentsOfAReg != operand2)) {
      contentsOfAReg = "";
   }  if(contentsOfAReg != operand2) {
      	emit("", "mov", "eax,["+symbolTable.find(operand2)->second.getInternalName()+"]", "; AReg = "+operand2);
	   contentsOfAReg = operand2;
   }
   
   //compare register to memory
	else if(contentsOfAReg == operand2)
   		emit("", "cmp", "eax,["+NAME(operand1)+"]", "; compare "+operand2+" and "+operand1);
	else
		processError("compiler error: error in less than logic");
	
   const auto label_equal = getLabel(); 
   emit("", "jge", label_equal, "; if "+operand2+" >= "+operand1+" then jump to set eax to TRUE");
   
   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
   
   const auto label_false = getLabel();
   emit("", "jmp", label_false, "; unconditionally jump");
   emit(label_equal+":", "", "", "");
   
   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
   
            //symbolTable.insert(pair<string, SymbolTableEntry>(name, SymbolTableEntry(name,inType,inMode,inValue,inAlloc,inUnits)));
   
   emit(label_false+":", "", "", "");
   
//  emit code to label the next instruction with the second acquired label L(n+1)
//  deassign all temporaries involved and free those names for reuse
//  A Register = next available temporary name and change type of its symbol table entry to boolean
	
	//insert function logic here
	
	if (isTemporary(operand1))
		freeTemp();
	if (isTemporary(operand2))
		freeTemp();
	contentsOfAReg = getTemp();
	symbolTable.find(contentsOfAReg)->second.setDataType(BOOLEAN);
	pushOperand(contentsOfAReg);
}
