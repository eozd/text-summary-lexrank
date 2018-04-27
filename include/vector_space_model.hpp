#pragma once

#include "defs.hpp"
#include <unordered_map>

namespace ir {

/**
 * @brief Calculate idf value of every term in the given document corpus and
 * return the result as a mapping from terms to their idf values.
 *
 * This function computes the idf score of every term. idf value of term \f$t\f$
 * is defined as
 *
 * \f[
 *     \log_{10}{\frac{N}{df_t}}
 * \f]
 *
 * where \f$N\f$ is the total number of documents in the corpus and \f$df_t\f$
 * is the number of documents containing term \f$t\f$. The results dampened
 * using base 10 logarithm.
 *
 * @param document_corpus Document corpus to use when calculating idf scores.
 * idf score of every term in this corpus will be calculated.
 * @return A mapping from terms to their idf scores.
 */
std::unordered_map<std::string, double> idf_scores(
    const std::unordered_map<size_t, ir::NormalizedDocument>& document_corpus);

/**
 * @brief Calculate tf-idf vector of every sentence in the given document and
 * return the vectors in the same sentence order.
 *
 * This function computes tf-idf vector of every sentence in the document text
 * and returns the vectors in the same order. tf-idf vectors of golden standard
 * summary sentences are not calculated.
 *
 * For efficiency reasons, tf-idf vectors are stored as unordered maps. Terms
 * whose tf-idf value is very close to 0 are not included in the returned maps.
 * Therefore, the returned maps contain terms and their tf-idf values which are
 * different than 0.
 *
 * @param norm_doc Normalized document containing sentences whose tf-idf vectors
 * will be calculated.
 * @param idf_scores A mapping from terms to their idf scores.
 * @return Vector of tf-idf maps. Each entry of the returned vector contains the
 * tf-idf map of the corresponding sentence in the given document, in the same
 * order.
 */
std::vector<std::unordered_map<std::string, double>>
tf_idf_maps(const ir::NormalizedDocument& norm_doc,
            const std::unordered_map<std::string, double>& idf_scores);

/**
 * @brief Return the Euclidean length of the given tf-idf map.
 *
 * @param vec tf-idf map containing terms and their tf-idf values.
 * @return Euclidean length of the given tf-idf map.
 */
double euc_len(const std::unordered_map<std::string, double>& vec);

/**
 * @brief Calculate the cosine similarity between two given tf-idf maps.
 *
 * Cosine similarity of two tf-idf maps represented as \f$M_1, M_2\f$ are
 * calculated as follows:
 *
 * \f[
 *     \frac{1}{|M_1|\cdot|M_2|}\sum_{w \in M_1 \cup M_2} M_1[w] \cdot M_2[w]
 * \f]
 *
 * where \f$|\cdot|\f$ represents the Euclidean norm and \f$M_1[w]\f$ is the
 * tf-idf value of word \f$w\f$ in map \f$M_1\f$.
 *
 * @param vec1 First tf-idf map.
 * @param vec2 Second tf-idf map.
 * @return Cosine similarity between the given tf-idf maps.
 */
double cosine_sim(const std::unordered_map<std::string, double>& vec1,
                  const std::unordered_map<std::string, double>& vec2);
} // namespace ir
