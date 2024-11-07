#include "Solver.h"
#include <iostream>
#include <queue>
#include <format>
#include <algorithm>
#include <fstream>
#include <stack>

void Solver::run() {
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

    //adding paths between dependent actions
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (dependency_relations_[word[i]].contains(word[j])) {
                adj[i][j] = true;
            }
        }
    }

    std::ofstream dot_file("../dependency_graph.dot");
    dot_file << "digraph g {\n";
    graph_ += "digraph g {\n";

    for (size_t i = 0; i < n; i++) {
        dot_file << (i + 1) << "[label=" << word[i] << "]\n";
        graph_ += std::format("{}[label={}]\n", i + 1, word[i]);
    }

// Checking every pair of actions for dependencies (i, j) where i < j
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (adj[i][j]) {
                
                //temporarily close path between i and j
                adj[i][j] = false;

                std::vector<bool> visited(n, false);
                bool other_path = has_path(adj, i, j, visited);

                // If there is no other path, then this edge is essential
                if (!other_path) {
                    dot_file << (i + 1) << " -> " << (j + 1) << "\n";
                    graph_ += std::format("{} -> {}\n", i + 1, j + 1);
                }

                adj[i][j] = true;
            }
        }
    }

    dot_file << "}\n";
    graph_ += "}\n";
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
        std::cout << "(";
        for (char x : elem) {
            std::cout << x;
        }
        std::cout << ")";
    }
    std::cout << "\n\n";
    
    std::cout << graph_;
}