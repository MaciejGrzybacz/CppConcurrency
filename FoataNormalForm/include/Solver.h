#pragma once

#include <set>
#include <string_view>
#include <unordered_set>
#include <vector>
#include "InputParser.h"

/**
 * @brief Solver for analyzing transaction dependencies and generating Foata Normal Form
 * 
 * This class is responsible for:
 * - Determining dependencies between transactions
 * - Calculating Foata Normal Form (FNF)
 * - Building and visualizing dependency graphs
 * - Analyzing transaction relations
 */
class Solver {
public:
    /**
     * @brief Constructs a new Solver object
     * @param path Path to the input file containing transaction definitions
     */
    explicit Solver(std::string_view path);

    /**
     * @brief Executes the complete analysis workflow
     * 
     * This method:
     * 1. Parses the input
     * 2. Determines relations between transactions
     * 3. Calculates FNF
     * 4. Builds dependency graph
     */
    void run();

    /**
     * @brief Prints the analysis results including dependencies and FNF
     */
    void print_results();

private:
    /**
     * @brief Determines dependency and independency relations between transactions
     * 
     * Analyzes transaction variables to establish which transactions are dependent
     * on each other and which can be executed independently.
     */
    void determine_relations();

    /**
     * @brief Calculates the Foata Normal Form for the given word
     * 
     * Generates the FNF representation by grouping independent actions
     * that can be executed concurrently.
     */
    void calculate_fnf();

    /**
     * @brief Builds and saves a dependency graph in DOT format
     * 
     * Creates a visual representation of transaction dependencies
     * and saves it as a DOT file for visualization.
     */
    void build_dependency_graph();

    /**
     * @brief Checks if there exists a path between two nodes in the dependency graph
     * @param adj Adjacency matrix representing the graph
     * @param start Starting node
     * @param end End node
     * @param visited Vector tracking visited nodes
     * @return true if a path exists, false otherwise
     */
    bool has_path(const std::vector<std::vector<bool>>& adj, int start, int end,
                  std::vector<bool>& visited);

    InputParser parser_;
    std::unordered_map<char, std::set<char>> dependency_relations_;
    std::unordered_map<char, std::set<char>> independent_relations_;
    std::vector<std::vector<char>> fnf_;
};