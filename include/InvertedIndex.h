#pragma once
#include <vector>
#include <map>
#include <string>
#include <cstddef>

/**
 * структура для хранения идентификатора текстового документа и количества вхождений слов в тектовый документ
 */
struct Entry {
    size_t doc_id, count;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const ;
};

/**
 * подсчет количества вхождений каждого слова из конкретного текстового документа в этот документ
 * @param freq_dictionary - частотный словарь
 * @param word - слово из текстового документа
 * @param docID - идетнификатор текстового документа
 */
void calculateCountOfWord (std::map <std::string, std::vector<Entry>>& freq_dictionary, std::string& word, size_t docID);

class InvertedIndex {
private:
    std::vector<std::string> docs;//список содержимого документов
    std::map <std::string, std::vector<Entry>> freq_dictionary;//частотный словарь
public:
    InvertedIndex();

    /**
     * обновление или заполнение базы документов, по которым будем совершать поиск
     * @param input_docs - содержимое документов
     */
    void UpdateDocumentBase(const std::vector<std::string>& input_docs);

    /**
     * получение количества вхождений слова word в загруженной базе документов
     * @param word - слово, частоту вхождений которого необходимо определить
     * @return - возвращает подготовленный список с частотой слов
     */
    std::vector<Entry> GetWordCount(const std::string& word);
};
