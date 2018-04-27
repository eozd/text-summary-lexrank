#include "parser.hpp"
#include "file_manager.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <utility>

ir::RawDocument ir::parse_doc_file(std::istream& input_stream) {
    std::vector<std::string> sentence_vec;

    std::string line;
    while (std::getline(input_stream, line) && not line.empty()) {
        sentence_vec.push_back(line);
    }

    return RawDocument(sentence_vec);
}

std::unordered_map<size_t, ir::RawDocument>
ir::docs_from_files(const std::vector<std::string>& file_list) {
    std::unordered_map<size_t, ir::RawDocument> result;
    for (const std::string& filepath : file_list) {
        // parse document and document id
        std::ifstream ifs(filepath);
        size_t doc_id = ir::doc_id_from_filepath(filepath);
        auto doc = ir::parse_doc_file(ifs);

        assert(result.find(doc_id) == result.end() &&
               "Document with id already exists!");

        result.insert(std::make_pair(doc_id, doc));
    }

    return result;
};
