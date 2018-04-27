#include "vector_space_model.hpp"
#include "util.hpp"
#include <unordered_set>
#include <cmath>

std::unordered_map<std::string, double> ir::idf_scores(
    const std::unordered_map<size_t, ir::NormalizedDocument>& document_corpus) {
    std::unordered_map<std::string, double> result;

    // store counts for each document
    for (const auto& doc_pair : document_corpus) {
        const auto& doc = doc_pair.second;

        // store which words occur in current document
        std::unordered_set<std::string> doc_words;
        for (const auto& sentence : doc.sentence_term_counts) {
            for (const auto& word_pair : sentence) {
                const auto& word = word_pair.first;
                doc_words.insert(word);
            }
        }

        // increment count of each occurring word
        for (const auto& word : doc_words) {
            ++result[word];
        }
    }

    size_t n_docs = document_corpus.size();
    // calculate idf scores
    for (auto& term_pair : result) {
        double doc_freq = term_pair.second;
        double idf = std::log10(n_docs / doc_freq);

        term_pair.second = idf;
    }

    return result;
};

std::vector<std::unordered_map<std::string, double>>
ir::tf_idf_maps(const ir::NormalizedDocument& norm_doc,
                const std::unordered_map<std::string, double>& idf_scores) {
    std::vector<std::unordered_map<std::string, double>> result;
    for (const auto& sentence : norm_doc.sentence_term_counts) {
        result.emplace_back();

        for (const auto& term_pair : sentence) {
            const std::string& term = term_pair.first;
            size_t count = term_pair.second;

            double tf = (count > 0) ? (1 + std::log10(count)) : 0;
            double idf = idf_scores.at(term);

            double tfidf = tf * idf;
            if (not close(tfidf, 0.0)) {
                result.back()[term] = tfidf;
            }
        }
    }

    return result;
}

double ir::euc_len(const std::unordered_map<std::string, double>& vec) {
    double result = 0;
    for (const auto& pair : vec) {
        double value = pair.second;

        result += value * value;
    }

    return std::sqrt(result);
}

double ir::cosine_sim(const std::unordered_map<std::string, double>& vec1,
                      const std::unordered_map<std::string, double>& vec2) {
    double result = 0;
    for (const auto& term_pair : vec1) {
        const auto& term = term_pair.first;
        const double tfidf1 = term_pair.second;

        // if term occurs in both maps
        if (vec2.find(term) != vec2.end()) {
            const double tfidf2 = vec2.at(term);
            result += tfidf1 * tfidf2;
        }
    }

    return result / (euc_len(vec1) * euc_len(vec2));
}
