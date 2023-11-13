void Compiler::insert(string externalName,storeType inType, modes inMode, string inValue,
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
		 name += externalName.front()
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
	 if ((name.front() >= 'A') && (name.front() <= 'Z'))
		 symbolTable[name]= SymbolTableEntry(name,inType,inMode,inValue,inAlloc,inUnits);
	 else
		 symbolTable[name]= SymbolTableEntry(genInternalName(inType),inType,inMode,inValue,inAlloc,inUnits);
 }
}

string Compiler::genInternalName(storeType inType) //Generates unique, legal internal names
{
	string newName;
	static int boolCount = 0, intCount = 0;
	if (inType == "Integer")
	{
		newName += 'I';
		newName += char(intCount+'0');
		++intCount;
	}
	else if (inType == "Boolean")
	{
		newName += 'B';
		newName += char(boolCount+'0');
		++boolCount;
	}
	else if (intType == "PROG_NAME")
		newName = "PROG";					//temporary until I figure out what I'm supposed to name this
	
	return newName;
}

storeTypes Compiler::whichType(string name) //tells which data type a name has
{
	storeTypes dataType;
	
	if (name is a literal)
	{
		if (name is a boolean literal)
			dataType = "BOOLEAN";
		else
			dataType = "INTEGER";
	}
	
	else //name is an identifier and hopefully a constant
	{
		if (symbolTable.find() != symbolTable.end())
			dataType = symbolTable[name].getDataType();
		else
			processError("reference to undefined constant");
	}
	
	return dataType;
}

string Compiler::whichValue(string name) //tells which value a name has
{
	string value;
	
	if (name is a literal)
		value = name;
	else //name is an identifier and hopefully a constant
	
	if (symbolTable[name] is defined and has a value)
		value = value of symbolTable[name]
	else
		processError(reference to undefined constant)
	return value;
}