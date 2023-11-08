//TODO convert this file to real code






void insert(string externalName,storeType inType, modes inMode, string inValue,
 allocation inAlloc, int inUnits)
 //create symbol table entry for each identifier in list of external names
 //Multiply inserted names are illegal
{
 string name
 while (name broken from list of external names and put into name != "")
 {
 if (symbolTable[name] is defined)
 processError(multiple name definition)
 else if (name is a keyword)
 processError(illegal use of keyword)
 else //create table entry
 {
 if (name begins with uppercase)
 symbolTable[name]=(name,inType,inMode,inValue,inAlloc,inUnits)
 else
 symbolTable[name]=(genInternalName(inType),inType,inMode,inValue,
 inAlloc,inUnits)
 }
}

storeTypes whichType(string name) //tells which data type a name has
{
 if (name is a literal)
 if (name is a boolean literal)
 data type = BOOLEAN
 else
 data type = INTEGER
 else //name is an identifier and hopefully a constant
 if (symbolTable[name] is defined)
 data type = type of symbolTable[name]
 else
 processError(reference to undefined constant)
 return data type
}
string whichValue(string name) //tells which value a name has
{
 if (name is a literal)
 value = name
 else //name is an identifier and hopefully a constant
 if (symbolTable[name] is defined and has a value)
 value = value of symbolTable[name]
 else
 processError(reference to undefined constant)
 return value
}


void code(string op, string operand1, string operand2)
{
 if (op == "program")
 emitPrologue(operand1)
 else if (op == "end")
 emitEpilogue()
 else
 processError(compiler error since function code should not be called with
 illegal arguments)
}


void emit(string label, string instruction, string operands, string comment)
{
 Turn on left justification in objectFile
 Output label in a field of width 8
 Output instruction in a field of width 8
 Output the operands in a field of width 24
 Output the comment
}
void emitPrologue(string progName, string operand2)
{
 Output identifying comments at beginning of objectFile
 Output the %INCLUDE directives
 emit("SECTION", ".text")
 emit("global", "_start", "", "; program" + progName)
 emit("_start:")
}
void emitEpilogue(string operand1, string operand2)
{
 emit("","Exit", "{0}");
 emitStorage();
}
void emitStorage()
{
 emit("SECTION", ".data")
 for those entries in the symbolTable that have
 an allocation of YES and a storage mode of CONSTANT
 { call emit to output a line to objectFile }
 emit("SECTION", ".bss")
 for those entries in the symbolTable that have
 an allocation of YES and a storage mode of VARIABLE
{ call emit to output a line to objectFile }
}
