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

using json = nlohmann::json;
json decode_bencoded_string(std::string const &input, size_t &position);
json decode_bencoded_integer(std::string const &input, size_t &position);
json decode_bencoded_list(std::string const &input, size_t &position);
json decode_bencoded_value(const std::string &encoded_value, size_t &position);
json decode_bencoded_value(const std::string &encoded_value);

bool is_bencoded_string(std::string const &input) { return std::isdigit(input[0]); };

json decode_bencoded_string(std::string const &input, size_t &position) {
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

json decode_bencoded_integer(std::string const &input, size_t &position) {
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

json decode_bencoded_list(std::string const &input, size_t &position) {
    if (input[position] != 'l') {
        throw std::runtime_error("bencoded value is not a list: " + input);
    }
    json array = json::array();

    position = position + 1;
    while (position < input.size()) {
        char current = input[position];
        if (current == 'e')
            break;

        json value = decode_bencoded_value(input, position);
        array.push_back(value);
    };
    position = position + 1;

    return array;
}

json decode_bencoded_dict(std::string const &input, size_t &position) {
    if (input[position] != 'd') {
        throw std::runtime_error("bencoded value is not a dictionary: " + input);
    }
    json dict = json::object_t();

    position = position + 1;
    while (position < input.size()) {
        char current = input[position];
        if (current == 'e')
            break;

        std::string key = decode_bencoded_string(input, position);
        json value = decode_bencoded_value(input, position);
        dict[key] = value;
    }
    position = position + 1;

    return dict;
}

json decode_bencoded_value(const std::string &encoded_value, size_t &position) {
    if (std::isdigit(encoded_value[position])) {
        std::string str = decode_bencoded_string(encoded_value, position);
        return json(str);
    } else if (encoded_value[position] == 'i') {
        int64_t number = decode_bencoded_integer(encoded_value, position);
        return json(number);
    } else if (encoded_value[position] == 'l') {
        return decode_bencoded_list(encoded_value, position);
    } else if (encoded_value[position] == 'd') {
        return decode_bencoded_dict(encoded_value, position);
    } else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}

json decode_bencoded_value(const std::string &encoded_value) {
    size_t position = 0;
    if (std::isdigit(encoded_value[position])) {
        std::string str = decode_bencoded_string(encoded_value, position);
        return json(str);
    } else if (encoded_value[position] == 'i') {
        int64_t number = decode_bencoded_integer(encoded_value, position);
        return json(number);
    } else if (encoded_value[position] == 'l') {
        return decode_bencoded_list(encoded_value, position);
    } else if (encoded_value[position] == 'd') {
        return decode_bencoded_dict(encoded_value, position);
    } else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}
