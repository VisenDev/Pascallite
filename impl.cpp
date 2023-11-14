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
   //TODO add date and time of day
   this->listingFile << "STAGE0: Robert Burnett, Mattias Bartonette" << std::endl;
   this->listingFile << "LINE NO: " << "SOURCE STATEMENT" << std::endl;
}

void Compiler::parser()
{
   this->ch = this->nextChar();
   //ch must be initialized to the first character of the source file
   if (this->nextToken() != "program") {
      processError("keyword \"program\" expected");
      //a call to nextToken() has two effects
      // (1) the variable, token, is assigned the value of the next token
      // (2) the next token is read from the source file in order to make
      // the assignment. The value returned by nextToken() is also
      // the next token.
      this->prog();
      //parser implements the grammar rules, calling first rule
   }
}

void Compiler::createListingTrailer()
{
   this->listingFile << "COMPILATION TERMINATED"  << "# ERRORS ENCOUNTERED" << std::endl;
}

void Compiler::processError(string err)
{
 //Output err to listingFile
 this->listingFile << err << std::endl;
 exit(EXIT_FAILURE);
}

