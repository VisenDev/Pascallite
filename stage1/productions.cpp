#include "stage1.h"

void Compiler::execStmts(){
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
void Compiler::express(){}
void Compiler::expresses(){}      // stage 1, production 10
void Compiler::term(){}           // stage 1, production 11
void Compiler::terms(){}          // stage 1, production 12
void Compiler::factor(){}         // stage 1, production 13
void Compiler::factors(){}        // stage 1, production 14
void Compiler::part(){}           // stage 1, production 15
