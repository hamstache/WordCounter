
#include <iostream>
#include "tokengenerator.h"


//  TokenGenerator constuctor definitions
TokenGenerator::TokenGenerator (string inputpath) {
    
    //  Initialize some values
    BuffIndex = 0;  // used to point to the current position in the content buffer
    BuffSize = 0;
    Buffer = NULL;
    LineNo = 0;
    
    
    FILE *infile;
    long fsize;
    size_t fresult;
    
    infile = fopen(inputpath.c_str(), "r");
    if (infile == NULL) {
        std::cout << "Error opening file: " << inputpath << "\n";
        exit(-1);
    }
    
    //  Get the file size
    fseek(infile, 0, SEEK_END);
    fsize = ftell(infile);
    rewind(infile);
    
    //  allocate the buffer to get the file data
    Buffer = (char *)malloc(sizeof(char)*fsize);
    if (Buffer == NULL) {
        std::cout << "Unable to allocate buffer\n";
        exit(-1);
    }
    
    BuffSize = (int)fsize;
    
    fresult = fread(Buffer, 1, fsize, infile);
    if (fresult != fsize) {
        std::cout << "Error reading file into memory\n";
        exit(-1);
    }
    
    fclose(infile);
    
}

//  TokenGenerator destructor
TokenGenerator::~TokenGenerator(void) {
    
    free(Buffer);

}

//
//  GetNextToken
//
//  Parameters:  None
//
//  Identify and return the next token in the buffer
int TokenGenerator::GetNextToken () {
    
    short periodcnt = 0;
    int ch;
    
	// if (TokenPushBack) {
	//	TokenPushBack = false;
	//	return PushBackTokenType;
	// }
   
    //  reset the token until we find something
	TokenType = TOKEN_UNKNOWN;
    Token = "\0";
    
    if (BuffIndex >= BuffSize) {
        TokenType = TOKEN_EOF;
        return TokenType;
    }
    
	while (BuffIndex < BuffSize) {        
		ch = Buffer[BuffIndex];
		//  count line numbers first
		if (ch == CHAR_LINE_FEED) {
            BuffIndex++;  // go ahead and skip the LF in the input buffer
            LineNo++;     // count the LF as another line in the input buffer
			if (TokenType == TOKEN_LITERAL) {
				Token.append(" ");  // replace a line feed with white space and continue
				continue;
			}
            
			if (Token.size() == 0) {
                // we just hit a line feed with no current token identified; must be a blank line only
                // if we are not already mid-stream on a token, ignore the EOL (treat like white space)
                if (TokenType == TOKEN_UNKNOWN)
                    continue;
                TokenType = TOKEN_EOL;
            }
			return TokenType;
		}
        
		//  if token already started, then keep going until we hit
		//  a delimiter of some sort
		if (TokenType != TOKEN_UNKNOWN) {
			if (ch == CHAR_TAB || ch == CHAR_CR || ch == CHAR_SPACE) {
                BuffIndex++;   //  skip the white space no matter what
                if (TokenType == TOKEN_LITERAL) {
                    Token.append(" ");
                    continue;
                }
                
                //  thought it was going to be a number starting with a '.' or a '-', but
                //  it's just a '.' or '-' symbol by itself; change the token type
				if (TokenType == TOKEN_NUMBER && Token.size() == 1 &&
					(Token.compare(".") == 0 || Token.compare("-") == 0)) {
                    TokenType = TOKEN_SYMBOL;
				}
                
                // now get out; token is done
                return TokenType;
			}

            /*if (ch == CHAR_DOUBLE_QUOTE) {
                if (TokenType == TOKEN_LITERAL) {
                    //  bump BuffIndex only if it is a literal.  Anything else
                    //  and we are done.  The next token will be started as a 
                    //  literal
                    BuffIndex++;
                    
                    //  when parsing html pages, instances were found with 2 double
                    //  quotes at the end of a literal (poor aol.com element attributes)
                    //  check for this scenario and skip the 2nd quote if it appears
                    if (Buffer[BuffIndex] == '"')
                        BuffIndex++;
                }
                
                if (TokenType == TOKEN_NUMBER && Token.size() == 1 &&
                    (Token.compare(".") == 0 || Token.compare("-") == 0)) {
                    TokenType = TOKEN_SYMBOL;
                }
                return TokenType;
            }*/
                        
            if ((ch >= CHAR_A && ch <= CHAR_Z) || (ch >= CHAR_LOWER_A && ch <= CHAR_LOWER_Z)) {
                if (TokenType == TOKEN_LITERAL || TokenType == TOKEN_IDENTIFIER) {
                    Token.append(&Buffer[BuffIndex++], 1);
                    continue;
                }
                
                // must be a number, we are done
                if (Token.size() == 1 && (Token.compare(".") == 0 || Token.compare("-") == 0)) {
                    TokenType = TOKEN_SYMBOL;
                }
                return TokenType;
            }
                    
            if (ch >= CHAR_0 && ch <= CHAR_9) {
                //  can only be a literal, identifier, or number, append it
                Token.append(&Buffer[BuffIndex++], 1);
                continue;
            }

            if (ch == CHAR_PERIOD) {
                if (TokenType == TOKEN_NUMBER && periodcnt == 0) {
                    Token.append(&Buffer[BuffIndex++], 1);
                    periodcnt++;
                    continue;
                }
                                    
                if (TokenType == TOKEN_LITERAL) {
                    Token.append(&Buffer[BuffIndex++], 1);
                    continue;
                }
                
                // end of identifier or second period in number
                if (TokenType == TOKEN_NUMBER && Token.size() == 1 && 
                    (Token.compare(".") == 0 || Token.compare("-") == 0)) {
                    TokenType = TOKEN_SYMBOL;
                }
                    
                return TokenType;
            }

            if (ch == CHAR_COMMA) {
                if (TokenType == TOKEN_LITERAL) {
                    Token.append(&Buffer[BuffIndex++], 1);
                    continue;
                }

                // end of number or identifier
                if (TokenType == TOKEN_NUMBER && Token.size() == 1 && 
                    (Token.compare(".") == 0 || Token.compare("-") == 0)) {
                    TokenType = TOKEN_SYMBOL;
                }
                
                return TokenType;
            }
            
            // must be some other symbol, unless token is a literal, we are done
            if (TokenType == TOKEN_LITERAL) {
                Token.append(&Buffer[BuffIndex++], 1);
                continue;
            }

            if (TokenType == TOKEN_NUMBER && Token.size() == 1 && 
                (Token.compare(".") == 0 || Token.compare("-") == 0)) {
                TokenType = TOKEN_SYMBOL;
            }
                                        
            return TokenType;
        }

		//  have not yet started a recoginzed token; take a stab at what it might be
        if (ch == CHAR_TAB || ch == CHAR_LINE_FEED || ch == CHAR_CR || ch == CHAR_SPACE) {
				BuffIndex++;
				continue;  //  skip white space
        }

        //if (ch == CHAR_DOUBLE_QUOTE) {
        //    TokenType = TOKEN_LITERAL;
        //    BuffIndex++;
        //}
        //else
        if ((ch >= CHAR_A && ch <= CHAR_Z) || (ch >= CHAR_LOWER_A && ch <= CHAR_LOWER_Z)) {
            TokenType = TOKEN_IDENTIFIER;
            Token.append(&Buffer[BuffIndex++], 1);
        }
        else
        if ((ch >= CHAR_0 && ch <= CHAR_9) || ch == CHAR_PERIOD || ch == CHAR_DASH) {
            TokenType = TOKEN_NUMBER;
            Token.append(&Buffer[BuffIndex++], 1);
            if (ch == CHAR_PERIOD)
                periodcnt++;
        }
        else
        if (ch == CHAR_COMMA) {
            TokenType = TOKEN_DELIMITER;
            Token.append(&Buffer[BuffIndex++], 1);
            return TokenType;
        }
        else {
            TokenType = TOKEN_SYMBOL;
            Token.append(&Buffer[BuffIndex++], 1);
            return TokenType;
        }
    }
        
	return TokenType;

}

int TokenGenerator::GetTokenType() {
    
    return TokenType;
}

char TokenGenerator::GetTokenChar() {
    
    char tmp = Token.at(0);
    return tmp;
}

int TokenGenerator::GetTokenSize () {
    
    return (int)Token.size();
    
}

//  return a copy of the token value.
string TokenGenerator::GetTokenValue () {
    
    return Token;
}

void TokenGenerator::PrintCurrentToken () {
    
    std::cout << "Token type = " << TokenType << "\n";
    std::cout << "Token length = " << Token.size() << "\n";
    std::cout << "Token value = " << Token << "\n\n";
}

void TokenGenerator::DumpContent (char *path) {
    
    FILE *out;
    out = fopen(path, "w");
    fwrite(Buffer, BuffSize, 1L, out);
    fclose(out);
}