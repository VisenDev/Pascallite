#include "stage0.h"

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
      processError("no text may follow \"end\", found \"" + string{ch} + "\"");
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
   if (nextToken() != "end") {
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
         processError("boolean expected after \"not\"");
      }
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
