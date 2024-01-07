#include "InvertedIndex.h"

InvertedIndex::InvertedIndex() = default;

bool Entry::operator ==(const Entry& other) const {
    return (doc_id == other.doc_id && count == other.count);
}

void calculateCountOfWord (std::map <std::string, std::vector<Entry>>& freq_dictionary, std::string& word, size_t docID) {
    if(freq_dictionary.count(word)){//если в словаре freq_dictionary уже есть слово, записанное в переменной word, выполняем цикл
        for(Entry& entry: freq_dictionary[word]){
            if(entry.doc_id == docID) {
                ++entry.count;
                return;
            }
        }
    }
    Entry entry{docID,1};//в противном случае говорим, что это первое слово и записывапем его в freq_dictionary
    freq_dictionary[word].push_back(entry);//к ключу word записываем значение entry
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    while(true) {
        if(!input_docs.empty()) {//ничего не выполняется пока не загрузится хотя бы один текст в вектор input_docs
            for (int i = 0;i < input_docs.size();++i) {
                std::string word;
                for (int j = 0;j < input_docs[i].size();++j) {
                    if (input_docs[i][j] != ' ') {
                        word += input_docs[i][j];//разбиваем текст на отдельные слова и по буквам записываем в переменную word
                    } else {
                        if (!word.empty()) {
                            docs.push_back(word);//каждое слово отдельно заносим в вектор docs
                            calculateCountOfWord (freq_dictionary, word, i);//подсчитываем сколько раз слово встречается в том или ином текстовом файле
                            word.clear();
                        }
                    }
                }
                docs.push_back(word);
                calculateCountOfWord (freq_dictionary, word, i);//подсчитываем сколько раз слово встречается в том или ином текстовом файле
            }
            return;
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    std::vector<Entry> entryVec;
    if (freq_dictionary.count(word)) {               //если слово содержится в freq_dictionary,
        for(Entry& entry : freq_dictionary[word]) { //то в вектор entryVec добавляем значения ключа word,
            entryVec.push_back(entry);              //где указано сколько раз слово встречалось в том или ином документе
        }
    } else {
        Entry entry{0,0};
        entryVec.push_back(entry);
    }
    return entryVec;
}