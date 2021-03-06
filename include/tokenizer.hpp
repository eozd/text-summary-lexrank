#pragma once

#include "defs.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace ir {

/**
 * @brief Split the given string with respect to whitespace characters and
 * return the resulting tokens and their positions in the document as a
 * vector.
 *
 * @param str Input string to tokenize.
 *
 * @return std::vector of pairs containing the tokens and their positions.
 */
std::vector<std::string> tokenize(const std::string& str);

/**
 * @brief Remove certain punctuation characters from certain parts of the
 * given string and return a copy.
 *
 * Punctuation characters are removed as specified in ir::normalize.
 *
 * @param token Input token from which punctuation will be removed.
 *
 * @return String with certain punctuation characters removed from token.
 */
std::string remove_punctuation(const std::string& token);

/**
 * @brief Tokenize and normalize a given raw document and return an
 * ir::NormalizeDocument object. containing terms and counts of each sentence.
 *
 * This function tokenizes the given raw document by splitting it by
 * whitespace, and then does normalization operations to each token as
 * defined in ir::normalize.
 *
 * @param doc A raw document stored as ir::RawDocument.
 *
 * @return Normalized version of the given raw document.
 */
NormalizedDocument normalize_document(const RawDocument& doc);

/**
 * @brief Return the normalized version a given token.
 *
 * Token normalization consists of the following steps:
 *
 * 1. Punctuation removal
 *   * All punctuation characters at the beginning and at the end of the
 * token is removed. Additionally, <blockquote>' " < > </blockquote>
 * characters are removed from anywhere in the token.
 *
 * 2. Case folding
 *   * All characters are converted to lowercase.
 * 3. Stopword removal
 *   * If the given token is a stopword, an empty string is returned.
 * 4. Stemming
 *   * The resulting token is stemmed using the Porter Stemmer.
 *
 * @param token Token to normalize.
 *
 * @return Normalized version of the token. IF the given token is a
 * stopword, an empty string is returned.
 */
std::string normalize(const std::string& token);

/**
 * @brief Normalize all the tokens in the given vector of tokens
 * in-place.
 *
 * This function is a convenience function to apply ir::normalize to
 * each element of input token_vec and then remove empty strings from
 * the vector.
 *
 * @param token_vec vector of tokens to normalize in-place.
 */
void normalize_all(std::vector<std::string>& token_vec);

/**
 * @brief Check whether the input string is a stopword.
 *
 * This function simply checks if the given word is in the stopword list
 * defined in ir::STOPWORD_PATH.
 *
 * For efficiency purposes, the file is read only once when the function
 * is called for the first time, and the stopword list is sorted. Then,
 * stopword check is done using binary search.
 *
 * @param word Word to check if it is a stopword.
 *
 * @return true if word is in stopword list; false, otherwise.
 */
bool is_stopword(const std::string& word);

/**
 * @brief Return normalized versions of the given raw document index.
 *
 * @param raw_docs Mapping from document ID to ir::RawDocument objects.
 * @return Normalized versions of the given raw documents as a mapping from
 * document IDs to ir::NormalizedDocument objects.
 */
std::unordered_map<size_t, ir::NormalizedDocument>
normalized_docs_from_raw_docs(
    const std::unordered_map<size_t, ir::RawDocument>& raw_docs);
} // namespace ir
