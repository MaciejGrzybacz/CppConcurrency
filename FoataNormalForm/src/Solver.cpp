#include "Solver.h"
#include <iostream>
#include <queue>
#include <format>
#include <algorithm>
#include <fstream>
#include <stack>

Solver::Solver(std::string_view path) : parser_(path) {}

void Solver::run() {
    parser_.parse_input();
    determine_relations();
    calculate_fnf();
    build_dependency_graph();
}

void Solver::determine_relations() {
    const auto& transactions = parser_.get_transactions();
    for (const auto& [action, transaction] : transactions) {
        dependency_relations_[action].insert(action);

        for (char right_var : transaction.get_right_vars()) {
            const auto& dependent_actions = parser_.get_var_actions(right_var);

            for (char dependent_action : dependent_actions) {
                dependency_relations_[dependent_action].insert(action);
                dependency_relations_[action].insert(dependent_action);
            }
        }
    }

    const auto& alphabet = parser_.get_alphabet();
    for (char action1 : alphabet) {
        for (char action2 : alphabet) {
            if (dependency_relations_[action1].find(action2) == dependency_relations_[action1].end()) {
                independent_relations_[action1].insert(action2);
            }
        }
    }
}

// void Solver::calculate_fnf() {
//     auto word = parser_.get_word();
//     std::vector<bool> used(word.length(), false);
//
//     for (size_t i = 0; i < word.length(); i++) {
//         if (used[i]) continue;
//
//         std::vector<char> bucket;
//         bool can_start_bucket = true;
//
//         for (size_t k = 0; k < i; k++) {
//             if (!used[k] && (dependency_relations_[word[k]].contains(word[i]) ||
//                              dependency_relations_[word[i]].contains(word[k]))) {
//                 can_start_bucket = false;
//                 break;
//             }
//         }
//
//         if (!can_start_bucket) continue;
//
//         bucket.push_back(word[i]);
//         used[i] = true;
//
//         for (size_t j = 0; j < word.length(); j++) {
//             if (used[j] || i == j) continue;
//
//             bool can_add = true;
//             for (char c : bucket) {
//                 if (dependency_relations_[c].contains(word[j]) ||
//                     dependency_relations_[word[j]].contains(c)) {
//                     can_add = false;
//                     break;
//                 }
//             }
//
//             for (size_t k = 0; k < j; k++) {
//                 if (!used[k] && (dependency_relations_[word[k]].contains(word[j]) ||
//                                  dependency_relations_[word[j]].contains(word[k]))) {
//                     can_add = false;
//                     break;
//                 }
//             }
//
//             if (can_add) {
//                 bucket.push_back(word[j]);
//                 used[j] = true;
//             }
//         }
//
//         if (!bucket.empty()) {
//             std::sort(bucket.begin(), bucket.end(), [&word](char a, char b) {
//                 return word.find(a) < word.find(b);
//             });
//             fnf_.push_back(bucket);
//         }
//     }
// }

void Solver::calculate_fnf() {
    std::unordered_map<char, std::stack<char>> stacks;
    std::string word = parser_.get_word();
    auto alphabet = parser_.get_alphabet();

    for(char x : alphabet)
        stacks[x] = {};

    std::reverse(word.begin(), word.end());

    for(char a : word) {
        stacks[a].push(a);
        auto dependent = dependency_relations_[a];
        for(char b : dependent)
            if(a != b) stacks[b].push('*');
    }

    for(int i = 0; i < word.size(); i++) {
        std::vector<char> current;
        for(char x : alphabet)
        {
            if(!stacks[x].empty())
            {
                char top = stacks[x].top();
                if(top != '*')
                    current.push_back(top);
            }
        }

        for(char x : current)
            for(char a : dependency_relations_[x])
                if(!stacks[a].empty())
                    stacks[a].pop();

        if(!current.empty())
            fnf_.push_back(current);
    }
}
bool Solver::has_path(const std::vector<std::vector<bool>>& adj, int start, int end,
                      std::vector<bool>& visited) {
    if (start == end) return true;
    visited[start] = true;

    for (size_t i = 0; i < adj.size(); i++) {
        if (adj[start][i] && !visited[i]) {
            if (has_path(adj, i, end, visited)) {
                return true;
            }
        }
    }
    return false;
}

void Solver::build_dependency_graph() {
    auto word = parser_.get_word();
    size_t n = word.length();

    std::vector<std::vector<bool>> adj(n, std::vector<bool>(n, false));

    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (dependency_relations_[word[i]].contains(word[j]) ||
                dependency_relations_[word[j]].contains(word[i])) {
                adj[i][j] = true;
            }
        }
    }

    std::ofstream dot_file("../dependency_graph.dot");
    dot_file << "digraph g {\n";

    for (size_t i = 0; i < n; i++) {
        dot_file << (i + 1) << "[label=" << word[i] << "]\n";
    }

    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (adj[i][j]) {
                adj[i][j] = false;

                std::vector<bool> visited(n, false);
                bool other_path = has_path(adj, i, j, visited);

                if (!other_path) {
                    dot_file << (i + 1) << " -> " << (j + 1) << "\n";
                }

                adj[i][j] = true;
            }
        }
    }

    dot_file << "}\n";
}

void Solver::print_results() {
    std::cout << "Dependency Relations (D):\n";
    for (const auto& [first, dependent] : dependency_relations_) {
        for (const auto& second : dependent) {
            std::cout << std::format("({}, {}) ", first, second);
        }
    }

    std::cout << "\n\nIndependent Relations (I):\n";
    for (const auto& [first, dependent] : independent_relations_) {
        for (const auto& second : dependent) {
            std::cout << std::format("({}, {}) ", first, second);
        }
    }

    std::cout << "\n\nFNF: ";
    for (const auto& elem : fnf_) {
        std::cout << "{";
        for (char x : elem) {
            std::cout << x;
        }
        std::cout << "}";
    }
    std::cout << "\n";
}