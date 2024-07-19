#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>

#include "lib/bencode/bencode.hpp"
#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

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
    } else if (command == "info") {
        std::string filepath = argv[2];
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open the file." << std::endl;
            return 1;
        }

        std::string line;
        std::getline(file, line);

        json decoded_value = decode_bencoded_value(line);
        std::string tracker_url = decoded_value.at("announce").get<std::string>();
        int64_t length = decoded_value.at("info").at("length").get<int64_t>();

        std::cout << "Tracker URL: " << tracker_url << std::endl;
        std::cout << "Length: " << length << std::endl;

        file.close();
    } else {
        std::cerr << "unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
