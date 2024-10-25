#include "Dinner.hpp"

int main() {
    DinnerManager dinnerManager(PhilosopherType::Arbiter);
    dinnerManager.dine();
    return 0;
}