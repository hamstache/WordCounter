

#ifndef word_h
#define word_h


#include <string>
using namespace std;

class Word{
public:
    Word(string value); 
    
    ~Word();
    
    int IsEqual(string value);
    int GetCount();
    string GetValue();
    void BumpCount();
    
    
    
    
private:
    int _count;
    string _word;
    string _lowerword;
    
};

#endif
