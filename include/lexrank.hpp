#pragma once

#include "defs.hpp"
#include "matrix.hpp"
#include "util.hpp"
#include "vector_space_model.hpp"
#include <algorithm>
#include <vector>

namespace ir {

/**
 * @brief Cosine similarity threshold for creating an edge between two nodes
 *
 * If the cosine similarity of two sentences are greater than or equal to
 * this value, then there will be an edge between the nodes of the two
 * sentences.
 */
const double LexrankEdgeThreshold = 0.1;

/**
 * @brief Epsilon value that determines the convergence of power iteration.
 *
 * During power iteration, for vectors \f$x^t\f$ and \f$x^{t+1}\f$ where \f$t\f$
 * is iteration count, if
 *
 * \f[
 *     |x_i^t - x_i^{t+1}| \leq \epsilon \qquad \forall i
 * \f]
 *
 * then it is assumed that power iteration has converged.
 */
const double PowerIterationEpsilon = 1e-5;

/**
 * @brief Damping factor of the LexRank algorithm (teleportation rate of the
 * underlying Markov Chain).
 */
const double DampingFactor = 0.15;

/**
 * @brief Build an adjacency matrix to be used in LexRank algorithm from the
 * tf-idf vectors of all sentences.
 *
 * tf-idf vectors of the sentences are taken as unordered maps to prevent
 * storing many 0 tf-idf values.
 *
 * In the adjacency matrix, if two sentences \f$i\f$ and \f$j\f$ have cosine
 * similarity greater than or equal to ir::LexrankEdgeThreshold, then there will
 * be 1's in entries \f$X_{ij}\f$ and \f$X_{ji}\f$. All other entries in the
 * matrix are 0.
 *
 * @param tfidf_maps Vector storing tf-idf map of each sentence.
 * @return An ir::Matrix of chars having 0 or 1 as value.
 */
Matrix<char> build_adjacency_matrix(
    const std::vector<std::unordered_map<std::string, double>>& tfidf_maps);

/**
 * @brief Construct the Markov Chain transition probability matrix from the
 * given adjacency matrix and the damping factor.
 *
 * Transition probability matrix is constructed by incorporating the damping
 * factor into the adjacency matrix and normalizing each column so that it sums
 * to 1.
 *
 * Each entry \f$X_{ij}\f$ represents the probability of being at state \f$i\f$
 * in the next timestep given that the random walker is at state \f$j\f$ in the
 * current timestep. Therefore, \f$j^{th}\f$ of the returned matrix represents
 * the transition distribution from state \f$j\f$. As a consequence, each column
 * of this matrix sums to 1.
 *
 * Damping factor is used so that every pair of states has an edge between them,
 * i.e. the underlying graph is fully connected. This is used to make the
 * Markov Chain ergodic so that a steady state distribution is guaranteed to
 * exist.
 *
 * Damping factor \f$d\f$ is used by setting all 0's to \f$\frac{d}{N}\f$ where
 * \f$N\f$ is the dimension of the square adjacency matrix. All nonzero entries
 * are multiplied by \f$1 - d\f$ and incremented by \f$\frac{d}{N}\f$. These
 * operations can be succinctly characterized by the following vector formula:
 *
 * \f[
 *     \frac{d}{N}\boldsymbol{I} + (1 - d)\boldsymbol{X}
 * \f]
 *
 * @param adj_mat Adjacency matrix consisting of 0's and 1's.
 * @param damping_factor Damping factor to apply.
 * @return Transition probability matrix of the underlying Markov Chain
 * constructed with the given damping factor.
 */
Matrix<double> markov_chain_mat(const Matrix<char>& adj_mat,
                                double damping_factor);

/**
 * @brief Apply LexRank algorithm to the folliwng normalized document and return
 * the LexRank score of each sentence in the given order.
 *
 * @param norm_doc A normalized document containing terms and counts of each
 * sentence.
 * @param idf_scores A map containing idf scores of all terms that occur in the
 * given document.
 * @return Vector of LexRank score of each sentence in the given order.
 */
std::vector<double>
lexrank(const ir::NormalizedDocument& norm_doc,
        const std::unordered_map<std::string, double>& idf_scores);
} // namespace ir
