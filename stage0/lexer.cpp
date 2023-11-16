
#include "stage0.h"
#include <cctype>
static bool isEnd = 0;

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
	if (token == "end")
		isEnd = 1;
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
      listingFile.put(ch);
      if((ch == '\n') && !isEnd) {
         //TODO Don't add line number for last line
         ++lineNo;
         listingFile << right << setw(4) << lineNo << "|";
      }
   }

   return ch;
}
