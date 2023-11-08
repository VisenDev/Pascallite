
#include "stage0.h"
#include <cctype>

string Compiler::nextToken() //returns the next token or end of file marker
{
   token = "";
   while (token == "") {
      if(ch == '{') {
         auto temp = nextChar();
         while (temp != END_OF_FILE and temp != '}') { //empty body }
         if (ch == END_OF_FILE) {
            processError("unexpected end of file");
         } else {
            nextChar();
         }
      }
   } else if(ch == '}'){
      processError("'}' cannot begin token"); 
   } else if (isspace(ch)) {
      nextChar();
   } else if (isSpecialSymbol(ch)) {
      token = ch;
      nextChar();
   } else if(islower(ch)){ 
      token = ch;
      auto temp = nextChar();
      while (islower(temp) or isdigit(ch) or ch == '_') {
         token+=ch;
      }
      if (ch == END_OF_FILE) {
         processError("unexpected end of file");
      }
   } else if (isdigit(ch)) {
      token = ch;
      while (isdigit(nextChar())) {
         token+=ch;
      }
      if (ch == END_OF_FILE) {
         processError("unexpected end of file");
      }
   } else if(ch == END_OF_FILE) {
      token = ch;
   } else{
      processError("illegal symbol");
   }
}
return token;
}


//TODO finish nextChar
char Compiler::nextChar() //returns the next character or end of file marker
{
   read in next character
      if end of file
         ch = END_OF_FILE //use a special character to designate end of file
      else
         ch = next character
            print to listing file (starting new line if necessary)
            return ch;
}
