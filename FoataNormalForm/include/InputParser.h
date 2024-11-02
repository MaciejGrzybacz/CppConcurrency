#pragma once

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Transaction.h"

/**
 * @brief Parser for processing input files containing transaction definitions and configuration
 * 
 * This class is responsible for reading and parsing input files that contain:
 * - Transaction definitions with actions and variables
 * - Alphabet definitions
 * - Word sequences
 */
class InputParser {
public:
    /**
     * @brief Constructs a new Input Parser object
     * @param path Path to the input file to be parsed
     * @throws std::runtime_error if the file cannot be opened
     */
    explicit InputParser(std::string_view path);

    ~InputParser() = default;
    InputParser(const InputParser&) = delete;
    InputParser& operator=(const InputParser&) = delete;
    InputParser(InputParser&&) noexcept = default;
    InputParser& operator=(InputParser&&) noexcept = default;

    /**
     * @brief Parses the input file and populates internal data structures
     * @throws std::runtime_error if the file format is invalid or if parsing fails
     */
    void parse_input();

    /**
     * @brief Gets the set of actions associated with a given variable
     * @param key The variable to look up
     * @return const std::set<char>& Reference to the set of actions
     */
    [[nodiscard]] const std::set<char>& get_var_actions(char key) const;

    [[nodiscard]] const std::unordered_map<char, Transaction>& get_transactions() const noexcept { return transactions_; }
    [[nodiscard]] const std::vector<char>& get_alphabet() const noexcept { return alphabet_; }
    [[nodiscard]] const std::string& get_word() const noexcept { return word_; }

private:
    /**
     * @brief Processes a single line from the input file
     * @param line Line to process
     * @throws std::runtime_error if the line format is invalid
     */
    void process_line(std::string_view line);

    /**
     * @brief Handles parsing of a transaction definition line
     * @param line Line containing transaction definition
     * @throws std::runtime_error if the transaction format is invalid
     */
    void handle_transaction(std::string_view line);

    /**
     * @brief Handles parsing of the alphabet definition line
     * @param line Line containing alphabet definition
     * @throws std::runtime_error if the alphabet format is invalid
     */
    void handle_alphabet(std::string_view line);

    /**
     * @brief Handles parsing of the word definition line
     * @param line Line containing word definition
     * @throws std::runtime_error if the word format is invalid
     */
    void handle_word(std::string_view line);

    std::unordered_map<char, Transaction> transactions_;
    std::unordered_map<char, std::set<char>> var_actions_;
    std::vector<char> alphabet_;
    std::string word_;
    const std::string filename_;
};