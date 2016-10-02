#ifndef PUTPIXEL_VECTOR_READER_H
#define PUTPIXEL_VECTOR_READER_H

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <boost/lexical_cast.hpp>

namespace util
{
    template<typename T, typename Begin, typename End>
    std::vector<T> read_vector(Begin it, const End itEnd)
    {
        std::vector<T> result;
        std::transform(it, itEnd, std::back_inserter(result), boost::lexical_cast<T, decltype(*it)>);
        return result;
    }

    template<typename T>
    std::vector<T> read_vector(std::string s)
    {
        std::istringstream iss(s);
        return read_vector<T>(
            std::istream_iterator<std::string> {iss},
            std::istream_iterator<std::string> {});
    }
}

#endif //PUTPIXEL_VECTOR_READER_H
