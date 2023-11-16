#include "stage0.h"
#include <iterator>

void Compiler::insert(string externalName, storeTypes inType, modes inMode, string inValue,
      allocation inAlloc, int inUnits)
   //create symbol table entry for each identifier in list of external names
   //Multiply inserted names are illegal
{
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


      if (symbolTable.find(name) != symbolTable.end())
         processError("multiple name definition");
      else if (isKeyword(name))
         processError("illegal use of keyword");
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
      newName += char(intCount+'0');
      ++intCount;
   }
   else if (inType == BOOLEAN)
   {
      newName += 'B';
      newName += char(boolCount+'0');
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
	(void) operand2; //suppress warning
	if (op == "program") {
		emitPrologue(operand1);
   } else if (op == "end") {
		emitEpilogue();
   } else {
		processError("compiler error since function code should not be called with illegal arguments");
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
	objectFile << left << setw(36) << "; Robert Burnett, Matthew Barton" << ctime(&now);
	
	objectFile << "%INCLUDE \"Along32.inc\"" << endl;
	objectFile << "%INCLUDE \"Macros_Along.inc\"" << setw(0) << endl << endl;
	
	emit("SECTION", ".text");
	emit("global", "_start", "", "; program " + progName);
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
	emit();
	emit("SECTION", ".data");
	
	for (auto itr = symbolTable.begin(); itr != symbolTable.end(); ++itr)
		if ((itr->second.getAlloc() == YES) && (itr->second.getMode() == CONSTANT))
		{
			emit(itr->second.getInternalName(), "dd", itr->second.getValue(), "; "+itr->first);
		}
	
	emit();
	emit("SECTION", ".bss");
	
	for (auto itr = symbolTable.begin(); itr != symbolTable.end(); ++itr)
		if ((itr->second.getAlloc() == YES) && (itr->second.getMode() == VARIABLE))
		{
			emit(itr->second.getInternalName(), "resd", "1" , "; "+itr->first); // I hard coded in 1 for units, but this needs to be changed later I think
		}
	
}
