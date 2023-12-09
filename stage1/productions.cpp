#include "stage1.h"

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
   if(!isNonKeyId(token)) {
      processError("expected NON_KEY_ID in assignStmt");
   }
   const auto lhs = token;
   nextToken();
   if(token != ":=") {
      processError("expected \":=\" after NON_KEY_ID in assignStmt");
   }
   nextToken();
   express();
   nextToken();
   if(token != ";") {
      processError("expected \";\" after assignStmt");
   }
   code(popOperator(), popOperand(), popOperand());
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
   emitReadCode(csv);
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
   emitWriteCode(csv);

}
void Compiler::express(){
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
      or token == "=>"
      or token == "<"
      or token == ">"
   );
}

void Compiler::expresses(){
   if (isRelOp(token)
   ){
      pushOperator(token);
      nextToken();
      term();
      code(popOperator(), popOperand(), popOperand());
      nextToken();
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
      and token == "+"
      and token == "-"
   );
}

void Compiler::term(){
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
    nextToken();
    factor();
    nextToken();
    terms();
}

void Compiler::terms(){
   if(isAddLevelOp(token)){
      pushOperator(token);
      nextToken();
      factor();
      code(popOperator(), popOperand(), popOperand());
      nextToken();
      
      terms();
   } else if (
         token == "<>"
      or token == "="
      or token == "<="
      or token == "=>"
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
   if(token != "not"
      and token != "true"
      and token != "false"
      and token != "("
      and token != "+"
      and token != "-"
      and !isInteger(token)
      and !isNonKeyId(token)
   ){
      processError("expected true, false, (, +, -, INTEGER, or NON_KEY_ID");
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
   if(isMultLevelOp(token)){
      pushOperator(token);
      nextToken();
      part();
      code(popOperator(), popOperand(), popOperand());
      nextToken();

      factors();
   } else if (token == "<>"
         or token == "="
         or token == "<="
         or token == "=>"
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
      processError("[factors] expected ADD_LEVEL_OP, REL_OP, \")\", or \";\", found " + token);
   }
}      
void Compiler::part(){
   if(token == "not"){
      nextToken();
      
      if(token == "(") {
         
         nextToken();
         express();
         nextToken();
         if(token != ")") {
            processError("expected \")\" after (...");
         }
         code("not", popOperand());
         nextToken();
      } else if(isBoolean(token)) {
         pushOperand(token == "true" ? "false" : "true" );
         nextToken();
      } else if(isNonKeyId(token)) {
         code("not", token);
         nextToken();
      }
   } else if(token == "+"){
      nextToken();
      if(token == "(") {
         nextToken();
         express();
         nextToken();
         if(token != ")") {
            processError("expected \")\" after (...");
         }
         nextToken();
      } else if(isBoolean(token) or isNonKeyId(token)) {
         pushOperand(token);
         nextToken();
      } else {
         processError("[part] expected (, BOOLEAN, or NON_KEY_ID after \"not\"");
      }  

      //TODO finish implementing the code after this
   } else if(token == "-"){
      nextToken();

      if(token == "(") {
         nextToken();
         express();
         nextToken();
         if(token != ")") {
            processError("expected \")\" after (...");
         }
         code("neg", popOperand());
         nextToken();
      } 
   } else if (token == "(") {
      nextToken();
      express();
      nextToken();
      if(token != ")") {
         processError("expected \")\" after (...");
      }
      nextToken();
   } else if(isInteger(token)){
      nextToken();
      //TODO do something
   } else if(isBoolean(token)) {
      nextToken();
      //TODO do something
   } else if(isNonKeyId(token)) {
      nextToken();
      //TODO do something
   } else {
      processError("[part] expected: not, +, -, (, INTEGER, BOOLEAN, NON_KEY_ID, found\"" + token + "\"");
   }
}
