#include <semaphore>
#include "Philosopher.hpp"

class NaivePhilosopher : public Philosopher {
public:
    using Philosopher::Philosopher;

    void eat() override;
};

class StarvingPhilosopher : public Philosopher {
public:
    using Philosopher::Philosopher;

    void eat() override;
};

class AsymmetricPhilosopher : public Philosopher {
public:
    using Philosopher::Philosopher;

    void eat() override;
};

class ArbiterPhilosopher : public Philosopher {
public:
    ArbiterPhilosopher(std::shared_mutex &m1, std::shared_mutex &m2, int id) : 
        Philosopher(m1,m2, id),arbiter((4)) {};

    void eat() override;
    
private:
    std::counting_semaphore<4> arbiter;
};