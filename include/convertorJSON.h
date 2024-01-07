#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <exception>
#include <fstream>
#include "SearchServer.h"

/**
 * класс с переопределенным методом what() для выбрасывания исключения,
 * когда файл config.json не открылся
 */
class ConfigFileMissingException : public std::exception {
    const char* what() const noexcept override;
};

/**
 * класс с переопределенным методом what() для выбрасывания исключения,
 * когда в файле config.json отсутствует поле config
 */
class ConfigFileEmptyException : public std::exception {
    const char* what() const noexcept override;
};

/**
 * класс с переопределенным методом what() для выбрасывания исключения,
 * когда в файле config.json отсутствует поле files
 */
class ConfigFileEmptyFieldException : public std::exception {
    const char* what() const noexcept override;
};

/**
 * функция для выбрасывания исключения в случае не открытия файла config.json
 * @param fileJSON - ссылка на переменную класса для чтения файла config.json
 */
void checkConfigException(std::ifstream& fileJSON);

/**
 * перегруженная функция для выбрасывания исключения, когда в файле config.json отсутствует пеле config
 * @param dict - ссылка на переменную для работы с JSON-форматом данных
 */
void checkConfigException(nlohmann::json& dict);

/**
 * функция для выбрасывания исключения в случае отсутствия в файле config.json поля files
 * @param dict - ссылка на переменную для работы с JSON-форматом данных
 */
void checkConfigFilesException(nlohmann::json& dict);

/**
 * Класс для работы с JSON-файлами
 */
class ConverterJSON {
private:
    std::vector<std::string> vecConfig;//вектор содержимого текстовых файлов
    std::vector<std::string> vecRequests;//список пользовательских запросов
    int responses;//максимальное количество ответов
    nlohmann::json dictConfig;//переменная для работы с JSON-форматом данных из файла config.json
    nlohmann::json dictRequests;//переменная для работы с JSON-форматом данных из файла requests.json
    nlohmann::json dictAnswers;//переменная для работы с JSON-форматом данных из файла answers.json
public:
    ConverterJSON();

    /**
     * получение содержимого файлов
     * @return возвращает список с содержимым перечисленных в config.json
     */
    std::vector<std::string> GetTextDocuments();

    /**
     * получение максимального количества ответов
     * @return возвращает максимальное количество ответов
     */
    int GetResponsesLimit();

    /**
     * получение запросов из файла requests.json
     * @return возвращает список запросов из файла requests.json
     */
    std::vector<std::string> GetRequests();

    /**
     * запись в файл answers.json результатов поисковых запросов
     * @param answers - результат поискового запроса
     */
    void putAnswers(std::vector<std::vector<std::pair<int, float>>>& answers);
};
