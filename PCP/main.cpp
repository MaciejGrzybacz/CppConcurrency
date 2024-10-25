#include "include/ProducerConsumerProblem.hpp"
#include "include/ProcessChain.hpp"

void runAllPCP() {
//    std::cout  << std::endl << "One to one: " << std::endl;
//    runOneToOne();
//    std::cout  << std::endl << "One to many: " << std::endl;
//    runOneToMany();
//    std::cout  << std::endl << "Many to one: " << std::endl;
//    runManyToOne();
    std::cout  << std::endl << "Many to many: " << std::endl;
    runManyToMany();
}
int main() {
    Simulation simulation;
    simulation.run();
}