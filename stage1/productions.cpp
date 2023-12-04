#include "stage1.h"

void Compiler::execStmts(){
   nextToken();
   if (token == "end") {
      return;
   }

   if(token != "read" and token != "write" and !isNonKeyId(token)){
      processError("expected \"read\", \"write\", \"end\", or \"non key id\"");
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
      processError("expected \"read\", \"write\", or \"non key id\"");
   }
}

void Compiler::assignStmt(){}     // stage 1, production 4
                                  // 
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
   nextToken();
   if(token != ")") {
      processError("\")\" expected after \"read(...\"");
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
   nextToken();
   if(token != ")") {
      processError("\")\" expected after \"write(...\"");
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
      processError("expected true, false, (, +, -, INTEGER, or NON_KEY_ID");
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
      processError("expected REL_OP, \")\", or \";\"");
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
      processError("expected true, false, (, +, -, INTEGER, or NON_KEY_ID");
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
      processError("expected ADD_LEVEL_OP, REL_OP, \")\", or \";\"");
   }
}      
void Compiler::part(){
   //TODO implement this functions
}
