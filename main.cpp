#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "tokengenerator.h"
#include "word.h"
#include "wordlist.h"

int  main()  {
    
    cout << "starting\n";
    
    TokenGenerator tg = TokenGenerator("/users/hamstache/sherlock.txt");
    
    
    
    int tokentype = tg.GetNextToken();
    
    Word wd("");
    WordList list;
    while(tokentype != TOKEN_EOF) {
        if (tg.GetTokenType() == TOKEN_IDENTIFIER || tg.GetTokenType() == TOKEN_LITERAL) {
            wd = Word(tg.GetTokenValue());
            
            list.AddWord(wd);
        }
        tokentype = tg.GetNextToken();
    }
    list.PrintSizedWords();
    
    
    exit(0);
    
    return  0;
}