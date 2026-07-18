#include "Utils.hpp"
#include <boost/locale/generator.hpp>

std::string Utils::ToLower(std::string_view text)
{
    static const std::locale loc = boost::locale::generator()("ru_RU.UTF-8");
    return boost::locale::to_lower(std::string(text), loc);
}
