#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "InvertedIndex.h"

TEST(TestCaseInvertedIndex, TestBasic) {
    const std::vector<std::string> docs = {"london is the capital of great britain",
                                           "big ben is the nickname for the Great bell of the striking clock"};
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<std::vector<Entry>> expected = {{{0, 1}},
                                                      {{0, 1}, {1, 3}}};
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}//тест класса InvertedIndex
TEST(TestCaseInvertedIndex, TestBasic2) {
    const std::vector<std::string> docs = {"milk milk milk milk water water water",
                                           "milk water water",
                                           "milk milk milk milk milk water water water water water",
                                           "americano cappuccino"};
    const std::vector<std::string> requests = {"milk", "water", "cappuccino"};
    const std::vector<std::vector<Entry>> expected = {{{0, 4}, {1, 1}, {2, 5}},
                                                      {{0, 3}, {1, 2}, {2, 5}},
                                                      {{3, 1}}};
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}//тест класса InvertedIndex
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const std::vector<std::string> docs = {"a b c d e f g h i j k l", "statement"};
    const std::vector<std::string> requests = {"m", "statement"};
    const std::vector<std::vector<Entry>> expected = {{{0,0}}, {{1, 1}}};
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}//тест класса InvertedIndex



