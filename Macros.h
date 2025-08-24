#pragma once
#include<exception>
#include<string>
#include<concepts>
#define _NODISCARD [[nodiscard]]

#define _CONSTEXPR constexpr
#define _PANAGIOTIS_BEGIN namespace panagiotis{
#define _PANAGIOTIS_END }
_PANAGIOTIS_BEGIN
class out_of_bounds :public std::exception
{
private:
    std::string errorMessage; // To store the error message
public:
    //Constructor to initialize the error message
    explicit out_of_bounds(const std::string& message)
        : errorMessage(message) {
    }

    // Override the what() method
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};


_PANAGIOTIS_END
