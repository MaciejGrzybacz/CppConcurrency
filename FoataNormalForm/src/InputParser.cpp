#include "InputParser.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <format>

InputParser::InputParser(std::string_view path) : filename_(path) {}

const std::set<char>& InputParser::get_var_actions(char key) const {
    static const std::set<char> empty_set;
    auto it = var_actions_.find(key);
    return it != var_actions_.end() ? it->second : empty_set;
}

void InputParser::parse_input() {
    std::ifstream file(filename_);
    if (!file) {
        throw std::runtime_error(std::format("Cannot open file: {}", filename_));
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            process_line(line);
        }
    }
}

void InputParser::process_line(std::string_view line) {
    if (line.empty()) return;

    if (line[0] == '(') {
        handle_transaction(line);
    } else if (line.starts_with("A = ")) {
        handle_alphabet(line);
    } else if (line.starts_with("w = ")) {
        handle_word(line);
    }
}

void InputParser::handle_transaction(std::string_view line) {
    constexpr size_t MIN_TRANSACTION_LENGTH = 10;
    
    if (line.size() < MIN_TRANSACTION_LENGTH) {
        throw std::runtime_error(
                std::format("Transaction line too short: {}", std::string(line)));
    }

    char action = line[1];
    char left_var = line[4];

    Transaction transaction(action, left_var);
    var_actions_[left_var].insert(action);

    auto assign_pos = line.find(":= ");
    if (assign_pos == std::string_view::npos) {
        throw std::runtime_error(
                std::format("Invalid transaction format: {}", std::string(line)));
    }

    std::string_view right_side = line.substr(assign_pos + 3);
    for (char c : right_side) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            transaction.add_right_var(c);
        }
    }

    transactions_.emplace(action, std::move(transaction));
}

void InputParser::handle_alphabet(std::string_view line) {
    auto start = line.find('{');
    auto end = line.find('}');

    if (start == std::string_view::npos || end == std::string_view::npos) {
        throw std::runtime_error(
                std::format("Invalid alphabet format: {}", std::string(line)));
    }

    std::string_view contents = line.substr(start + 1, end - start - 1);
    for (char c : contents) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            alphabet_.push_back(c);
        }
    }
}

void InputParser::handle_word(std::string_view line) {
    constexpr size_t PREFIX_LENGTH = 4;
    
    if (line.size() < PREFIX_LENGTH) {
        throw std::runtime_error(
                std::format("Word line too short: {}", std::string(line)));
    }
    word_ = std::string(line.substr(PREFIX_LENGTH));
}