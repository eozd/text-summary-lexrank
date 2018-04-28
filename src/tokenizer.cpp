#include "tokenizer.hpp"
#include "util.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <file_manager.hpp>
#include <fstream>
#include <iostream>
#include <numeric>

// tell the compiler that stem will be externally linked
extern int stem(char* p, int i, int j);

std::vector<std::string> ir::tokenize(const std::string& str) {
    std::string str_copy(str);

    return split(str_copy, " \t\n\r\v\f");
}

std::string ir::remove_punctuation(const std::string& token) {
    std::string result(token);

    // remove certain puncts from anywhere in the word
    std::replace(result.begin(), result.end(), '\"', '\'');
    std::replace(result.begin(), result.end(), ',', '\'');
    std::replace(result.begin(), result.end(), '<', '\'');
    std::replace(result.begin(), result.end(), '>', '\'');
    result.erase(std::remove(result.begin(), result.end(), '\''), result.end());

    if (result.empty()) {
        return result;
    }

    // iterate as long as current char is not alphanumeric and not '
    // (' will be removed)
    auto to_remove = [](const char c) { return c != '\'' && !isalnum(c); };
    // remove any kind of punct from the start and end of the word
    for (size_t i = 0; i < result.size() && to_remove(result[i]); ++i) {
        result[i] = '\'';
    }
    for (int i = result.size() - 1; i >= 0 && to_remove(result[i]); --i) {
        result[i] = '\'';
    }
    result.erase(std::remove(result.begin(), result.end(), '\''), result.end());

    return result;
}

bool ir::is_stopword(const std::string& word) {
    static std::vector<std::string> stopwords;

    // if calling for the first time
    if (stopwords.empty()) {
        std::ifstream ifs(ir::STOPWORD_PATH);
        std::string stopword;
        while (ifs >> stopword) {
            stopwords.push_back(stopword);
        }
        assert(!stopwords.empty());

        std::sort(stopwords.begin(), stopwords.end());
    }

    return std::binary_search(stopwords.begin(), stopwords.end(), word);
}

std::string ir::normalize(const std::string& token) {
    // remove punctuation using heuristics
    std::string result = remove_punctuation(token);
    // convert string to lowercase
    std::transform(result.begin(), result.end(), result.begin(), tolower);
    // if string is a stopword, return empty string
    if (is_stopword(result)) {
        return "";
    }
    // stem the word
    auto word_end = static_cast<size_t>(stem(&result[0], 0, result.size() - 1));
    result = result.substr(0, word_end + 1);

    return result;
}

void ir::normalize_all(std::vector<std::string>& token_vec) {
    // normalize all words in-place
    std::transform(token_vec.begin(), token_vec.end(), token_vec.begin(),
                   [](const std::string& token) { return normalize(token); });
    // remove empty strings
    token_vec.erase(std::remove(token_vec.begin(), token_vec.end(), ""),
                    token_vec.end());
}

ir::NormalizedDocument ir::normalize_document(const ir::RawDocument& raw_doc) {
    ir::NormalizedDocument norm_doc;
    for (const auto& sentence : raw_doc.sentences) {
        auto tokens = tokenize(sentence);
        normalize_all(tokens);
        if (tokens.empty()) {
            continue;
        }

        std::sort(tokens.begin(), tokens.end());
        norm_doc.sentence_term_counts.emplace_back();
        for (const auto& term : tokens) {
            ++norm_doc.sentence_term_counts.back()[term];
        }
    }

    return norm_doc;
}

std::unordered_map<size_t, ir::NormalizedDocument>
ir::normalized_docs_from_raw_docs(
    const std::unordered_map<size_t, ir::RawDocument>& raw_docs) {

    std::unordered_map<size_t, ir::NormalizedDocument> result;
    // normalize each document
    for (const auto& doc_pair : raw_docs) {
        size_t id = doc_pair.first;
        const auto& doc = doc_pair.second;

        result[id] = ir::normalize_document(doc);
    }

    return result;
};
