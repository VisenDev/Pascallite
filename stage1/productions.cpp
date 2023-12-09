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
   pushOperator(":=");
   pushOperand(token);
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
   cout << "[express  ]   " << token << endl;
   
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
   cout << "[expresses]   " << token << endl;
   if (isRelOp(token)
   ){
      pushOperator(token);
      nextToken();
      cout << "           " << token << endl;
      term();
      code(popOperator(), popOperand(), popOperand());
      nextToken();
      cout << "           " << token << endl;
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
    cout << "[term     ]   " << token << endl;
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
   cout << "[terms    ]   " << token << endl;
   if(isAddLevelOp(token)){
      pushOperator(token);
      nextToken();
      cout << "           " << token << endl;
      factor();
      code(popOperator(), popOperand(), popOperand());
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
   cout << "[factor   ]   " << token << endl;
   if(token != "not"
      and token != "true"
      and token != "false"
      and token != "("
      and token != "+"
      and token != "-"
      and !isInteger(token)
      and !isNonKeyId(token)
   ){
      processError("[factor] expected true, false, (, +, -, INTEGER, or NON_KEY_ID, found \"" + token + "\"");
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
   cout << "[factors  ]   " << token << endl;
   if(isMultLevelOp(token)){
      pushOperator(token);
      nextToken();
      cout << "              " << token << endl;
      part();
      code(popOperator(), popOperand(), popOperand());
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
   cout << "[part     ]   " << token << endl;
   
   //'not' ( '(' EXPRESS ')' code('not',popOperand)  | 
   //BOOLEANx pushOperand(not x; i.e., 'true' or 'false')  |
   //NON_KEY_IDx code('not',x) )
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
      } else if(isBoolean(token)) {
         pushOperand(token == "true" ? "false" : "true" );
      } else if(isNonKeyId(token)) {
         code("not", token);
      } else {
         processError("[part] expected (, BOOLEAN, or NON_KEY_ID after \"+\"");
      }

   //'+' ( '(' EXPRESS ')' |
   //( INTEGERx | NON_KEY_IDx ) pushOperand(x) ) 
   } else if(token == "+"){
      nextToken();
      if(token == "(") {
         nextToken();
         express();
         nextToken();
         if(token != ")") {
            processError("expected \")\" after (...");
         }
      } else if(isInteger(token) or isNonKeyId(token)) {
         pushOperand(token);
      } else {
         processError("[part] expected INTEGER or NON_KEY_ID after \"+\", found \"" + token + "\"");
      }  

   //'-' ( '(' EXPRESS ')' code('neg',popOperand) |
   //INTEGERx pushOperand('-'+ x) |
   //NON_KEY_IDx code('neg',x) )
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
      } else if (isInteger(token)) {
         pushOperand("- " + token);
      } else if (isNonKeyId(token)) {
         code("neg", token);
      } else {
            processError("[part] INTEGER, \"(\", or NON_KEY_ID after \"-\"");
      }

   //'(' EXPRESS ')'
   } else if (token == "(") {
      nextToken();
      express();
      if(token != ")") {
         processError("expected \")\" after (...");
      }
   } else if(isInteger(token)){
      pushOperand(token);
   } else if(isBoolean(token)) {
      pushOperand(token);
   } else if(isNonKeyId(token)) {
      pushOperand(token);
   } else {
      processError("[part] expected: not, +, -, (, INTEGER, BOOLEAN, NON_KEY_ID, found\"" + token + "\"");
   }

   //get the next token
   nextToken();
   cout << "              " << token << endl;
}
