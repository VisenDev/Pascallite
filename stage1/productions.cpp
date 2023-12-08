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
   //TODO 
   //IMPORTANT
   //FINISH ACTUALLY EMITTING CODE FROM THIS
   //emitAssignCode(lhs);
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

void Compiler::expresses(){
   if (token == "<>"
      or token == "="
      or token == "<="
      or token == "=>"
      or token == "<"
      or token == ">"
   ){
      //TODO relops
      //should be right called  here before term and expresses
      term();
      expresses();
   } else if (token == ")" or token == ";") {
      return;
   } else {
      processError("[expresses] expected REL_OP, \")\", or \";\", found " + token);
   }
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
    factor();
    terms();
}
void Compiler::terms(){
   if(token == "-" or token == "+" or token == "or") {
      //TODO 
      //add_level_op should be herek
      factor();
      terms();
   } else if (token == "<>"
      or token == "="
      or token == "<="
      or token == "=>"
      or token == "<"
      or token == ">"
   ) {
      return;
   } else {
      processError("expected REL_OP, \")\", or \";\"");
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

void Compiler::factors(){
   if(token == "*"
      or token == "div"
      or token == "mod"
      or token == "and"
   ){
      //TODO MULT_LEVEL_OP here
      nextToken();
      //replace nextToken() with multi level op parsing
      
      part();
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
         nextToken();
      } else if(isBoolean(token)) {
         //TODO am I supposed to have a code here or something?
         //or just return
         nextToken();
      } else if(isNonKeyId(token)) {

         //TODO am I supposed to have a code here or something?
         //or just return
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
      } else if(isBoolean(token)) {
         //TODO am I supposed to have a code here or something?
         //or just return
         nextToken();
      } else if(isNonKeyId(token)) {

         //TODO am I supposed to have a code here or something?
         //or just return
         nextToken();
      }
   } else if(token == "-"){
      nextToken();
      if(token == "(") {
         nextToken();
         express();
         nextToken();
         if(token != ")") {
            processError("expected \")\" after (...");
         }
         nextToken();
      } else if(isBoolean(token)) {
         //TODO am I supposed to have a code here or something?
         //or just return
         nextToken();
      } else if(isNonKeyId(token)) {

         //TODO am I supposed to have a code here or something?
         //or just return
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
