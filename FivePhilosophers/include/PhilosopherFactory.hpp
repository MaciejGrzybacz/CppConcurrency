#include "PhilosopherImpl.hpp"

class PhilosopherFactory {
public:
    static std::unique_ptr<Philosopher> createPhilosopher(
            PhilosopherType type, std::shared_mutex& m1,std::shared_mutex& m2, int id);
};
