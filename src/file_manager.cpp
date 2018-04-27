#include "file_manager.hpp"
#include <algorithm>
#include <cassert>
#include <dirent.h>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <sstream>

std::vector<std::string>
ir::get_data_file_list(const std::string& dataset_dir) {
    DIR* dirp = opendir(dataset_dir.c_str());

    struct dirent* dp;
    std::string filename;
    std::vector<std::string> file_list;

    while ((dp = readdir(dirp)) != nullptr) {
        filename = dp->d_name;

        // if filename ends with the extension
        if (filename.find(DOCUMENT_EXTENSION) ==
            filename.size() - DOCUMENT_EXTENSION.size()) {
            file_list.push_back(dataset_dir + '/' + filename);
        }
    }

    closedir(dirp);

    std::sort(file_list.begin(), file_list.end());
    return file_list;
}

size_t ir::doc_id_from_filepath(const std::string& filepath) {
    size_t last_slash_index = filepath.rfind('/');
    size_t next_dot_inedx = filepath.find('.', last_slash_index);
    size_t len = next_dot_inedx - last_slash_index;

    std::string id_str = filepath.substr(last_slash_index + 1, len);

    return std::stoul(id_str);
}

std::ostream&
ir::write_idf_file(std::ostream& os,
                   const std::unordered_map<std::string, double>& idf_scores) {
    double precision = std::numeric_limits<double>::max_digits10;

    for (const auto& term_pair : idf_scores) {
        const auto& term = term_pair.first;
        const double idf = term_pair.second;

        os << term << ' ' << std::setprecision(precision) << idf << '\n';
    }
    os << std::flush;

    return os;
}

std::istream&
ir::read_idf_file(std::istream& is,
                  std::unordered_map<std::string, double>& idf_scores) {
    std::string term;
    double idf;
    while (is >> term >> idf) {
        assert(idf_scores.find(term) == idf_scores.end() &&
               "Term already exists in ir::read_idf_file");
        idf_scores[term] = idf;
    }

    return is;
}
