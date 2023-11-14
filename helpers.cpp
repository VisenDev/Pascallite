#include "stage0.h"
#include <cctype>

// Helper functions for the Pascallite lexicon
 
bool Compiler::isKeyword(string s) const {
   auto keywords = {"begin", "end", "vars", "consts", "program", "true", "false", "not", "integer", "boolean"};
   return std::find(keywords.begin(), keywords.end(), s) != keywords.end();
}

bool Compiler::isSpecialSymbol(char c) const {
   auto symbols = { '=' , ':' , ',' , ';' , '.' , '+' , '-' };
   return std::find(symbols.begin(), symbols.end(), c) != symbols.end();
}

bool Compiler::isNonKeyId(string s) const {
   return std::isalpha(s[0]) 
      && std::find_if(s.begin(), s.end(), [](unsigned char c) { return std::isdigit(c) || std::isalpha(c); }) == s.end()
      && s[s.length()] != '_';

}

bool Compiler::isInteger(string s) const {
   return std::find_if(s.begin(), 
         s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end(); 
}
bool Compiler::isBoolean(string s) const {
   return s == "true" || s == "false";
}
bool Compiler::isLiteral(string s) const {
   return isBoolean(s) 
      || isInteger(s) 
      || (s.substr(0, 4) == "not " && isBoolean(s.substr(4, s.npos))) 
      || ((s.substr(0, 2) == "- " || s.substr(0, 2) == "+ " ) && isInteger(s.substr(2, s.npos)));
}
