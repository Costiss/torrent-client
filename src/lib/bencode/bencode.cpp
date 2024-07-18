#include <cstdint>
#include <stdexcept>
#include <string>

std::string decode_bencoded_string(std::string const &input) {
    size_t colon_index = input.find(':');
    if (colon_index != std::string::npos) {
        std::string number_string = input.substr(0, colon_index);
        int64_t number = std::atoll(number_string.c_str());
        return input.substr(colon_index + 1, number);
    } else {
        throw std::runtime_error("Invalid encoded value: " + input);
    }
};

int64_t decode_bencoded_integer(std::string const &input) {
    size_t e_index = input.find('e');
    if (e_index != std::string::npos) {
        std::string number_string = input.substr(1, e_index);
        return std::atoll(number_string.c_str());
    } else {
        throw std::runtime_error("Invalid encoded value: " + input);
    }
};
