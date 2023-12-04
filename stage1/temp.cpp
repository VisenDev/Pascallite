#include "stage1.h"
#include <string>

void Compiler::freeTemp(){
   currentTempNo -= 1;
   if(currentTempNo < -1) {
      processError("Compiler Internal Error - currentTempNo should be >= -1");
   }
}

string Compiler::getTemp(){
   string temp;
   currentTempNo += 1;
   temp = "T" + to_string(currentTempNo);
   if(currentTempNo > maxTempNo) {
      insert(temp, UNKNOWN, VARIABLE, "", NO, 1);
      maxTempNo += 1;
   }
   return temp;
   
}
string Compiler::getLabel(){
   static int i = -1;
   i++;
   return to_string(i);
}

bool Compiler::isTemporary(string s) const {
   return s[0] == 'T' and isInteger(s.substr(1));
}
