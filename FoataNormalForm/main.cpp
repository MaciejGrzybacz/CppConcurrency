#include "Solver.h"

int main() {
    Solver solver("../data/case0.txt");
    solver.run();
    solver.print_results();
    return 0;
}
