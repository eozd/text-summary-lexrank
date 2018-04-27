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
 * @brief IDF calculation main program.
 *
 * Main program
 *
 * i.   reads command-line arguments,
 * ii.  parses, tokenizes, normalizes every document in the corpus,
 * iii. computes idf scores
 * iv.  writes idf scores to ir::IDF_FILEPATH.
 *
 * @param argc Number of command-line arguments including program name.
 * @param argv Command-line arguments string array.
 * @return -1 if incorrect arguments are given; 0 if program executed
 * successfully.
 */
int main(int argc, char** argv) {
    // read command line arguments
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <Dataset_folder>" << std::endl;
        return -1;
    }
    std::string dataset_dir(argv[1]);

    // get filepath of all documents to be used
    std::vector<std::string> file_list = ir::get_data_file_list(dataset_dir);

    // parse documents and create raw documents
    auto raw_docs = ir::docs_from_files(file_list);

    // normalize documents
    auto norm_docs = ir::normalized_docs_from_raw_docs(raw_docs);

    // compute IDF score of each term
    auto idf_scores = ir::idf_scores(norm_docs);

    // write IDF scores to file
    std::ofstream out_idf_file(ir::IDF_FILEPATH);
    ir::write_idf_file(out_idf_file, idf_scores);
}
