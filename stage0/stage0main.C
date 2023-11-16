#include "stage0.h"
#include "utest.h"

char** argv_copy;

int main(int argc, char **argv)
{
   // This program is the stage0 compiler for Pascallite.  It will accept
   // input from argv[1], generate a listing to argv[2], and write object 
   // code to argv[3].
   
   //====INITIATES UNIT TESTING========
#ifdef TEST
   argv_copy = argv;
   return utest_main(argc, argv);
#endif
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
UTEST(helperFunctions, isKeyword) {
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

UTEST(helperFunctions, isBoolean) {
    Compiler myCompiler(argv_copy);
   
    UTEST_TRUE(myCompiler.isBoolean("true"), "\"true\" is a boolean", false);
    UTEST_TRUE(myCompiler.isBoolean("false"), "\"false\" is a boolean", false);

    UTEST_FALSE(myCompiler.isBoolean("1"), "\"1\" is not a boolean", false);
    UTEST_FALSE(myCompiler.isBoolean("0"), "\"0\" is not a boolean", false);
    UTEST_FALSE(myCompiler.isBoolean("hello"), "\"hello\" is not a boolean", false);
    UTEST_FALSE(myCompiler.isBoolean("TRUE"), "\"TRUE\" is not a boolean", false);
    UTEST_FALSE(myCompiler.isBoolean("FALSE"), "\"FALSE\" is not a boolean", false);
}

UTEST(helperFunctions, isLiteral) {
    Compiler myCompiler(argv_copy);
   
    UTEST_TRUE(myCompiler.isLiteral("1000"), "\"1000\" is a an integer literal", false);
    UTEST_TRUE(myCompiler.isLiteral("1234"), "\"1234\" is a an integer literal", false);
    UTEST_TRUE(myCompiler.isLiteral("1"), "\"1\" is a an integer literal", false);
    UTEST_TRUE(myCompiler.isLiteral("99999"), "\"99999\" is a an integer literal", false);
    UTEST_TRUE(myCompiler.isLiteral("-99"), "\"-99\" is a an integer literal", false);

    UTEST_FALSE(myCompiler.isLiteral("0x99999"), "\"0x99999\" is not a an integer literal", false);
    UTEST_FALSE(myCompiler.isLiteral("FFAA"), "\"FFAA\" is not a an integer literal", false);

}


UTEST(helperFunctions, isSpecialSymbol) {
    Compiler myCompiler(argv_copy);
   
    UTEST_TRUE(myCompiler.isSpecialSymbol('.'), "\'.\' is a special symbol", false);
    UTEST_TRUE(myCompiler.isSpecialSymbol(':'), "\'.\' is a special symbol", false);

    UTEST_FALSE(myCompiler.isSpecialSymbol('a'), "\'a\' is not a special symbol", false);
    UTEST_FALSE(myCompiler.isSpecialSymbol('Z'), "\'Z\' is not a special symbol", false);
    UTEST_FALSE(myCompiler.isSpecialSymbol('8'), "\'8\' is not a special symbol", false);
    UTEST_FALSE(myCompiler.isSpecialSymbol('$'), "\'$\' is not a special symbol", false);
}

UTEST(helperFunctions, isNonKeyId) {
    Compiler myCompiler(argv_copy);
    
    UTEST_TRUE(myCompiler.isNonKeyId("foo"), "\"foo\" is a valid indentifier", false);
    UTEST_TRUE(myCompiler.isNonKeyId("hello_world"), "\"hello_world\" is a valid indentifier", false);
    UTEST_TRUE(myCompiler.isNonKeyId("abc_123"), "\"abc_123\" is a valid indentifier", false);
    
    UTEST_FALSE(myCompiler.isNonKeyId("var"), "\"var\" is not a valid indentifier", false);
    UTEST_FALSE(myCompiler.isNonKeyId("hello_world_"), "trailing underscore is invalid", false);
    UTEST_FALSE(myCompiler.isNonKeyId("HELLO_WORLD"), "undercase only", false);
    UTEST_FALSE(myCompiler.isNonKeyId("9abc"), "cannot start with number", false);
    UTEST_FALSE(myCompiler.isNonKeyId("_abc"), "cannot start with underscore", false);
}
