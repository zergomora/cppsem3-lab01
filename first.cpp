#include <random>
#include <iostream>

class Dice {
public:
    Dice(unsigned max, unsigned seed):
        max(max), dstr(1, max), reng(seed) {}
    virtual unsigned roll() {
        return dstr(reng);
    }
private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

class ThreeDicePool : public Dice {
public:
    ThreeDicePool(unsigned max,
        unsigned seed):
        Dice(max, seed) {}
    unsigned roll() {
        return Dice::roll() + Dice::roll() + Dice::roll(); 
    }
};

double expected_value(Dice &d, unsigned number_of_rolls = 1) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += d.roll();
    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

int main() {

    int max = 6, attemps = 10000;

    Dice one(max, 12414);
    ThreeDicePool three(max, 1231323);

    std::cout << "Dice roll: " << one.roll() << std::endl;
    std::cout << "Three dice rolls: " << three.roll() << std::endl;

    std::cout << "Expected value for Dice: " << expected_value(one, attemps) << std::endl;
    std::cout << "Expected value for ThreeDicePool: " << expected_value(three, attemps) << std::endl;
    
    return 0;
}