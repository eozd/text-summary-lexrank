#pragma once

#include <limits>
#include <string>
#include <cmath>
#include <vector>

namespace ir {

/**
 * @brief Split the given input string using one of the delimeters and return
 * a vector of tokens.
 *
 * This function is a wrapper around well-known C strtok function to tokenize
 * a string using a list of delimiters.
 *
 * @param str String to tokenize. Characters that are one of the given
 * delimiters will be replaced by the NULL character.
 * @param delimiters List of delimiters.
 *
 * @return std::vector of resulting tokens.
 */
std::vector<std::string> split(std::string& str, const std::string& delimiters);

/**
 * @brief Check whether the given two Real types are within epsilon of each
 * other.
 *
 * This function returns true for inputs \f$a, b, \epsilon\f$ if
 *
 * \f[
 *     |a - b| \leq \epsilon
 * \f]
 *
 * Otherwise it returns false.
 *
 * @tparam Real A floating point real type.
 * @param a First number.
 * @param b Second number.
 * @param eps Epsilon that will be used to check for closeness. Two numbers are
 * close if their difference is less than or equal to this value.
 * @return True if the given two numbers are close; false, otherwise.
 */
template <typename Real>
bool close(Real a, Real b, Real eps = std::numeric_limits<Real>::epsilon()) {
    return std::abs(a - b) <= eps;
}
} // namespace ir
