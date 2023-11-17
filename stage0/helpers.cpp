#include "stage0.h"
#include <cctype>

// Helper functions for the Pascallite lexicon
 
bool Compiler::isKeyword(string s) const {
   auto keywords = {"begin", "end", "var", "const", "program", "true", "false", "not", "integer", "boolean"};
   return std::find(keywords.begin(), keywords.end(), s) != keywords.end();
}

bool Compiler::isSpecialSymbol(char c) const {
   auto symbols = { '=' , ':' , ',' , ';' , '.' , '+' , '-' };
   return std::find(symbols.begin(), symbols.end(), c) != symbols.end();
}

bool Compiler::isNonKeyId(string s) const {
	 auto result =  
      std::islower(s[0]) 
      && !isKeyword(s)
      && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !isdigit(c) and !islower(c) and c != '_'; }) == s.end()
      && s[s.length() - 1] != '_';
	
	for (uint i = 0; i < s.size(); ++i)
	{
		if ((s[i] == '_') && ((i+1) < s.size()))
			if (s[i+1] == '_') 	// it turns out this is sufficient as the allowed characters in the string is already checked to be valid
				result = 0;     //so we only need to check that '_' doesn't follow '_'
	}
	
   return result ;
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
      || ((s.substr(0, 1) == "-" || s.substr(0, 1) == "+" ) && isInteger(s.substr(2, s.npos)));
}
