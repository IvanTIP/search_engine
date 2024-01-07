#include <iostream>
#include <thread>
#include "convertorJSON.h"


void getTexts(ConverterJSON* converter, std::vector<std::string>& text) {//получение текста из текстовых файлов
    text = converter->GetTextDocuments();
}

void UpdateDocumentBase(InvertedIndex* ind, std::vector<std::string>& text){//обновление базы документов
    ind->UpdateDocumentBase(text);
}

void checkApp() {//проверка файла config.json
    std::ifstream file("..\\configurations_files\\config.json");
    try {
        checkConfigException(file);
    }
    catch(const std::exception& x) {
        std::cerr << "Caught exception: " << x.what() << std::endl;//если файл не открылся, выбрасываем исключение "Config file is missing!"
        system("pause");
        exit(0);
    }
    nlohmann::json dictConfig;
    file >> dictConfig;
    file.close();
    try {
        checkConfigException(dictConfig);
    }
    catch(const std::exception& x) {
        std::cerr << "Caught exception: " << x.what() << std::endl;//если поле "config" отсутствует, выбрасываем исключение "Config file is empty!"
        system("pause");
        exit(0);
    }

    try {
        checkConfigFilesException(dictConfig);
    }
    catch(const std::exception& x) {
        std::cerr << "Caught exception: " << x.what() << std::endl;//если поле "files" отсутствует, выбрасываем исключение "File paths are missing or files do not exist!"
        system("pause");
        exit(0);
    }
    std::cout << "Getting started with the app: " << dictConfig["config"]["name"] << std::endl;
}

void checkVersionOfApplication() {//проверка версии приложения
    std::ifstream CMakeListsFile("..\\CMakeLists.txt");
    if(!CMakeListsFile.is_open()) {
        std::cerr << "CMakeLists.txt file error" << std::endl;
        system("pause");
        exit(0);

    }
    std::string strVers = "VERSION";
    while(!CMakeListsFile.eof()){//ищем строчку, где указывается версия приложения в CMakeLists.txt и записываем версю в переменную strVers
        CMakeListsFile >> strVers;
        if(strVers == "VERSION") {
            CMakeListsFile >> strVers;
            break;
        }
    }
    CMakeListsFile.close();

    std::string version;
    for(char ch : strVers) {
        if(ch >= '0' && ch <= '9' || ch == '.') {//записываем численное значение версии приложения исключая ненужные символы
            version += ch;
        } else {
            break;
        }
    }
    nlohmann::json dict;
    std::ifstream configFile("..\\configurations_files\\config.json");
    if(!configFile.is_open()) {
        std::cerr << "config.json file error" << std::endl;
        system("pause");
        exit(0);
    }
    configFile >> dict;
    configFile.close();
    if(version != dict["config"]["version"]) {//сравниваем версию в CMakeLists и в config.json
        std::cerr << "config.json has incorrect file" << std::endl;
        system("pause");
        exit(0);
    }
}

int main(){

    checkApp();//проверяем поля файла config.json и выводим сообщение о начале работы
    checkVersionOfApplication();//проверка версии приложения в CMakeList и в config.json
    auto* converter = new ConverterJSON;
    std::vector<std::string> text;
    std::vector<std::string> requests;
    std::vector<std::vector<std::pair<int, float>>> answers;
    auto* ind = new InvertedIndex;
    requests = converter->GetRequests();
    std::thread getTextsThread(getTexts, converter, std::ref(text));//поток для получения всех текстов из текстовых файлов
    std::thread updateDocsTread(UpdateDocumentBase, ind, std::ref(text));//поток для обновления базы документов
    std::this_thread::sleep_for(std::chrono::milliseconds(300));//останов основного потока на 300 миллисекунд перед проведением расчета
    auto* server = new SearchServer(ind);
    calculateRelRelative(server, requests, answers);
    converter->putAnswers(answers);
    getTextsThread.join();
    updateDocsTread.join();
    delete converter;
    delete server;
    std::cout << "Exit from app" << std::endl;
    system("pause");

    return 0;
}