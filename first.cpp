#include <random>
#include <iostream>

struct AbstractDice {
    virtual ~AbstractDice() {}
    virtual unsigned roll() = 0;
};

class Dice : public AbstractDice {
public:
    Dice(unsigned max, unsigned seed):
        max(max), dstr(1, max), reng(seed) {}
    unsigned roll() {
        return dstr(reng);
    }
private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

class ThreeDicePool : public AbstractDice {
public:
    ThreeDicePool(AbstractDice &dice_1, AbstractDice &dice_2, AbstractDice &dice_3) :
        dice_1(dice_1), dice_2(dice_2), dice_3(dice_3) {}
    unsigned roll() {
        return dice_1.roll() + dice_2.roll() + dice_3.roll();
    }
private:
    AbstractDice &dice_1, &dice_2, &dice_3;
};

double expected_value(AbstractDice &d, unsigned number_of_rolls = 1) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += d.roll();
    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

int main() {

    int max = 6, attemps = 10000;

    Dice alpha(max, 12414);
    Dice beta(max, 12423414);
    Dice gamma(max, 122414);

    ThreeDicePool three(alpha, beta, gamma);

    std::cout << "Dice roll: " << alpha.roll() << std::endl;
    std::cout << "Three dice rolls: " << three.roll() << std::endl;

    std::cout << "Expected value for Dice: " << expected_value(alpha, attemps) << std::endl;
    std::cout << "Expected value for ThreeDicePool: " << expected_value(three, attemps) << std::endl;

    return 0;
}