#pragma once

#include "defs.hpp"
#include <string>
#include <vector>

namespace ir {

/**
 * @brief Extension of documents to be gathered under ir::DATASET_DIR directory.
 */
const std::string DOCUMENT_EXTENSION = ".txt";

/**
 * @brief Relative path from executable to text file containing stopwords.
 */
const std::string STOPWORD_PATH = "stopwords.txt";

/**
 * @brief File containing IDF score of every term.
 */
const std::string IDF_FILEPATH = "idf.txt";

/**
 * @brief Return a list of filepaths of documents under the given dataset
 * directory.
 *
 * UNIX C-API is used to get filenames (specifically dirent.h)
 *
 * @param dataset_dir Directory containing the files.
 *
 * @return vector of strings where each string is the relative path from the
 * executable to a document.
 */
std::vector<std::string> get_data_file_list(const std::string& dataset_dir);

/**
 * @brief Extract the ID of the document from its filepath.
 *
 * @param filepath Path to the document file.
 * @return Document ID.
 */
size_t doc_id_from_filepath(const std::string& filepath);

/**
 * @brief Write the given mapping from terms to their idf scores to the given
 * output stream.
 *
 * The terms and their scores are written as follows:
 *
 * <blockquote>
 * term_1 score_1\n
 * term_2 score_2\n
 * \f$\vdots\f$\n
 * term_N score_N\n
 * </blockquote>
 *
 * @param os Output stream to write the term-idf mapping.
 * @param idf_scores Mapping from terms to their idf scores.
 * @return Modified output stream.
 */
std::ostream&
write_idf_file(std::ostream& os,
               const std::unordered_map<std::string, double>& idf_scores);

/**
 * @brief Read a term-idf mapping from the given input stream.
 *
 * The terms and their scores are read according to the format as specified in
 * ir::write_idf_file.
 *
 * @param is Input stream to read the term-idf mapping.
 * @param idf_scores Mapping to modify in-place and store the read mapping.
 * @return Modified input stream.
 */
std::istream&
read_idf_file(std::istream& is,
              std::unordered_map<std::string, double>& idf_scores);
} // namespace ir
