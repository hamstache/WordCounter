
#ifndef spidy_tokengenerator_h
#define spidy_tokengenerator_h

#include <string>


//  Web Input Constants
#define CONTENT_BUFFER_PAGESIZE    2000

//  Token Type Values returned by GetNextToken()
#define TOKEN_EOF           100
#define TOKEN_UNKNOWN       101
#define TOKEN_IDENTIFIER    102
#define TOKEN_NUMBER        103
#define TOKEN_LITERAL       104
#define TOKEN_SYMBOL        105
#define TOKEN_DELIMITER     106
#define TOKEN_EOL           107
#define TOKEN_KEYWORD       108

//  Character hex equivalents used to search the buffer
#define CHAR_TAB            0x09
#define CHAR_LINE_FEED      0x0A
#define CHAR_CR             0x0D
#define CHAR_SPACE          0x20
#define CHAR_DOUBLE_QUOTE   0x22
#define CHAR_SINGLE_QUOTE   0x27
#define CHAR_ASTERISK       0x2A
#define CHAR_COMMA          0x2C
#define CHAR_DASH           0x2D
#define CHAR_PERIOD         0x2E
#define CHAR_FOR_SLASH      0x2F
#define CHAR_0              0x30
#define CHAR_9              0x39
#define CHAR_LESS_THAN      0x3C
#define CHAR_EQUAL          0x3D
#define CHAR_GREATER_THAN   0x3E
#define CHAR_A              0x41
#define CHAR_Z              0X5A
#define CHAR_BACK_SLASH     0x5C
#define CHAR_UNDERBAR       0X5F
#define CHAR_LOWER_A        0x61
#define CHAR_LOWER_Z        0x7A


// internal type definitions

struct Token {
    int tokentype;
    int tokenlength;
    char *tokenstring;
};

using namespace std;

class TokenGenerator {
    
private:
    //  internal class variable definitions
    int BuffIndex;
    int BuffSize;
    char *Buffer;
    string Token;  // holds the current token value
    int TokenType;  // numerical value representing the type of token
    int LineNo;  //  count the number of lines in the input buffer
    
    //  File I/O variables
    FILE *InputFile;
    
    //  internal class functions
    
public:
    // Constructors
    TokenGenerator(string inputpath);
    // Destructor
    ~TokenGenerator();
    
    //  Public Methods
      // GetNextToken() may need a future enhancement to accept a mode parameter that would
      // cause it to retain double quotes on LITERAL types; needed for HTML parsing
    int GetNextToken ();
    int GetTokenType ();
    char GetTokenChar ();
    int GetTokenSize ();
    string GetTokenValue ();
    void PrintCurrentToken ();
    void DumpContent(char *path);

    
};

#endif
