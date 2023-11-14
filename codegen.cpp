#include "stage0.h"

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
            symbolTable.find(name)->second = SymbolTableEntry{name,inType,inMode,inValue,inAlloc,inUnits};
         } else {
            symbolTable.find(name)->second = SymbolTableEntry{genInternalName(inType),inType,inMode,inValue,inAlloc,inUnits};
            //symbolTable[name]= SymbolTableEntry(genInternalName(inType),inType,inMode,inValue,inAlloc,inUnits);
         }
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

   if (isLiteral(name))
   {
      if (isBoolean(name)) {
         dataType = BOOLEAN;
      }
      else {
         dataType = INTEGER;
      }
   }

   else //name is an identifier and hopefully a constant
   {
      if (symbolTable.find(name) != symbolTable.end()) {
         dataType = symbolTable.find(name)->second.getDataType();
      } else {
         processError("reference to undefined constant");
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
         processError("reference to undefined constant");
      }
   }
   return value;
}
