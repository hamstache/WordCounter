#include "word.h"
#include <string>
#include <stdio.h>

Word::Word(string value) {
    _word = value;
    _count = 1;

    char *tmp = new char[value.length()+1];
    strncpy(tmp, value.c_str(), value.length());
    for (int i = 0; i < value.length(); i++) {
        if (tmp[i] >= 'A' && tmp[i] <= 'Z')
            tmp[i] += 32;
    }
    _lowerword = string(tmp);
    //_lowerword = tolower(_word.front());
}

Word::~Word(){
    
    
    
}

int Word::IsEqual(string value) {
    //string lowervalue;
    //lowervalue = tolower(value.front());
    //return lowervalue.compare(_lowerword);
    return value.compare(_lowerword);
}

int Word::GetCount() {
    return _count;
}

string Word::GetValue(){
    return _lowerword;
}

void Word::BumpCount() {
    _count++;
}