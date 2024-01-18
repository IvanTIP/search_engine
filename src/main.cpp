#include <iostream>
#include <thread>
#include "convertorJSON.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPlainTextEdit>

void getTexts(ConverterJSON* converter, std::vector<std::string>& text) {//получение текста из текстовых файлов
    text = converter->GetTextDocuments();
}

void UpdateDocumentBase(InvertedIndex* ind, std::vector<std::string>& text){//обновление базы документов
    ind->UpdateDocumentBase(text);
}

void checkApp() {//проверка файла config.json
    std::ifstream file("..\\..\\search_engine\\configurations_files\\config.json");
    try {
        checkConfigException(file);
    }
    catch(const std::exception& x) {
        std::cerr << "Caught exception: " << x.what() << std::endl;//если файл не открылся, выбрасываем исключение "Config file is missing!"
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
        exit(0);
    }

    try {
        checkConfigFilesException(dictConfig);
    }
    catch(const std::exception& x) {
        std::cerr << "Caught exception: " << x.what() << std::endl;//если поле "files" отсутствует, выбрасываем исключение "File paths are missing or files do not exist!"
        exit(0);
    }
    std::cout << "Getting started with the app: " << dictConfig["config"]["name"] << std::endl;
}

void checkVersionOfApplication() {//проверка версии приложения
    std::ifstream CMakeListsFile("..\\..\\search_engine\\CMakeLists.txt");
    if(!CMakeListsFile.is_open()) {
        std::cerr << "CMakeLists.txt file error" << std::endl;
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
    std::ifstream configFile("..\\..\\search_engine\\configurations_files\\config.json");
    if(!configFile.is_open()) {
        std::cerr << "config.json file error" << std::endl;
        exit(0);
    }
    configFile >> dict;
    configFile.close();
    if(version != dict["config"]["version"]) {//сравниваем версию в CMakeLists и в config.json
        std::cerr << "config.json has incorrect file" << std::endl;
        exit(0);
    }
}

int visualApp(int argc, char** argv, std::vector<std::vector<std::pair<int, float>>>& answers){
    QApplication app(argc, argv);
    std::ifstream configFile("..\\..\\search_engine\\configurations_files\\config.json");
    nlohmann::json dict;
    try {
        checkConfigException(configFile);
    }
    catch(const std::exception& x) {
        std::cerr << "Caught exception: " << x.what() << std::endl;//если файл не открылся, выбрасываем исключение "Config file is missing!"
        exit(0);
    }
    configFile >> dict;
    configFile.close();

    auto* window = new QFrame;
    auto* layout = new QVBoxLayout;
    QStringList lst;
    auto* lwg = new QListWidget;
    auto* label = new QPlainTextEdit;

    window->setLayout(layout);
    window->setWindowTitle("Search engine");
    layout->addWidget(lwg);
    layout->addWidget(label);

    lwg->setIconSize(QSize(48, 48));

    for (auto &str: dict["files"]) {
        std::string s = str.dump();
        lst << s.c_str();
    }
    for (int i = 0; i < lst.size(); ++i) {
        auto *pitem = new QListWidgetItem(lst[i], lwg);
        pitem->setIcon(QPixmap("..\\..\\search_engine\\pictures\\text.png"));
        lwg->insertItem(i, pitem);
    }

    QObject::connect(lwg, &QListWidget::itemClicked, [lwg, lst, answers, label]() {
                         auto *currentFile = lwg->currentItem();
                         QString pathCurrentFile = currentFile->text();
                         QStringList outText;
                         int textFilePos = 0;
                         while (true) {
                             if (lst[textFilePos] == pathCurrentFile) {
                                 break;
                             }
                             ++textFilePos;
                         }

                         for (int i = 0; i < answers.size(); ++i) {
                             std::string requestsNum = "request" + std::to_string(i + 1);//записываем номер запроса
                             outText.push_back(requestsNum.c_str());
                             if (answers[i].size() > 1) {
                                 //если на один запрос нашлись ответы в нескольких документах
                                 bool flag = false;
                                 for (int j = 0; j < answers[i].size(); ++j) {
                                     if (answers[i][j].second != 0.0f) {
                                         if (answers[i][j].first == textFilePos) {
                                             outText.push_back("    Condition: true");
                                             std::string rank = std::to_string(answers[i][j].second);
                                             outText.push_back("    Relevance: ");
                                             outText.push_back(rank.c_str());
                                             outText.push_back("\n");
                                             flag = true;
                                             break;
                                         }
                                     }
                                 }
                                 if(!flag){
                                     outText.push_back("    Condition: false\n");
                                 }
                             } else if (answers[i].size() == 1) {  //если на один запрос ответ нашелся только в одном документе, выполняем это условие
                                 bool flag = false;
                                 if (answers[i][0].second != 0.0f) {//если запрос нашел ответ в каком-то документе, то переменная rank будет отлично от нуля, выполняем это условие
                                     if (answers[i][0].first == textFilePos) {
                                         outText.push_back("    Condition: true");
                                         std::string rank = std::to_string(answers[i][0].second);
                                         outText.push_back("    Relevance: ");
                                         outText.push_back(rank.c_str());
                                         outText.push_back("\n");
                                         flag = true;
                                     }
                                 }
                                 if(!flag) {
                                     outText.push_back("    Condition: false\n");
                                 }
                             }

                             QString s;
                             for (auto &str: outText) {
                                 s += str;
                             }
                             label->setPlainText(s);
                         }
                     }

    );
    window->resize(400, 600);
    window->move(700, 200);
    window->show();
    return QApplication::exec();
}

int main(int argc, char** argv) {
    checkApp();//проверяем поля файла config.json и выводим сообщение о начале работы
    checkVersionOfApplication();//проверка версии приложения в CMakeList и в config.json
    auto *converter = new ConverterJSON;
    std::vector<std::string> text;
    std::vector<std::string> requests;
    std::vector<std::vector<std::pair<int, float>>> answers;
    auto *ind = new InvertedIndex;
    requests = converter->GetRequests();
    std::thread getTextsThread(getTexts, converter, std::ref(text));//поток для получения всех текстов из текстовых файлов
    std::thread updateDocsTread(UpdateDocumentBase, ind, std::ref(text));//поток для обновления базы документов
    std::this_thread::sleep_for(std::chrono::milliseconds(300));//останов основного потока на 300 миллисекунд перед проведением расчета
    auto *server = new SearchServer(ind);
    calculateRelRelative(server, requests, answers);
    converter->putAnswers(answers);
    getTextsThread.join();
    updateDocsTread.join();
    delete converter;
    delete server;
    visualApp(argc, argv, answers);//функция для визуализации приложения
    std::cout << "Exit from app" << std::endl;
    return 0;
}


