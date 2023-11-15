
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
         nextChar();
         while (islower(ch) or isdigit(ch) or ch == '_') {
            token+=ch;
            nextChar();
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
      } else if(ch == END_OF_FILE || ch == EOF) {
         ch = END_OF_FILE;
         token = END_OF_FILE;
      } else{
         processError("illegal symbol, symbol \"" + string{ch} + "\" is not allowed");
      }
}
return token;
}


char Compiler::nextChar() //returns the next character or end of file marker
{
   ch = sourceFile.get();
      
   if(sourceFile.eof() or ch == EOF){
      ch = END_OF_FILE;//use a special character to designate end of file
   }
   
   //print to listing file (starting new line if necessary)
   if(ch != EOF && ch != END_OF_FILE) {
      static int line_number = 1;
      listingFile.put(ch);
      if(ch == '\n') {
         //TODO use iomanip here for proper formatting
         //Don't add line number for last line
         listingFile << "    " << line_number << "|";
         ++line_number;
      }
   }
   
   return ch;
}