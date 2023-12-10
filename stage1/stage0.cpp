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
   
   if (op == "program") {
      emitPrologue(operand1);
   } else if (op == "end") {
      emitEpilogue();
   }  else if (op == "read") { 
      emitReadCode(operand1);
   } else if (op == "write") {
      emitWriteCode(operand1);
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
            std::cout << "table value found for " << table_value->second.getInternalName() << " is " << table_value->second.getValue() << "\n";
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
   auto keywords = {"begin", "end", "var", "const", "program", "true", "false", "not", "integer", "boolean"};
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
