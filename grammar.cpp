#include "stage0.h"

void Compiler::prog() //token should be "program"
{
   if (token != "program") {
      processError("keyword \"program\" expected");
      progStmt();
   } 

   if (token == "const") {
      consts();
   }

   if (token == "var") {
      vars();
   }

   if (token != "begin") {
      processError("keyword \"begin\" expected");
      beginEndStmt();

   }

   if (ch != END_OF_FILE)  {
      processError("no text may follow \"end\"");
   }
}
