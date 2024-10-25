#include "PhilosopherFactory.hpp"

class DinnerManager {
public:
    explicit DinnerManager(PhilosopherType type);
    
    void dine();
    
private:
    std::vector<std::shared_mutex> mutexes;
    std::vector<std::unique_ptr<Philosopher>> philosophers;
};
