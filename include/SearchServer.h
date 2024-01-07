#pragma once
#include "InvertedIndex.h"

/**
 * структура для хранения идентификатора текстового документа и значения релеватности
 */
struct RelativeIndex{
    size_t doc_id;
    float rank;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const RelativeIndex& other) const;
};

/**
 * сортировка вектора в породке возрастания
 * @param entryVec - ссылка на вектор, содержащий идентификатор текстовых документов и количество
 * вхождений каждого слова из запроса в текстовые документы
 * @param maxDocId - ссылка на переменную с максимальным номером идентификатора текстовых документов
 */
void sortVec(std::vector<Entry>& entryVec, size_t& maxDocId);

/**
 * сортировка вектора в порядке возрастания
 * @param listAnswers - ссылка на список ответов на поисковый запрос
 */
void sortAnswerList(std::vector<std::vector<RelativeIndex>>& listAnswers);

/**
 * определение количества вхождений каждого слова из запроса в каждый текстовый документ
 * @param entryVec - ссылка на переменную, содержащую идентификатор текстовых документов и
 * количество вхождений каждого слова в текстовые документы каждого слова текстовых документов
 * @param maxDocId - ссылка на переменную с максимальным номером идентификатора текстовых документов
 * @param countRequestWord - количество слов в запросе
 * @param RIndxVec - ссылка на переменную, содержащую идентификаторы текстовых окументов и количество вхождений
 * каждого слова из запроса в текстовые документы
 */
void findingWordCount (std::vector<Entry>& entryVec, size_t& maxDocId, int countRequestWord, std::vector<RelativeIndex>& RIndxVec);

class SearchServer {
private:
    InvertedIndex* index = nullptr;
public:

    /**
     * конструктор класса SearchServer
     * @param idx - указатель на класс InvertedIndex, чтобы SearchServer мог узнать частоту слов, встречаемых в запросе
     */
    explicit SearchServer(InvertedIndex* idx);

    /**
     * обработка поисковых запросов
     * @param queries_input - ссылка на переменную с поисковыми запросами, взятыми из файла requests.json
     * @return возвращает отсортированный список релевантных ответов для заданных запросов
     */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
};

/**
 * получение значений относительной релеватности запросов по текстовым документам
 * @param server - указатель для получения данных методом обработки посиковых запросов search
 * @param queries_input - ссылка на переменную с поисковыми запросами, взятыми из файла requests.json
 * @param answers - ссылка на переменную с ответами на поисковый запрос
 */
void calculateRelRelative(SearchServer* server, const std::vector<std::string>& queries_input, std::vector<std::vector<std::pair<int, float>>>& answers);