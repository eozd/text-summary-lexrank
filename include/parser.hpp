#pragma once

#include <utility>

#include "defs.hpp"

namespace ir {

/**
 * @brief Parse a news text file containing the sentences of the news article.
 *
 * The document to parse must have the following layout:
 *
 * <blockquote>
 * document_sentence_1\n
 * document_sentence_2\n
 * \f$\vdots\f$\n
 * document_sentence_N\n
 *
 * </blockquote>
 *
 * where the last line is empty (single \\n character).
 *
 * @param input_stream Input stream to parse the document from.
 * @return An ir::RawDocument storing the parsed document.
 */
ir::RawDocument parse_doc_file(std::istream& input_stream);

/**
 * @brief Parse each file in the given filelist and return a map from document
 * IDs to ir::RawDocument objects.
 *
 * @param file_list List of files to parse and create raw documents.
 * @return Mapping from parsed document IDs to ir::RawDocument objects.
 */
std::unordered_map<size_t, ir::RawDocument>
docs_from_files(const std::vector<std::string>& file_list);
} // namespace ir
