#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "../nlohmann/json.hpp"

bool is_bencoded_string(std::string const &input) { return std::isdigit(input[0]); };

std::string decode_bencoded_string(std::string const &input, size_t &position) {
    if (!std::isdigit(input[position])) {
        throw std::runtime_error("bencoded value is not a string: " + input);
    }

    size_t colon_index = input.find(':', position);
    if (colon_index != std::string::npos) {
        std::string number_string = input.substr(position, colon_index);
        int64_t number = std::atoll(number_string.c_str());
        position = colon_index + number + 1;
        return input.substr(colon_index + 1, number);
    } else {
        throw std::runtime_error("Invalid encoded value: " + input);
    }
};

bool is_bencoded_integer(std::string const &input) { return input[0] == 'i'; };

int64_t decode_bencoded_integer(std::string const &input, size_t &position) {
    if (input[position] != 'i') {
        throw std::runtime_error("bencoded value is not a integer: " + input);
    }

    size_t e_index = input.find('e', position);
    if (e_index != std::string::npos) {
        std::string number_string = input.substr(position + 1, e_index);
        position = e_index + 1;
        return std::atoll(number_string.c_str());
    } else {
        throw std::runtime_error("Invalid encoded value: " + input);
    }
};

nlohmann::json decode_bencoded_list(std::string const &input, size_t &position) {
    if (input[position] != 'l') {
        throw std::runtime_error("bencoded value is not a list: " + input);
    }
    nlohmann::json array = nlohmann::json::array();

    position = position + 1;
    while (position < input.size()) {
        char current = input[position];

        if (std::isdigit(current)) {
            std::string string_value = decode_bencoded_string(input, position);
            array.push_back(string_value);
        } else if (current == 'i') {
            int64_t integer_value = decode_bencoded_integer(input, position);
            array.push_back(integer_value);
        } else if (current == 'l') {
            nlohmann::json list_value = decode_bencoded_list(input, position);
            array.push_back(list_value);
        } else if (current == 'e') {
            position = position + 1;
            break;
        }
    };

    return array;
}
