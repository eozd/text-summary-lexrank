#include "lexrank.hpp"

ir::Matrix<char> ir::build_adjacency_matrix(
    const std::vector<std::unordered_map<std::string, double>>& tfidf_maps) {
    // resulting adjacency matrix
    Matrix<char> result(tfidf_maps.size(), tfidf_maps.size());

    // for each different pair
    for (size_t i = 0; i < tfidf_maps.size(); ++i) {
        for (size_t j = i + 1; j < tfidf_maps.size(); ++j) {
            const auto& row_sentence = tfidf_maps[i];
            const auto& col_sentence = tfidf_maps[j];

            // if cosine similarity is greater than threshold, make an edge
            double cos_sim = ir::cosine_sim(row_sentence, col_sentence);
            if (cos_sim >= LexrankEdgeThreshold) {
                result(i, j) = result(j, i) = 1;
            }
        }
    }

    // every node must have an edge to itself
    for (size_t i = 0; i < result.rows(); ++i) {
        result(i, i) = true;
    }

    return result;
}

ir::Matrix<double> ir::markov_chain_mat(const Matrix<char>& adj_mat,
                                        double damping_factor) {
    const size_t n = adj_mat.rows();

    // compute column-normalized no-teleportation transition probability matrix
    Matrix<double> result(n, n);
    for (size_t j = 0; j < n; ++j) {

        // calculate column sum
        size_t colsum = 0;
        for (size_t i = 0; i < n; ++i) {
            colsum += adj_mat(i, j);
        }

        // uniform distribution initialization (random walk)
        double entry = 1.0 / colsum;
        for (size_t i = 0; i < n; ++i) {
            if (adj_mat(i, j)) {
                result(i, j) = entry;
            }
        }
    }

    // teleportation (damping)
    double entry_damping = damping_factor / n;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            result(i, j) *= (1 - damping_factor);
            result(i, j) += entry_damping;
        }
    }

    return result;
}

std::vector<double>
ir::lexrank(const ir::NormalizedDocument& norm_doc,
            const std::unordered_map<std::string, double>& idf_scores) {
    // get tf-idf maps
    auto tfidf_maps = ir::tf_idf_maps(norm_doc, idf_scores);

    // construct markov chain transition matrix
    const Matrix<double> trans_mat =
        markov_chain_mat(build_adjacency_matrix(tfidf_maps), DampingFactor);

    // initial distribution (assign uniform; doesn't matter anyways)
    Vector<double> lexrank_dist(trans_mat.cols());
    for (size_t i = 0; i < lexrank_dist.size(); ++i) {
        lexrank_dist(i) = 1.0 / lexrank_dist.size();
    }

    // power iteration
    Vector<double> prev = lexrank_dist;
    while (true) {
        // compute next distribution
        lexrank_dist = trans_mat * lexrank_dist;

        // if every entry is the same, we have converged
        bool converged = true;
        for (size_t i = 0; i < lexrank_dist.size(); ++i) {
            if (not ir::close(lexrank_dist(i), prev(i),
                              PowerIterationEpsilon)) {
                converged = false;
                break;
            }
        }

        if (converged) {
            break;
        }

        prev = lexrank_dist;
    }

    // store results in a vector and return
    std::vector<double> result(lexrank_dist.size());
    std::copy(lexrank_dist.data(), lexrank_dist.data() + lexrank_dist.size(),
              result.begin());
    return result;
}
