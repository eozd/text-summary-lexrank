#include "file_manager.hpp"
#include "lexrank.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"
#include "vector_space_model.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>

/**
 * @brief Print LexRank scores and top 3 LexRank sentences as summary
 *
 * @param lexrank_scores Vector containing LexRank score of each sentence in
 * the given raw document.
 * @param raw_doc Raw document containing the original sentences.
 */
static void print_summary(const std::vector<double>& lexrank_scores,
                          const ir::RawDocument& raw_doc) {
    // output lexrank scores
    for (size_t i = 0; i < lexrank_scores.size(); ++i) {
        double score = lexrank_scores[i];
        auto sentence = raw_doc.sentences[i];
        std::cout << std::fixed << std::setprecision(6) << score << '\n';
    }
    std::cout << std::endl;

    // sort sentences with respect to lexrank
    std::vector<std::pair<double, int>> scores_indices;
    for (size_t i = 0; i < lexrank_scores.size(); ++i) {
        scores_indices.emplace_back(lexrank_scores[i], i);
    }
    std::sort(scores_indices.begin(), scores_indices.end(),
              [](const auto& left, const auto& right) {
                  return left.first >= right.first;
              });

    // print top 3 LexRank sentences
    for (size_t i = 0; i < 3; ++i) {
        std::cout << raw_doc.sentences[scores_indices[i].second] << std::endl;
    }
}

/**
 * @brief LexRank main program.
 *
 * Main program
 *
 *   i.   reads command-line arguments,
 *   ii.  parses, tokenizes, normalizes the target document,
 *   iii. reads idf scores,
 *   iv.  computes LexRank scores,
 *   v.   prints LexRank scores and a summary using the top 3 LexRank sentences.
 *
 * @param argc Number of command-line arguments including program name.
 * @param argv Command-line arguments string array.
 * @return -1 if incorrect arguments are given; 0 if program executed
 * successfully.
 */
int main(int argc, char** argv) {
    // read command line arguments
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <Dataset_folder> <filename>"
                  << std::endl;
        return -1;
    }
    std::string dataset_dir(argv[1]);
    std::string filepath = dataset_dir + '/' + std::string(argv[2]);

    // put file to summarize into vector
    std::vector<std::string> file_list = {filepath};

    // parse document and create raw document
    const auto raw_docs = ir::docs_from_files(file_list);

    // normalize document
    const auto norm_docs = ir::normalized_docs_from_raw_docs(raw_docs);

    // read IDF scores
    std::unordered_map<std::string, double> idf_scores;
    {
        std::ifstream idf_file(ir::IDF_FILEPATH);
        ir::read_idf_file(idf_file, idf_scores);
    }

    // get ID and raw/normalized version of target document
    const size_t doc_id = ir::doc_id_from_filepath(filepath);
    const ir::RawDocument rawdoc_to_process = raw_docs.at(doc_id);
    const ir::NormalizedDocument doc_to_process = norm_docs.at(doc_id);

    // compute LexRank scores
    std::vector<double> lexrank_scores = lexrank(doc_to_process, idf_scores);

    // print summary and scores
    print_summary(lexrank_scores, rawdoc_to_process);
}
