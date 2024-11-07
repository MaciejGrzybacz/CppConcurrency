#include "Solver.h"

int main() {
    std::string name;
    std::cout << "Enter the name of the test file located in the data directory: " << std::endl;

    std::cin >> name;
            
    InputParser parser("../data/" + name + ".txt");
    parser.parse_input();
    
    Solver solver(parser);
    solver.run();
    solver.print_results();
    
    return 0;
}
