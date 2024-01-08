#include "convertorJSON.h"

const char* ConfigFileMissingException::what() const noexcept{
    return "Config file is missing!";
}

const char* ConfigFileEmptyException::what() const noexcept{
    return "Config file is empty!";
}

const char* ConfigFileEmptyFieldException::what() const noexcept{
    return "File paths are missing or files do not exist!";
}

/**
 *выбрасываем исключение "Config file is missing!" если файл не открылся
 * @param fileJSON - переменная для чтения файла config.json
 */
void checkConfigException(std::ifstream& fileJSON) {
    if (!fileJSON.is_open()) {
        throw ConfigFileMissingException();
    }
}

/**
 * выбрасываем исключение "Config file is empty!" если поле "config" отсутствует
 * @param dict - переменная с содержимым файла config.json
 */
void checkConfigException(nlohmann::json& dict) {
    if (dict["config"] == nullptr) {
        throw ConfigFileEmptyException();
    }
}

/**
 * выбрасываем исключение "File paths are missing or files do not exist!" если поле "files" отсутствует
 * @param dict - переменная с содержимым файла config.json
 */
void checkConfigFilesException(nlohmann::json& dict) {
    if (dict["files"] == nullptr) {
        throw ConfigFileEmptyFieldException();
    }
}

ConverterJSON::ConverterJSON() {
    responses = 0;
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::ifstream fileJSON("..\\..\\configurations_files\\config.json");
    try {
        checkConfigException(fileJSON);
    }
    catch(const std::exception& x) {
        std::cerr << "Caught exception: " << x.what() << std::endl;//если файл не открылся, выбрасываем исключение "Config file is missing!"
        system("pause");
        exit(0);
    }
    fileJSON >> dictConfig;
    fileJSON.close();
    for (const auto& way : dictConfig["files"]) {
        if(way.empty()) break;
        std::ifstream fileTXT(way);//открываем файлы .txt по путям из config.json
        if (!fileTXT.is_open()) {//если по этому пути файл не существует, выводится об этом сообщение и считывается следующий путь
            std::cout << "Way: " << way << std::endl;
            std::cout << "File missing or incorrect path!" << std::endl;
            continue;
        }
        std::string text;
        std::getline(fileTXT, text); //считываем по очереди из каждого файла .txt весь текст в переменную text
        vecConfig.push_back(text);
        fileTXT.close();
    }
    if(vecConfig.empty()) {
        std::cerr << "All the specified paths are not correct" << std::endl;
        system("pause");
        exit(0);
    }
    return vecConfig;
}

int ConverterJSON::GetResponsesLimit() {
    std::ifstream file("..\\..\\configurations_files\\config.json");
    try {
        checkConfigException(file);
    }
    catch(const std::exception& x) {
        std::cerr << "Caught exception: " << x.what() << std::endl;//если файл не открылся, выбрасываем исключение "Config file is missing!"
        system("pause");
        exit(0);
    }

    file >> dictConfig;
    file.close();
    if(dictConfig["config"]["max_responses"] == nullptr) {//если поле max_responses в config.json отсутствует, то берем значение 5
        return 5;
    }
    responses = dictConfig["config"]["max_responses"];//считываем значение поле max_responses
    return responses;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream file("..\\..\\configurations_files\\requests.json");
    if (!file.is_open()) {
        std::cerr << "requests.json file error!" << std::endl;
        system("pause");
        exit(0);
    }
    file >> dictRequests;
    file.close();
    for (const auto& request : dictRequests["requests"]) {
        vecRequests.push_back(request);                    //получаем и записываем все пользовательские запросы в вектор vecRequests
    }
    return vecRequests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>>& answers) {//записываем результат работы поискового движка
    std::ofstream file("..\\..\\configurations_files\\answers.json");
    if (!file.is_open()) {
        std::cerr << "answers.json file error!" << std::endl;
        system("pause");
        exit(0);
    }

    for(int i = 0;i < answers.size();++i) {
        std::string requestsNum = "request" + std::to_string(i + 1);//записываем номер запроса
        if (answers[i].size() > 1) {                                    //если на один запрос нашлись ответы в нескольких документах
            dictAnswers["answers"][requestsNum]["result"] = "true";     //выполняем это условие
            for(int j = 0;j < answers[i].size();++j){
                if(j == GetResponsesLimit()) break;
                if(answers[i][j].second != 0.0f) {
                    dictAnswers["answers"][requestsNum]["relevance"][j]["docid"] = answers[i][j].first;//запись всех
                    dictAnswers["answers"][requestsNum]["relevance"][j]["rank"] = answers[i][j].second;//положительных ответов
                }
            }
        } else if (answers[i].size() == 1) {  //если на один запрос ответ нашелся только в одном документе, выполняем это условие
            if(answers[i][0].second != 0.0f) {//если запрос нашел ответ в каком-то документе, то переменная rank будет отлично от нуля, выполняем это условие
                dictAnswers["answers"][requestsNum]["result"] = "true";
                dictAnswers["answers"][requestsNum]["docid"] = answers[i][0].first;
                dictAnswers["answers"][requestsNum]["rank"] = answers[i][0].second;
            } else {//если запрос не нашел ответ в документах, то переменная rank будет равна нулю, выполняем это условие
                dictAnswers["answers"][requestsNum]["result"] = "false";
            }
        }
    }
    file << dictAnswers.dump(3);
    file.close();
}
