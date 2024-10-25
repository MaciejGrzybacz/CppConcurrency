#include "PhilosopherFactory.hpp"

std::unique_ptr<Philosopher>
PhilosopherFactory::createPhilosopher(PhilosopherType type, std::shared_mutex &m1, std::shared_mutex &m2, int id) {
    switch (type) {
        case PhilosopherType::Naive:
            return std::make_unique<NaivePhilosopher>(m1, m2, id);
        case PhilosopherType::Starving:
            return std::make_unique<StarvingPhilosopher>(m1, m2, id);
        case PhilosopherType::Asymmetric:
            return std::make_unique<AsymmetricPhilosopher>(m1, m2, id);
        case PhilosopherType::Arbiter:
            return std::make_unique<ArbiterPhilosopher>(m1, m2, id);
        default:
            throw std::invalid_argument("Unknown philosopher type");
    }
}
