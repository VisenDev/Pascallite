#include "stage0.h"
#include "utest.h"

char** argv_copy;

int main(int argc, char **argv)
{
   // This program is the stage0 compiler for Pascallite.  It will accept
   // input from argv[1], generate a listing to argv[2], and write object 
   // code to argv[3].

   if (argc != 4)         // Check to see if pgm was invoked correctly
   {
      // No; print error msg and terminate program
      cerr << "Usage:  " << argv[0] << " SourceFileName ListingFileName "
         << "ObjectFileName" << endl;
      exit(EXIT_FAILURE);
   }

   Compiler myCompiler(argv);

   myCompiler.createListingHeader();
   myCompiler.parser();
   myCompiler.createListingTrailer();

   
   //====INITIATES UNIT TESTING
   argv_copy = argv;
   return utest_main(argc, argv);
}


//=======NEEDED FOR TESTING
UTEST_STATE();

//========TESTING=========
UTEST(helper, isKeyword) {
   Compiler myCompiler(argv_copy);
   
   UTEST_TRUE( myCompiler.isKeyword("begin"), "begin is keyword", true);
   UTEST_TRUE( myCompiler.isKeyword("end"), "end is keyword", true);
   UTEST_TRUE( myCompiler.isKeyword("const"), "const is keyword", true);
   UTEST_TRUE( myCompiler.isKeyword("var"), "const is keyword", true);
   
   UTEST_FALSE( myCompiler.isKeyword("consts"), "fake keyword", true);
   UTEST_FALSE( myCompiler.isKeyword("vars"), "fake keyword", true);
   UTEST_FALSE( myCompiler.isKeyword("foo"), "fake keyword", true);
   UTEST_FALSE( myCompiler.isKeyword("foobar"), "fake keyword", true);
}

