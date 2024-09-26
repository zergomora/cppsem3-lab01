#include <random>
#include <iostream>

class Dice {
public:
    Dice(unsigned max, unsigned seed):
        max(max), dstr(1, max), reng(seed) {}
    
    virtual unsigned roll() {
        return dstr(reng);
    }
    double expected_value(unsigned number_of_rolls = 1) {
        auto accum = 0llu;
        for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
            accum += Dice::roll();
        return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
    }
protected: // чтобы появился доступ в дочерних классах к dstr
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

class ThreeDicePool : public Dice {
public:
    ThreeDicePool(unsigned max,
        unsigned seed_1, unsigned seed_2, unsigned seed_3):
        Dice(max, seed_1),
        dstr_2(1, max), dstr_3(1, max),
        reng_2(seed_2), reng_3(seed_3) {}
    unsigned roll() {
        //return dstr(reng) + dstr_2(reng_2) + dstr_3(reng_3); - такой вариант оказался тоже нормальным
        return Dice::roll() + dstr_2(reng_2) + dstr_3(reng_3); 
    }
private:
    std::uniform_int_distribution<unsigned> dstr_2, dstr_3;
    std::default_random_engine reng_2, reng_3;
};

int main() {

    int max = 6, attemps = 10000;

    Dice one(max, 12414);
    ThreeDicePool three(max, 1231323, 321213, 2113213);

    std::cout << "Dice roll: " << one.roll() << std::endl;
    std::cout << "Three dice rolls: " << three.roll() << std::endl;

    std::cout << "Expected value for Dice: " << one.expected_value(attemps) << std::endl;
    std::cout << "Expected value for ThreeDicePool: " << three.expected_value(attemps) << std::endl;

    return 0;
}