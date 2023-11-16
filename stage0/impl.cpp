#include "stage0.h"
#include <cstdlib>

Compiler::Compiler(char **argv) // constructor
{
   this->sourceFile.open(argv[1]);
   this->listingFile.open(argv[2]);
   this->objectFile.open(argv[3]);
}

Compiler::~Compiler() // destructor
{
   this->sourceFile.close();
   this->listingFile.close();
   this->objectFile.close();
}

void Compiler::createListingHeader()
{
	time_t now = time (NULL);
	
   this->listingFile << left << setw(42) << "STAGE0: Robert Burnett, Matthew Barton" << ctime(&now)  << std::endl;
   this->listingFile << setw(22) << "LINE NO. " << "SOURCE STATEMENT\n";
   ++lineNo;
   listingFile << "   " << lineNo << '|';
}

void Compiler::parser()
{
   this->ch = this->nextChar();
   //ch must be initialized to the first character of the source file
   if (this->nextToken() != "program") {
      processError("keyword \"program\" expected, found " + token);
      //a call to nextToken() has two effects
      // (1) the variable, token, is assigned the value of the next token
      // (2) the next token is read from the source file in order to make
      // the assignment. The value returned by nextToken() is also
      // the next token.
      //parser implements the grammar rules, calling first rule
   }
   this->prog();
}

void Compiler::createListingTrailer()
{
   this->listingFile << "COMPILATION TERMINATED"  << "# ERRORS ENCOUNTERED" << std::endl;
}

void Compiler::processError(string err)
{
 //Output err to listingFile
 this->listingFile << err << std::endl;
 objectFile.flush();
 exit(EXIT_FAILURE);
}

