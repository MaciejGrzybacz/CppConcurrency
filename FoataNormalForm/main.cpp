#include "Solver.h"

int main() {
    Solver solver("../data/case2.txt");
    solver.run();
    solver.print_results();
    return 0;
}
