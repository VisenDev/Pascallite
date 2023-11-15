#include "stage0.h"
#include "utest.h"

char** argv_copy;

int main(int argc, char **argv)
{
   // This program is the stage0 compiler for Pascallite.  It will accept
   // input from argv[1], generate a listing to argv[2], and write object 
   // code to argv[3].
   
   //====INITIATES UNIT TESTING========
   argv_copy = argv;
   return utest_main(argc, argv);
   //remove this for normal main
   //==================================
    

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
}


//=======NEEDED FOR TESTING
UTEST_STATE();

//========TESTING=========
UTEST(helper_functions, isKeyword) {
   Compiler myCompiler(argv_copy);
   
   UTEST_TRUE( myCompiler.isKeyword("begin"), "begin is valid keyword", false);
   UTEST_TRUE( myCompiler.isKeyword("end"), "end is valid keyword", false);
   UTEST_TRUE( myCompiler.isKeyword("const"), "const is a valid keyword", false);
   UTEST_TRUE( myCompiler.isKeyword("var"), "var is a valid keyword", false);
   
   UTEST_FALSE( myCompiler.isKeyword("consts"), "fake keyword", false);
   UTEST_FALSE( myCompiler.isKeyword("vars"), "fake keyword", false);
   UTEST_FALSE( myCompiler.isKeyword("foo"), "fake keyword", false);
   UTEST_FALSE( myCompiler.isKeyword("foobar"), "fake keyword", false);
}

UTEST(helper_functions, isBoolean) {
    Compiler myCompiler(argv_copy);
   
    UTEST_TRUE(myCompiler.isBoolean("true"), "\"true\" is a boolean", false);
    UTEST_TRUE(myCompiler.isBoolean("false"), "\"false\" is a boolean", false);

    UTEST_FALSE(myCompiler.isBoolean("1"), "\"1\" is not a boolean", false);
    UTEST_FALSE(myCompiler.isBoolean("0"), "\"0\" is not a boolean", false);
    UTEST_FALSE(myCompiler.isBoolean("hello"), "\"hello\" is not a boolean", false);
    UTEST_FALSE(myCompiler.isBoolean("TRUE"), "\"TRUE\" is not a boolean", false);
    UTEST_FALSE(myCompiler.isBoolean("FALSE"), "\"FALSE\" is not a boolean", false);
}
