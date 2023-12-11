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
   //cout << "[assignSmt]   " << token << endl;
   if(!isNonKeyId(token)) {
      processError("expected NON_KEY_ID in assignStmt");
   }
   const auto lhs = token;
   nextToken();
   //cout << "             " << token << endl;
   if(token != ":=") {
      processError("expected \":=\" after NON_KEY_ID in assignStmt");
   }
   pushOperator(":=");
   pushOperand(lhs);
   nextToken();
   //cout << "              " << token << endl;
   express();
   //nextToken();
   //cout << "              " << token << endl;
   if(token != ";") {
      processError("expected \";\" after assignStmt");
   }
   
   //cout << "    assignStmt";
   const auto my_operator = popOperator();
   const auto operand1 = popOperand();
   const auto operand2 = popOperand();
   code(my_operator, operand1, operand2);
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
   code("read", csv);
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
   //emitWriteCode(csv);
   code("write", csv);
}


void Compiler::express(){
   //cout << "[express  ]   " << token << endl;

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
         or token == ">="
         or token == "<"
         or token == ">"
         );
}

void Compiler::expresses(){
   //cout << "[expresses]   " << token << endl;
   if (isRelOp(token)
      ){
      pushOperator(token);
      nextToken();
      //cout << "           " << token << endl;
      term();
      const auto my_operator = popOperator();
      const auto operand1 = popOperand();
      const auto operand2 = popOperand();
      code(my_operator, operand1, operand2);

      //nextToken();
      //cout << "           " << token << endl;
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
         or token == "+"
         or token == "-"
         );
}

void Compiler::term(){
   //cout << "[term     ]   " << token << endl;
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
   //cout << "[terms    ]   " << token << endl;
   if(isAddLevelOp(token)){
      pushOperator(token);
      nextToken();
      //cout << "              " << token << endl;
      factor();

      //cout << "    terms";
      const auto my_operator = popOperator();
      const auto operand1 = popOperand();
      const auto operand2 = popOperand();
      code(my_operator, operand1, operand2);
      terms();
   } else if (
         token == "<>"
         or token == "="
         or token == "<="
         or token == ">="
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
   //cout << "[factor   ]   " << token << endl;
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
   //cout << "[factors  ]   " << token << endl;
   if(isMultLevelOp(token)){
      pushOperator(token);
      nextToken();
      //cout << "              " << token << endl;
      part();

      //cout << "    factors";
      const auto my_operator = popOperator();
      const auto operand1 = popOperand();
      const auto operand2 = popOperand();
      code(my_operator, operand1, operand2);

      factors();
      
   } else if (token == "<>"
         or token == "="
         or token == "<="
         or token == ">="
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
      processError("[factors] expected ADD_LEVEL_OP, MULT_LEVEL_OP, REL_OP, \")\", or \";\", found " + token);
   }
}      

void Compiler::part(){
   //cout << "[part     ]   " << token << endl;

   //'not' ( '(' EXPRESS ')' code('not',popOperand)  | 
   //BOOLEANx pushOperand(not x; i.e., 'true' or 'false')  |
   //NON_KEY_IDx code('not',x) )
   if(token == "not"){
      nextToken();
      //cout << "              " << token << endl;

      if(token == "(") {

         nextToken();
         //cout << "              " << token << endl;
         express();
         //    nextToken();
         if(token != ")") {
            processError("expected \")\" after (...");
         }
         //cout << "    part not";
         code("not", popOperand());
      } else if(isBoolean(token)) {
         pushOperand(token == "true" ? "false" : "true" );
      } else if(isNonKeyId(token)) {
         //cout << "and";
         code("not", token);
      } else {
         processError("[part] expected (, BOOLEAN, or NON_KEY_ID after \"not\"");
      }

      //'+' ( '(' EXPRESS ')' |
      //( INTEGERx | NON_KEY_IDx ) pushOperand(x) ) 
   } else if(token == "+"){
      nextToken();
      //cout << "              " << token << endl;
      if(token == "(") {
         nextToken();
         //cout << "              " << token << endl;
         express();
         if(token != ")") {
            processError("expected \")\" after (...");
         }
      } else if(isInteger(token) or isNonKeyId(token)) {
         pushOperand(token);
      } else {
         processError("[part] expected '(', INTEGER, or NON_KEY_ID after \"+\", found \"" + token + "\"");
      }  

      //'-' ( '(' EXPRESS ')' code('neg',popOperand) |
      //INTEGERx pushOperand('-'+ x) |
      //NON_KEY_IDx code('neg',x) )
   } else if(token == "-"){
      nextToken();
      //cout << "              " << token << endl;

      if(token == "(") {
         nextToken();
         //cout << "              " << token << endl;
         express();
         if(token != ")") {
            processError("expected \")\" after (...");
         }
         //cout << "    part neg";
         code("neg", popOperand());
      } else if (isInteger(token)) {
         pushOperand("- " + token);
      } else if (isNonKeyId(token)) {
         //cout << "    part neg keyid";
         code("neg", token);
      } else {
         processError("[part] INTEGER, \"(\", or NON_KEY_ID after \"-\"");
      }

      //'(' EXPRESS ')'
   } else if (token == "(") {
      nextToken();
      //cout << "              " << token << endl;
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
   //cout << "              " << token << endl;
}
