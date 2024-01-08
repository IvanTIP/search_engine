#include "SearchServer.h"

bool RelativeIndex::operator ==(const RelativeIndex& other) const {
    return (doc_id == other.doc_id && rank == other.rank);
}

SearchServer::SearchServer(InvertedIndex* idx) {
    index = idx;
}

void sortVec(std::vector<Entry>& entryVec, size_t& maxDocId) {
    for(int i = 0;i < entryVec.size();++i) {
        for(int j = i + 1;j != entryVec.size();++j) {
            if(entryVec[i].count > entryVec[j].count) {
                std::swap(entryVec[i],entryVec[j]);
            }
        }
        if(entryVec[i].doc_id > maxDocId) {
            maxDocId = entryVec[i].doc_id;
        }
    }
}

void sortAnswerList(std::vector<std::vector<RelativeIndex>>& listAnswers) {
    for(std::vector<RelativeIndex>& RIndxVec : listAnswers) {
        for(int i = 0;i < RIndxVec.size();++i) {
            for(int j = i + 1;j != RIndxVec.size();++j) {
                if(RIndxVec[i].rank < RIndxVec[j].rank) {
                    std::swap(RIndxVec[j], RIndxVec[i]);
                }
            }
        }
    }
}

void findingWordCount (std::vector<Entry>& entryVec, size_t& maxDocId, int countRequestWord, std::vector<RelativeIndex>& RIndxVec) {
    sortVec(entryVec, maxDocId);//сортеруем в порядок возрастания вектор
    for(size_t i = 0;i <= maxDocId;++i) {
        RelativeIndex RIndx{i, 0};
        for (const Entry &entry: entryVec) {//получаем, сколько конкретное слово из запроса встречалось в каждом текстовом документе
            if (entry.doc_id == i) {
                RIndx.rank += static_cast<float>(entry.count);
                break;
            }
        }

        if (countRequestWord > 1) {//если пользовательский запрос состоит из более чем одного слова
            bool flag = true;
            for (RelativeIndex &rI: RIndxVec) {
                if (rI.doc_id == RIndx.doc_id) {
                    rI.rank += RIndx.rank;
                    flag = false;
                    break;
                }
            }

            if (flag) {
                RIndxVec.push_back(RIndx);
                float sumRank = 0;
                for (const RelativeIndex &rI: RIndxVec) {
                    sumRank += rI.rank;
                }
                if (sumRank == 0.0f) {//если ни одно слово из пользовательского запроса не было найдено в .txt файле
                    RelativeIndex newRIndx{0, 0};
                    RIndxVec.clear();
                    RIndxVec.push_back(newRIndx);
                }
            }
        } else {//если пользовательский запрос состоит из одного слова
            if(RIndx.rank == 0.0f && i == maxDocId && RIndxVec.empty()) {
                RelativeIndex newRIndx{0, 0};
                RIndxVec.clear();
                RIndxVec.push_back(newRIndx);
            } else if(RIndx.rank != 0.0f){
                RIndxVec.push_back(RIndx);
            }
        }
    }
}

void calculateRelRelative(SearchServer* server, const std::vector<std::string>& queries_input, std::vector<std::vector<std::pair<int, float>>>& answers) {
    std::vector<std::vector<RelativeIndex>> relativeIndexVector = server->search(queries_input);

    for (std::vector<RelativeIndex> &RIndxVec: relativeIndexVector) {
        float bufferAbsRank = 0.0f;
        for (RelativeIndex &RIndx: RIndxVec) {
            if (bufferAbsRank < RIndx.rank) {//находим наибольшее значение из абсолютной релевантности, записанной в relativeIndexVector
                bufferAbsRank = RIndx.rank;  //и записываем его в bufferAbsRank
            }
        }
        for (RelativeIndex &RIndx: RIndxVec) {//производим деление каждого значения абсолютной релевантности на найденное наибольшее значение абсолютной релеванности
            if(bufferAbsRank == 0.0f) break;//если пользовательский запрос не был найден в документах, то наибольшее значение абсолюьной релевантности равно нулю. Деление не производим
            RIndx.rank /= bufferAbsRank;
        }
    }

    for (std::vector<RelativeIndex> &RIndxVec: relativeIndexVector) {//конвертируем std::vector<std::vector<RelativeIndex>> relativeIndexVector
        std::vector<std::pair<int, float>> pairsVec;                 //в std::vector<std::vector<std::pair<int, float>>> answers
        for (RelativeIndex &RIndx: RIndxVec) {
            std::pair<int, float> pair(RIndx.doc_id, RIndx.rank);
            pairsVec.push_back(pair);
        }
        answers.push_back(pairsVec);
    }
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    size_t maxDocId = 0;
    std::vector<std::vector<RelativeIndex>> listAnswers;
    for(const std::string& request : queries_input) {
        std::map<std::string, std::vector<Entry>> entryMap;
        std::vector<RelativeIndex> RIndxVec;
        int countRequestWord = 0;
        std::string word;

        for(char ch : request) {//разбиваем каждый запрос на отдельные слова
            if(ch != ' ') {
                word += ch;
            } else {
                if(!entryMap.count(word)) {
                    entryMap[word] = index->GetWordCount(word);//проверяем, сколько раз встречалось слово из зарпоса в текстовых документах
                    ++countRequestWord;//увеличиваем счетчик числа слов в пользовательском запросе
                    findingWordCount(entryMap[word], maxDocId, countRequestWord, RIndxVec);
                }
                word.clear();
            }
        }
        if(!entryMap.count(word)) {
            entryMap[word] = index->GetWordCount(word);//проверяем, сколько раз встречалось слово из зарпоса в текстовых документах
            ++countRequestWord;//увеличиваем счетчик числа слов в пользовательском запросе
            findingWordCount(entryMap[word], maxDocId, countRequestWord, RIndxVec);
        }
        listAnswers.push_back(RIndxVec);
    }
    sortAnswerList(listAnswers);//сортируем listAnswers в порядке убывания
    return listAnswers;
}
