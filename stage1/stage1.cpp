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

	  if(name.length() > 15){
         name = name.substr(0, 15);
      }
	   
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
	   
	  if(name.length() > 15){
         name = name.substr(0, 15);
      }

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
         processError("assignment operand types must match, " + operand1 + " is of type " 
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
      processError("undefined operands in addition operation");
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
      processError("undefined operands in subtraction operation");
   if((symbolTable.find(operand1)->second.getDataType() != INTEGER) or (symbolTable.find(operand2)->second.getDataType() != INTEGER)) {
      processError("binary \'-\' requires integer operands");
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
      processError("undefined operands in multiplication operation");
   if((symbolTable.find(operand1)->second.getDataType() != INTEGER) or (symbolTable.find(operand2)->second.getDataType() != INTEGER)) {
      processError("binary \'*\' requires integer operands");
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
      processError("undefined arguments in division operation");
   if ((DATATYPE(operand1) != INTEGER) or (DATATYPE(operand2) != INTEGER))
      processError("binary \'div\' requires integer operands");
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
      processError("undefined arguments in modulus operation");
   if ((DATATYPE(operand1) != INTEGER) or (DATATYPE(operand2) != INTEGER))
      processError("binary \'mod\' requires integer operands");
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
      processError("undefined argument in negation operation");
   if (symbolTable.find(operand1)->second.getDataType() != INTEGER)
      processError("unary \'-\' requires integer operand");
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
      processError("undefined operand in not operation");
   if ((DATATYPE(operand1) != BOOLEAN))
      processError("unary \'not\' requires boolean operand");

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
      processError("undefined operands in and operation");
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
      processError("undefined operands in or operation");
   if ((DATATYPE(operand1) != BOOLEAN) or (DATATYPE(operand2) != BOOLEAN))
      processError("binary \'or\' requires boolean operands");

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
      processError("undefined operands in equality operation");
   if (!( ((DATATYPE(operand1) == INTEGER) and (DATATYPE(operand2) == INTEGER)) or ((DATATYPE(operand1) == BOOLEAN) and (DATATYPE(operand2) == BOOLEAN))))
      processError("illegal operands, equality expected both boolean or both integer");
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
	if (!EXISTS("false"))
		symbolTable.insert(pair<string, SymbolTableEntry>("false", SymbolTableEntry("FALSE", BOOLEAN, CONSTANT, "0", YES, 1)));


   const auto label_false = getLabel();
   emit("", "jmp", label_false, "; unconditionally jump");
   emit(label_equal+":", "", "", "");

   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
	if (!EXISTS("true"))
		symbolTable.insert(pair<string, SymbolTableEntry>("true", SymbolTableEntry("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1)));


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
      processError("undefined operands in inequality operation");
   if (!( ((DATATYPE(operand1) == INTEGER) and (DATATYPE(operand2) == INTEGER)) or ((DATATYPE(operand1) == BOOLEAN) and (DATATYPE(operand2) == BOOLEAN))))
      processError("illegal type, inequality expected both boolean or both integer");
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
   emit("", "jne", label_equal, "; if "+operand2+" <> "+operand1+" then jump to set eax to TRUE");

   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
	if (!EXISTS("false"))
		symbolTable.insert(pair<string, SymbolTableEntry>("false", SymbolTableEntry("FALSE", BOOLEAN, CONSTANT, "0", YES, 1)));


   const auto label_false = getLabel();
   emit("", "jmp", label_false, "; unconditionally jump");
   emit(label_equal+":", "", "", "");

   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
	if (!EXISTS("true"))
		symbolTable.insert(pair<string, SymbolTableEntry>("true", SymbolTableEntry("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1)));


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
      processError("undefined operands in less than operation");
   if (!( ((DATATYPE(operand1) == INTEGER) and (DATATYPE(operand2) == INTEGER))))
      processError("illegal type, less than expected both integer");
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
   emit("", "jl", label_equal, "; if "+operand2+" < "+operand1+" then jump to set eax to TRUE");

   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
	if (!EXISTS("false"))
		symbolTable.insert(pair<string, SymbolTableEntry>("false", SymbolTableEntry("FALSE", BOOLEAN, CONSTANT, "0", YES, 1)));


   const auto label_false = getLabel();
   emit("", "jmp", label_false, "; unconditionally jump");
   emit(label_equal+":", "", "", "");

   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
	if (!EXISTS("true"))
		symbolTable.insert(pair<string, SymbolTableEntry>("true", SymbolTableEntry("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1)));


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
      processError("undefined operands in less than or equal to operation");
   if (!( ((DATATYPE(operand1) == INTEGER) and (DATATYPE(operand2) == INTEGER))))
      processError("illegal type, less than or equal to expected both integer");
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
   emit("", "jle", label_equal, "; if "+operand2+" <= "+operand1+" then jump to set eax to TRUE");

   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
	if (!EXISTS("false"))
		symbolTable.insert(pair<string, SymbolTableEntry>("false", SymbolTableEntry("FALSE", BOOLEAN, CONSTANT, "0", YES, 1)));


   const auto label_false = getLabel();
   emit("", "jmp", label_false, "; unconditionally jump");
   emit(label_equal+":", "", "", "");

   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
	if (!EXISTS("true"))
		symbolTable.insert(pair<string, SymbolTableEntry>("true", SymbolTableEntry("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1)));


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
      processError("undefined operands in greater than operation");
   if (!( ((DATATYPE(operand1) == INTEGER) and (DATATYPE(operand2) == INTEGER))))
      processError("illegal type, greater than expected both integer");
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
	if (!EXISTS("false"))
		symbolTable.insert(pair<string, SymbolTableEntry>("false", SymbolTableEntry("FALSE", BOOLEAN, CONSTANT, "0", YES, 1)));


   const auto label_false = getLabel();
   emit("", "jmp", label_false, "; unconditionally jump");
   emit(label_equal+":", "", "", "");

   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
	if (!EXISTS("true"))
		symbolTable.insert(pair<string, SymbolTableEntry>("true", SymbolTableEntry("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1)));


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
      processError("undefined operands in greater than or equal to operation");
   if (!(((DATATYPE(operand1) == INTEGER) and (DATATYPE(operand2) == INTEGER))))
      processError("illegal type, greater than or equal to expected both integer");
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
   emit("", "jge", label_equal, "; if "+operand2+" >= "+operand1+" then jump to set eax to TRUE");

   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
	if (!EXISTS("false"))
		symbolTable.insert(pair<string, SymbolTableEntry>("false", SymbolTableEntry("FALSE", BOOLEAN, CONSTANT, "0", YES, 1)));


   const auto label_false = getLabel();
   emit("", "jmp", label_false, "; unconditionally jump");
   emit(label_equal+":", "", "", "");

   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
	if (!EXISTS("true"))
		symbolTable.insert(pair<string, SymbolTableEntry>("true", SymbolTableEntry("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1)));


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


void Compiler::execStmts(){
   nextToken();
   if (token == "end") {
      return;
   }

   if(token != "read" and token != "write" and !isNonKeyId(token)){
      processError("[execStmts] expected \"read\", \"write\", or \"NON_KEY_ID\", found \"" + token + "\"");
   }
   execStmt();
   execStmts();
}    

void Compiler::execStmt(){

   if(token == "read") {
      readStmt();
   } else if (token == "write") {
      writeStmt();
   } else if(isNonKeyId(token)) {
      assignStmt();
   } else {
      processError("[execStmt] expected \"read\", \"write\", or \"NON_KEY_ID\", found \"" + token + "\"");
   }
}

void Compiler::assignStmt(){
   //cout << "[assignSmt]   " << token << endl;
   if(!isNonKeyId(token)) {
      processError("expected NON_KEY_ID in assignStmt");
   }
   const auto lhs = token;
   nextToken();
   //cout << "             " << token << endl;
   if(token != ":=") {
      processError("expected \":=\" after NON_KEY_ID in assignStmt");
   }
   pushOperator(":=");
   pushOperand(lhs);
   nextToken();
   //cout << "              " << token << endl;
   express();
   //nextToken();
   //cout << "              " << token << endl;
   if(token != ";") {
      processError("expected \";\" after assignStmt");
   }
   
   //cout << "    assignStmt";
   const auto my_operator = popOperator();
   const auto operand1 = popOperand();
   const auto operand2 = popOperand();
   code(my_operator, operand1, operand2);
} 

void Compiler::readStmt(){
   if(token != "read"){
      processError("\"read\" expected");
   }
   nextToken();
   if(token != "(") {
      processError("\"(\" expected after \"read\"");
   }
   nextToken();
   auto csv = ids();
   if(token != ")") {
      processError("\")\" expected after \"read(...\", found \" " + token + "\"");
   }
   nextToken();
   if(token != ";") {
      processError("\";\" expected after \"read(...)\"");
   }
   code("read", csv);
}

void Compiler::writeStmt(){
   if(token != "write"){
      processError("\"write\" expected");
   }
   nextToken();
   if(token != "(") {
      processError("\"(\" expected after \"write\"");
   }
   nextToken();
   auto csv = ids();
   if(token != ")") {
      processError("\")\" expected after \"write(...\", found \" " + token + "\"");
   }
   nextToken();
   if(token != ";") {
      processError("\";\" expected after \"write(...)\"");
   }
   //emitWriteCode(csv);
   code("write", csv);
}


void Compiler::express(){
   //cout << "[express  ]   " << token << endl;

   if(token != "not"
         and token != "true"
         and token != "false"
         and token != "("
         and token != "+"
         and token != "-"
         and !isInteger(token)
         and !isNonKeyId(token)
     ){
      processError("[express] expected true, false, (, +, -, INTEGER, or NON_KEY_ID");
   }
   term();
   expresses();
}

bool isRelOp(string token) {
   return (token == "<>"
         or token == "="
         or token == "<="
         or token == ">="
         or token == "<"
         or token == ">"
         );
}

void Compiler::expresses(){
   //cout << "[expresses]   " << token << endl;
   if (isRelOp(token)
      ){
      pushOperator(token);
      nextToken();
      //cout << "           " << token << endl;
      term();
      const auto my_operator = popOperator();
      const auto operand1 = popOperand();
      const auto operand2 = popOperand();
      code(my_operator, operand1, operand2);

      //nextToken();
      //cout << "           " << token << endl;
      expresses();
   } else if (token == ")" or token == ";") {
      return;
   } else {
      processError("[expresses] expected REL_OP, \")\", or \";\", found " + token);
   }
}

bool isAddLevelOp(string token) {
   return (
         token == "or"
         or token == "+"
         or token == "-"
         );
}

void Compiler::term(){
   //cout << "[term     ]   " << token << endl;
   if(token != "not"
         and token != "true"
         and token != "false"
         and token != "("
         and token != "+"
         and token != "-"
         and !isInteger(token)
         and !isNonKeyId(token)
     ){
      processError("[term] expected true, false, (, +, -, INTEGER, or NON_KEY_ID, found \"" + token + "\"");
   }
   factor();
   terms();
}

void Compiler::terms(){
   //cout << "[terms    ]   " << token << endl;
   if(isAddLevelOp(token)){
      pushOperator(token);
      nextToken();
      //cout << "              " << token << endl;
      factor();

      //cout << "    terms";
      const auto my_operator = popOperator();
      const auto operand1 = popOperand();
      const auto operand2 = popOperand();
      code(my_operator, operand1, operand2);
      terms();
   } else if (
         token == "<>"
         or token == "="
         or token == "<="
         or token == ">="
         or token == "<"
         or token == ">"
         or token == ")"
         or token == ";"
         ) {
      return;
   } else {
      processError("[terms] expected REL_OP, \")\", or \";\", found \"" + token + "\"");
   }
} 

void Compiler::factor(){
   //cout << "[factor   ]   " << token << endl;
   if(token != "not"
         and token != "true"
         and token != "false"
         and token != "("
         and token != "+"
         and token != "-"
         and !isInteger(token)
         and !isNonKeyId(token)
     ){
      processError("[factor] expected true, false, (, +, -, INTEGER, or NON_KEY_ID, found \"" + token + "\"");
   }
   part();
   factors();
}

bool isMultLevelOp(string token) {
   return (token == "*"
         or token == "div"
         or token == "mod"
         or token == "and"
         );
}

void Compiler::factors(){
   //cout << "[factors  ]   " << token << endl;
   if(isMultLevelOp(token)){
      pushOperator(token);
      nextToken();
      //cout << "              " << token << endl;
      part();

      //cout << "    factors";
      const auto my_operator = popOperator();
      const auto operand1 = popOperand();
      const auto operand2 = popOperand();
      code(my_operator, operand1, operand2);

      factors();
      
   } else if (token == "<>"
         or token == "="
         or token == "<="
         or token == ">="
         or token == "<"
         or token == ">"
         or token == ")"
         or token == ";"
         or token == "+"
         or token == "-"
         or token == "or"
         ) {
      return;
   } else {
      processError("[factors] expected ADD_LEVEL_OP, MULT_LEVEL_OP, REL_OP, \")\", or \";\", found " + token);
   }
}      

void Compiler::part(){
   //cout << "[part     ]   " << token << endl;

   //'not' ( '(' EXPRESS ')' code('not',popOperand)  | 
   //BOOLEANx pushOperand(not x; i.e., 'true' or 'false')  |
   //NON_KEY_IDx code('not',x) )
   if(token == "not"){
      nextToken();
      //cout << "              " << token << endl;

      if(token == "(") {

         nextToken();
         //cout << "              " << token << endl;
         express();
         //    nextToken();
         if(token != ")") {
            processError("expected \")\" after (...");
         }
         //cout << "    part not";
         code("not", popOperand());
      } else if(isBoolean(token)) {
         pushOperand(token == "true" ? "false" : "true" );
      } else if(isNonKeyId(token)) {
         //cout << "and";
         code("not", token);
      } else {
         processError("[part] expected (, BOOLEAN, or NON_KEY_ID after \"not\"");
      }

      //'+' ( '(' EXPRESS ')' |
      //( INTEGERx | NON_KEY_IDx ) pushOperand(x) ) 
   } else if(token == "+"){
      nextToken();
      //cout << "              " << token << endl;
      if(token == "(") {
         nextToken();
         //cout << "              " << token << endl;
         express();
         if(token != ")") {
            processError("expected \")\" after (...");
         }
      } else if(isInteger(token) or isNonKeyId(token)) {
         pushOperand(token);
      } else {
         processError("[part] expected '(', INTEGER, or NON_KEY_ID after \"+\", found \"" + token + "\"");
      }  

      //'-' ( '(' EXPRESS ')' code('neg',popOperand) |
      //INTEGERx pushOperand('-'+ x) |
      //NON_KEY_IDx code('neg',x) )
   } else if(token == "-"){
      nextToken();
      //cout << "              " << token << endl;

      if(token == "(") {
         nextToken();
         //cout << "              " << token << endl;
         express();
         if(token != ")") {
            processError("expected \")\" after (...");
         }
         //cout << "    part neg";
         code("neg", popOperand());
      } else if (isInteger(token)) {
         pushOperand("- " + token);
      } else if (isNonKeyId(token)) {
         //cout << "    part neg keyid";
         code("neg", token);
      } else {
         processError("[part] INTEGER, \"(\", or NON_KEY_ID after \"-\"");
      }

      //'(' EXPRESS ')'
   } else if (token == "(") {
      nextToken();
      //cout << "              " << token << endl;
      express();
      if(token != ")") {
         processError("expected \")\" after (...");
      }
   } else if(isInteger(token)){
      pushOperand(token);
   } else if(isBoolean(token)) {
      pushOperand(token);
   } else if(isNonKeyId(token)) {
      pushOperand(token);
   } else {
      processError("[part] expected: not, +, -, (, INTEGER, BOOLEAN, NON_KEY_ID, found\"" + token + "\"");
   }

   //get the next token
   nextToken();
   //cout << "              " << token << endl;
}


//Coded by: Robert Burnett and Matthew Barton
//Stage0 Compiler for Pascallite
#include "stage1.h"
#include <iomanip>
#include <ctime>
#include <algorithm>

void Compiler::insert(string externalName, storeTypes inType, modes inMode, string inValue,
      allocation inAlloc, int inUnits)
   //create symbol table entry for each identifier in list of external names
   //Multiply inserted names are illegal
{
   //cout << "inserting " << externalName << endl;
   string name;
   while (externalName != "")
   {
      name.clear();
      while ((externalName != "") && (externalName.front() != ','))
      {
         name += externalName.front();
         externalName.erase(0,1);
      }
      if ((externalName != "") && (externalName.front() == ','))
         externalName.erase(0,1);

      //Robert: cap name at 15 chars
      if(name.length() > 15){
         name = name.substr(0, 15);
      }

      if (symbolTable.find(name) != symbolTable.end())
         processError("multiple name definition");
      else if (isKeyword(name))
         processError("illegal use of keyword \"" + name + "\"");
      else if (symbolTable.size() > 255)
         processError("Symbol Table overflow");
      else //create table entry
      {
         if ((name.front() >= 'A') && (name.front() <= 'Z')) {
            //symbolTable.insert(SymbolTableEntry{name,inType,inMode,inValue,inAlloc,inUnits});
            //symbolTable.find(name)->second = SymbolTableEntry{name,inType,inMode,inValue,inAlloc,inUnits};
            symbolTable.insert(pair<string, SymbolTableEntry>(name, SymbolTableEntry(name,inType,inMode,inValue,inAlloc,inUnits)));
         } else {
            //symbolTable.find(name)->second = SymbolTableEntry{genInternalName(inType),inType,inMode,inValue,inAlloc,inUnits};
            //symbolTable[name]= SymbolTableEntry(genInternalName(inType),inType,inMode,inValue,inAlloc,inUnits);
            symbolTable.insert(pair<string, SymbolTableEntry>(name, SymbolTableEntry(genInternalName(inType),inType,inMode,inValue,inAlloc,inUnits)));
         }  //Matt:  Not sure if this is gonna work, I don't think name has been inserted to the symbolTable yet and symbolTable.find(name) will always return null ptr
      }
   }
}

//Generates unique, legal internal names
string Compiler::genInternalName(storeTypes inType) const
{
   string newName;
   static int boolCount = 0, intCount = 0;
   if (inType == INTEGER)
   {
      newName += 'I';
      newName += to_string(intCount);
      ++intCount;
   }
   else if (inType == BOOLEAN)
   {
      newName += 'B';
      newName += to_string(boolCount);
      ++boolCount;
   }
   else if (inType == PROG_NAME)
      newName = "P0";					//temporary until I figure out what I'm supposed to name this
                                    //Rob: I think its supposed to be P0? or P1

   return newName;
}

storeTypes Compiler::whichType(string name) //tells which data type a name has
{
   storeTypes dataType = INTEGER;

   //TODO I think this logic here is wrong, or the definition of isLiteral is wrong
   if (isLiteral(name))
   {
      if (isBoolean(name)) {
         dataType = BOOLEAN;
         return dataType;
      }
      else {
         dataType = INTEGER;
         return dataType;
      }
   }

   else //name is an identifier and hopefully a constant
   {
      if (symbolTable.find(name) != symbolTable.end()) {
         dataType = symbolTable.find(name)->second.getDataType();
      } else {
         processError("reference to undefined constant \"" + name + "\"");
      }
   }

   return dataType;
}

string Compiler::whichValue(string name) //tells which value a name has
{
   string value;

   if (isLiteral(name)) {
      value = name;
   } else { //name is an identifier and hopefully a constant
      if (symbolTable.find(name) != symbolTable.end()) {
         value = symbolTable.find(name)->second.getValue();
      } else {
         processError("reference to undefined constant \"" + name + "\"");
      }
   }
   return value;
}

void Compiler::code(string op, string operand1, string operand2) {
   //cout << "    code(" + op + ", " + operand1 + ", " + operand2 + ")" << endl;
   
   
     if (op == "read") { 
      emitReadCode(operand1);
 	  return;
   } else if (op == "write") {
      emitWriteCode(operand1);
	  return;
	 }
	
	if(operand1.length() > 15){
         operand1 = operand1.substr(0, 15);
      }
	if(operand2.length() > 15){
         operand2 = operand2.substr(0, 15);
      }
	
	if (op == "program") {
      emitPrologue(operand1);
   } else if (op == "end") {
      emitEpilogue();
   } else if (op == "+") { // this must be binary '+'
      emitAdditionCode(operand1, operand2);
   } else if (op == "-") { // this must be binary '-'
      emitSubtractionCode(operand1, operand2);
   } else if (op == "neg") { // this must be unary '-'
      emitNegationCode(operand1);
   } else if (op == "not") {
      emitNotCode(operand1);
   } else if (op == "*") {
      emitMultiplicationCode(operand1, operand2);
   } else if (op == "div") {
      emitDivisionCode(operand1, operand2);
   } else if (op == "mod") {
      emitModuloCode(operand1, operand2);
   } else if (op == "and") {
      emitAndCode(operand1, operand2);
   } else if (op == "=") {
      emitEqualityCode(operand1, operand2);
   } else if (op == ":=") {
      emitAssignCode(operand1, operand2);
   } else if (op == "or") {
      emitOrCode(operand1, operand2);
   } else if (op == "<>") {
      emitInequalityCode(operand1, operand2);
   } else if (op == "<") {
      emitLessThanCode(operand1, operand2);
   } else if (op == "<=") {
      emitLessThanOrEqualToCode(operand1, operand2);
   } else if (op == ">") {
      emitGreaterThanCode(operand1, operand2);
   } else if (op == ">=") {
      emitGreaterThanOrEqualToCode(operand1, operand2);
   } else {
      processError("[code] compiler error since function code should not be called with illegal arguments.\n args: " + operand1 + ", " + operand2);
   }
}

void Compiler::emit(string label, string instruction, string operands, string comment) {	
   if (operands == "true")
      operands = "-1";
   if (operands == "false")
      operands = "0";
   objectFile << left;
   objectFile << setw(8) << label << setw(8) << instruction << setw(24) << operands <<  comment << endl;
}

void Compiler::emitPrologue(string progName, string operand2) {
   (void) operand2; //suppress warning
   time_t now = time (NULL);
   objectFile << left << setw(36) << "; Robert Burnett, Matthew Barton" << setw(0) << ctime(&now);

   objectFile << "%INCLUDE \"Along32.inc\"" << endl;
   objectFile << "%INCLUDE \"Macros_Along.inc\"" << endl << endl;

   emit("SECTION", ".text");
   emit("global", "_start", "", "; program " + progName.substr(0, 15));
   objectFile << endl;
   emit("_start:");
}

void Compiler::emitEpilogue(string operand1, string operand2) {
   (void) operand1; //suppress warning
   (void) operand2; //suppress warning
   emit("","Exit", "{0}");
   emitStorage();
}

void Compiler::emitStorage()
{
   objectFile << endl;
   emit("SECTION", ".data");

   for (auto itr = symbolTable.begin(); itr != symbolTable.end(); ++itr)
      if ((itr->second.getAlloc() == YES) && (itr->second.getMode() == CONSTANT))
      {
         emit(itr->second.getInternalName(), "dd", itr->second.getValue(), "; "+itr->first);
      }

   objectFile << endl;
   emit("SECTION", ".bss");

   for (auto itr = symbolTable.begin(); itr != symbolTable.end(); ++itr)
      if ((itr->second.getAlloc() == YES) && (itr->second.getMode() == VARIABLE))
      {
         emit(itr->second.getInternalName(), "resd", to_string(itr->second.getUnits()) , "; "+itr->first);
      }

}

//==========+GRAMMAR===============
void Compiler::prog() //token should be "program"
{
   if (token != "program") {
      processError("keyword \"program\" expected");
   } 

   progStmt();

   if (token == "const") {
      consts();
   }

   if (token == "var") {
      vars();
   }

   if (token != "begin") {
      processError("keyword \"begin\" expected");
   }

   beginEndStmt();

   if (ch != END_OF_FILE)  {
      processError("no text may follow \"end\", found \"" + token + "\"");
   }
}

void Compiler::progStmt() //token should be "program"
{
   if (token != "program") {
      processError("keyword \"program\" expected");
   }

   string x = nextToken();
   if (!isNonKeyId(x)) {
      processError("program name expected");
   }
   if (nextToken() != ";") {
      processError("semicolon expected");
   }
   nextToken();
   code("program", x);
   insert(x,PROG_NAME,CONSTANT,x,NO,0);
}

void Compiler::consts() //token should be "const"
{
   if (token != "const") {
      processError("keyword \"const\" expected");
   }
   if (!isNonKeyId(nextToken())) {
      processError("non-keyword identifier must follow \"const\"");
   }
   constStmts();
}

void Compiler::vars() //token should be "var"
{
   if (token != "var") {
      processError("keyword \"var\" expected");
   }
   if (!isNonKeyId(nextToken())) {
      processError("non-keyword identifier must follow \"var\"");
   }
   varStmts();
}

void Compiler::beginEndStmt() //token should be "begin"
{
   if (token != "begin") {
      processError("keyword \"begin\" expected");
   }

   execStmts();

   if (token != "end") {
      processError("keyword \"end\" expected");
   } 
   if (nextToken() != ".") {
      processError("period expected");
   }
   nextToken();
   code("end", ".");
}

void Compiler::constStmts() //token should be NON_KEY_ID
{
   string x,y;
   if (!isNonKeyId(token)) {
      processError("non-keyword identifier expected");
   }
   x = token;
   if (nextToken() != "=") {
      processError("\"=\" expected found " + token);
   }
   y = nextToken();


   if (y != "+" and y != "-" and y != "not" and !isNonKeyId(y) and y != "true" and y!= "false" and !isInteger(y)) {
      processError("token to right of \"=\" illegal");
   }

   if (y == "+" or y == "-") 
   {
      if (!isInteger(nextToken())) {
         processError("integer expected after sign");
      }
      y = y + token;
   }

   if (y == "not")
   {
      if (!isBoolean(nextToken())) {

         auto table_value = symbolTable.find(token);

         if(table_value != symbolTable.end() && isBoolean(table_value->second.getValue()))  {
            //std::cout << "table value found for " << table_value->second.getInternalName() << " is " << table_value->second.getValue() << "\n";
            y = table_value->second.getValue() == "true" ? "false" : "true";
         } else {
            processError("boolean expected after \"not\"");
         }
      } else 
         if (token == "true") {
            y = "false";
         } else {
            y = "true";
         }
   }

   if (nextToken() != ";") {
      processError("semicolon expected");
   }

   if (whichType(y) != INTEGER and whichType(y) != BOOLEAN) {
      processError("data type of token on the right-hand side must be INTEGER or BOOLEAN");
   }

   insert(x,whichType(y),CONSTANT,whichValue(y),YES,1);
   x = nextToken();

   if (x != "begin" and x != "var" and !isNonKeyId(x)) {
      processError("non-keyword identifier, \"begin\", or \"var\" expected");
   }
   if (isNonKeyId(x)) {
      constStmts();
   }
}

void Compiler::varStmts() //token should be NON_KEY_ID
{
   string x,y;
   if (!isNonKeyId(token)) {
      processError("non-keyword identifier expected");
   }
   x = ids();
   if (token != ":") {
      processError("\":\" expected");
   }

   auto temp = nextToken();
   if (temp !=  "integer" and temp != "boolean") {
      processError("illegal type follows \":\"");
   }
   y = token;
   if (nextToken() != ";") {
      processError("semicolon expected");
   }
   //NOTE: I removed whichtype(y), that may have been incorrect

   const auto type = y == "integer"
      ? INTEGER 
      : y == "boolean"
      ? BOOLEAN : PROG_NAME;
   insert(x, type,VARIABLE,"",YES,1);

   temp = nextToken();
   if (temp != "begin" and !isNonKeyId(temp)) {
      processError("non-keyword identifier or \"begin\" expected");
   }
   if (isNonKeyId(token)) {
      varStmts();
   }
}

string Compiler::ids() //token should be NON_KEY_ID
{
   string temp,tempString;
   if (!isNonKeyId(token)) {
      processError("non-keyword identifier expected");
   }

	if(token.length() > 15){
         token = token.substr(0, 15);
      }
	
   tempString = token;
   temp = token;

   if (nextToken() == ",") {
      if (!isNonKeyId(nextToken())) {
         processError("non-keyword identifier expected");
      }
      tempString = temp + "," + ids();
   }
   return tempString;
}


//==========Helpers===========
#include <cctype>

// Helper functions for the Pascallite lexicon

bool Compiler::isKeyword(string s) const {
   auto keywords = {"begin", "end", "var", "const", "program", "true", "false", "not", "integer", "boolean", "and", "or", "mod", "div"};
   return std::find(keywords.begin(), keywords.end(), s) != keywords.end();
}

bool Compiler::isSpecialSymbol(char c) const {
   //TODO audit this
   auto symbols = { '=' , ':' , ',' , ';' , '.' , '+' , '-', '(', ')', '<', '>', '*'};
   return std::find(symbols.begin(), symbols.end(), c) != symbols.end();
}

bool Compiler::isNonKeyId(string s) const {
   auto result =  
      std::islower(s[0]) 
      && !isKeyword(s)
      && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !isdigit(c) and !islower(c) and c != '_'; }) == s.end()
      && s[s.length() - 1] != '_';

   for (uint i = 0; i < s.size(); ++i)
   {
      if ((s[i] == '_') && ((i+1) < s.size()))
         if (s[i+1] == '_')
            result = 0;
   }

   return result ;
}

bool Compiler::isInteger(string s) const {
   /*return std::find_if(s.begin(), 
         s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end(); 
	*/
	
	if (!(isdigit(s[0]) or (s[0] == '+') or (s[0] =='-')))
		return 0;
	if ((s.size() == 1) and ((s[0] == '+') or (s[0] == '-')))
		return 0;
	for (uint i = 1; i < s.size(); ++i)
	{
		if (!isdigit(s[i]))
			return 0;
	}
	
	return 1;
}
bool Compiler::isBoolean(string s) const {
   return s == "true" || s == "false";
}
bool Compiler::isLiteral(string s) const {
   return isBoolean(s) 
      || isInteger(s) 
      || (s.substr(0, 4) == "not " && isBoolean(s.substr(4, s.npos))) 
      || ((s.substr(0, 1) == "-" || s.substr(0, 1) == "+" ) && isInteger(s.substr(2, s.npos)));
}


//=============constructors, etc===========
#include <cstdlib>

Compiler::Compiler(char **argv) // constructor
{
   this->sourceFile.open(argv[1]);
   this->listingFile.open(argv[2]);
   this->objectFile.open(argv[3]);
}

Compiler::~Compiler() // destructor
{
   this->sourceFile.close();
   this->listingFile.close();
   this->objectFile.close();
}

void Compiler::createListingHeader()
{
   time_t now = time (NULL);

   this->listingFile << left << setw(42) << "STAGE0: Robert Burnett, Matthew Barton" << ctime(&now)  << std::endl;
   this->listingFile << setw(22) << "LINE NO. " << "SOURCE STATEMENT\n\n";
   ++lineNo;
   listingFile << right << setw(5) << lineNo << '|';
}

void Compiler::parser()
{
   this->ch = this->nextChar();
   //ch must be initialized to the first character of the source file
   if (this->nextToken() != "program") {
      processError("keyword \"program\" expected, found " + token);
      //a call to nextToken() has two effects
      // (1) the variable, token, is assigned the value of the next token
      // (2) the next token is read from the source file in order to make
      // the assignment. The value returned by nextToken() is also
      // the next token.
      //parser implements the grammar rules, calling first rule
   }
   this->prog();
}

void Compiler::createListingTrailer()
{
   this->listingFile << endl << left << setw(28) << "COMPILATION TERMINATED "  << errorCount <<" ERROR";
   if (errorCount != 1) listingFile << 'S';
   listingFile << " ENCOUNTERED" << std::endl;
}

void Compiler::processError(string err)
{
   //Output err to listingFile
   ++errorCount;
   listingFile << endl << "Error: Line " << lineNo << ": ";
   this->listingFile << err << std::endl;
   createListingTrailer();
   objectFile.flush();
   exit(EXIT_FAILURE);
}


//=============Lexer==============
static bool isEnd = 0;

//TODO this function is buggy on parsing stage1 operators, this needs to be fixed
string Compiler::nextToken() //returns the next token or end of file marker
{
   token = "";
   while (token == "") {
      if(ch == '{') {
         nextChar();
         while (ch != END_OF_FILE and ch != '}') {nextChar();} //empty body }
      if (ch == END_OF_FILE) {
         processError("unexpected end of file, unterminated comment");
      } else {
         nextChar();
      }
   } else if(ch == '}'){
      processError("'}' cannot begin token"); 
   } else if (isspace(ch)) {
      nextChar();
   } else if (isSpecialSymbol(ch)) {
      token = ch;
      //TODO update this temporary fix
      if ((token == "<" or token == ":" or token == ">") and isSpecialSymbol(sourceFile.peek())) {
         token += nextChar();
      }
	  if (token == "-")
	  {
		  while (isdigit(sourceFile.peek()))
			  token += nextChar();
	  }
		  
      nextChar();
   } else if(islower(ch)){ 
      token = ch;
      nextChar();

      while (islower(ch) or isdigit(ch) or ch == '_') {
         token+=ch;
         nextChar();
      }
      if (ch == END_OF_FILE) {
         processError("unexpected end of file when parsing token");
      }
   } else if (isdigit(ch)) {
      token = ch;
      while (isdigit(nextChar())) {
         token+=ch;
      }
      if (ch == END_OF_FILE) {
         processError("unexpected end of file when parsing number");
      }
   } else if(ch == END_OF_FILE || ch == EOF) {
      ch = END_OF_FILE;
      token = END_OF_FILE;
   } else{
      processError("illegal symbol, symbol \"" + string{ch} + "\" is not allowed");
   }
}
if (token == "end") {
   isEnd = 1;
}
return token;
}


char Compiler::nextChar() //returns the next character or end of file marker
{
   static bool newLine = 0;
   ch = sourceFile.get();

   if(sourceFile.eof() or ch == EOF){
      ch = END_OF_FILE;//use a special character to designate end of file
   }

   if (newLine == 1)
   {
      ++lineNo;
      listingFile << right << setw(5) << lineNo << '|';
      newLine = 0;
   }

   //print to listing file (starting new line if necessary)
   if(ch != EOF && ch != END_OF_FILE) {
      listingFile.put(ch);
      if((ch == '\n') && !isEnd) {
         newLine = 1;
      }
   }

   return ch;
}

#include <string>

void Compiler::freeTemp(){
   currentTempNo -= 1;
   if(currentTempNo < -1) {
      processError("Compiler Internal Error - currentTempNo should be >= -1");
   }
}

string Compiler::getTemp(){
   string temp;
   currentTempNo += 1;
   temp = "T" + to_string(currentTempNo);
   if(currentTempNo > maxTempNo) {
      insert(temp, UNKNOWN, VARIABLE, "", NO, 1);
      maxTempNo += 1;
   }
   return temp;
   
}
string Compiler::getLabel(){
   static int i = -1;
   i++;
   return ".L"+to_string(i);
}

bool Compiler::isTemporary(string s) const {
   return s[0] == 'T' and isInteger(s.substr(1));
}
