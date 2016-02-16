#include "wordlist.h"
#include <iostream>
WordList::WordList (){
    max_wordsize = 0;
}
WordList::~WordList(){
    
}

int WordList::AddWord(Word word) {
    int result;
    if (word.GetValue().length() > max_wordsize){
        max_wordsize = (int)word.GetValue().length();
    }

    for(int i = 0; i <list.size(); i++){
        result = list[i].IsEqual(word.GetValue());
        if(result == 0){
            list[i].BumpCount();
            return 0;
        }else if (result <0){
            list.insert(list.begin() +i , word);
            return 0;
        }
    }

    list.push_back(word);
    return 0;
}

int WordList::PrintSizedWords(){
    for (int i = 1; i <=max_wordsize; i++){
        std::cout << "words of size " << i << std::endl;
        for(int j = 0; j<list.size();j++){
            if (list[j].GetValue().length() == i){
                std::cout << list[j].GetValue() << " - " <<list[j].GetCount() << std::endl;
            }
        }
    }
    return 0;
}
/*
Word WordList::GetFirstWord(){
    
}
Word WordList::GetLastWord(){
    
}*/
