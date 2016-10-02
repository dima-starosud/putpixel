#ifndef PUTPIXEL_READER_H
#define PUTPIXEL_READER_H

#include <string>
#include <boost/lexical_cast.hpp>

namespace util
{
    template<typename... Ts, typename Begin, typename End>
    std::tuple<Ts...> read_tuple(Begin it, const End itEnd)
    {
        const std::tuple<Ts...> result{
            (it != itEnd ? boost::lexical_cast<Ts>(*it++) : throw std::runtime_error("too few elements"))...
        };
        if (it != itEnd)
            throw std::runtime_error("too many elements");
        return result;
    }

    template<typename... Ts>
    std::tuple<Ts...> read_tuple(std::string s)
    {
        std::istringstream iss(s);
        return read_tuple<Ts...>(
            std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>{});
    }
}

#endif //PUTPIXEL_READER_H
