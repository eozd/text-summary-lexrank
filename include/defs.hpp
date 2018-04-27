#pragma once

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Main namespace for all the functions and classes.
 */
namespace ir {
/**
 * @brief A map from terms to their counts.
 */
using doc_terms = std::unordered_map<std::string, size_t>;

/**
 * @brief A class representing a raw (non-tokenized/non-normalized) document.
 *
 * A raw document represents a sequence of sentences.
 */
struct RawDocument {
    /**
     * @brief Type of the sentences that make up the document.
     */
    using sentence = std::string;

    /**
     * @brief Default constructor.
     *
     * This constructor constructs an empty raw document.
     */
    RawDocument() = default;

    /**
     * @brief Constructor that initializes a document with the given sentences.
     *
     * @param sentence_vec Vector of sentences that make up the document text.
     */
    explicit RawDocument(std::vector<sentence> sentence_vec)
        : sentences(std::move(sentence_vec)) {}

    /**
     * @brief Vector of sentences that make up the document text.
     */
    std::vector<sentence> sentences;
};

/**
 * @brief A class representing a tokenized and normalized document.
 *
 * Each sentence in a normalized document consists of normalized terms.
 */
struct NormalizedDocument {
    /**
     * @brief Type of the sentences that make up a normalized document.
     */
    using sentence = doc_terms;

    /**
     * @brief Default constructor.
     *
     * This constructor constructs an empty normalized document.
     */
    NormalizedDocument() = default;

    /**
     * @brief Constructor that initializes a document with the given sentences.
     *
     * @param sentence_term_counts Vector of sentences that make up the document
     * text.
     */
    explicit NormalizedDocument(std::vector<sentence> sentence_term_counts)
        : sentence_term_counts(std::move(sentence_term_counts)) {}

    /**
     * @brief Vector storing terms and counts for each sentence that make
     * up the document text.
     */
    std::vector<sentence> sentence_term_counts;
};

} // namespace ir
