#pragma once

#include <set>
#include <string>
#include <iostream>
#include <format>

/**
 * @brief Represents a single transaction with its action, variables, and operations
 * 
 * A Transaction contains:
 * - An action identifier
 * - A left-hand variable
 * - A set of right-hand variables
 */
class Transaction {
public:
    Transaction() = default;

    /**
     * @brief Constructs a new Transaction object
     * @param action The action identifier
     * @param left_var The left-hand variable
     */
    Transaction(char action, char left_var)
            : action_(action), left_var_(left_var) {}

    /**
     * @brief Adds a variable to the right-hand side of the transaction
     * @param var Variable to add
     */
    void add_right_var(char var) { right_vars_.insert(var); }

    [[nodiscard]] char get_action() const noexcept { return action_; }
    [[nodiscard]] char get_left_var() const noexcept { return left_var_; }
    [[nodiscard]] const std::set<char>& get_right_vars() const noexcept { return right_vars_; }

    friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction) {
        os << "(" << transaction.action_ << ") "
           << transaction.left_var_ << " := ";

        bool first = true;
        for (char var : transaction.right_vars_) {
            if (!first) os << " + ";
            os << var;
            first = false;
        }
        return os;
    }

private:
    char action_{};
    char left_var_{};
    std::set<char> right_vars_{};
};