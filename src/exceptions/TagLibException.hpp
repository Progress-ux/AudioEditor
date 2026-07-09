#pragma once

#include <stdexcept>
class TagLibException : public std::runtime_error
{
public:
    explicit TagLibException(const std::string& message) : std::runtime_error(message) {}
};
