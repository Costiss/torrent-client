#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>

#include "lib/bencode/bencode.hpp"
#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

json decode_bencoded_value(const std::string &encoded_value) {
    size_t position = 0;
    if (std::isdigit(encoded_value[position])) {
        std::string str = decode_bencoded_string(encoded_value, position);
        return json(str);
    } else if (encoded_value[position] == 'i') {
        int64_t number = decode_bencoded_integer(encoded_value, position);
        return json(number);
    } else if (encoded_value[position] == 'l') {
        auto list = decode_bencoded_list(encoded_value, position);
        return list;
    } else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}

int main(int argc, char *argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "decode") {
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
            return 1;
        }

        std::string encoded_value = argv[2];
        json decoded_value = decode_bencoded_value(encoded_value);
        std::cout << decoded_value.dump() << std::endl;
    } else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
