#ifndef BENCODE_HPP_
#define BENCODE_HPP_

#include "../nlohmann/json.hpp"
#include <string>

#endif

std::string decode_bencoded_string(std::string const &input, size_t &position);
int64_t decode_bencoded_integer(std::string const &input, size_t &position);
nlohmann::json decode_bencoded_list(std::string const &input, size_t &position);
