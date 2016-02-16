#include <vector>

#include "word.h"
class WordList{
public:
    WordList();
    ~WordList();
    int AddWord(Word word);
    Word GetFirstWord();
    Word GetLastWord();
    int PrintSizedWords();
    
private:
    vector<Word>list;
    int max_wordsize;
};