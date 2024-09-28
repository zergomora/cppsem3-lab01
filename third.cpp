#include <random>
#include <fstream>
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

class PenaltyDice : virtual public AbstractDice {
public:
    PenaltyDice(AbstractDice &dice) :
        dice(dice) {}
    unsigned roll() {
        auto first = dice.roll(), second = dice.roll();
        return first <= second ? first : second;
    }
private:
    AbstractDice &dice;
};

class BonusDice : virtual public AbstractDice {
public:
    BonusDice(AbstractDice &dice) :
        dice(dice) {}
    unsigned roll() {
        auto first = dice.roll(), second = dice.roll();
        return first >= second ? first : second;
    }
private:
    AbstractDice &dice;
};

class DoubleDice : public PenaltyDice, public BonusDice {
public:
    DoubleDice(AbstractDice& dice) : 
        PenaltyDice(dice), BonusDice(dice) {}
    unsigned roll() {
        return (PenaltyDice::roll() + BonusDice::roll()) / 2;  
    }
};

double expected_value(AbstractDice &d, unsigned number_of_rolls = 1) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += d.roll();
    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

double value_probabilty(AbstractDice &d, unsigned value, unsigned number_of_rolls = 1) {
    unsigned accum = 0;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt) {
        if (d.roll() == value) accum += 1;
    }
    return static_cast<double>(accum) / number_of_rolls;
}

int main() {

    int max = 100, number_of_rolls = 1000000;

    Dice base(max, 1123);
    DoubleDice child(base);

    std::cout << "Expected value: " << expected_value(child, number_of_rolls) << std::endl;

    std::ofstream data("third.csv");
    
    data << "Target\tDouble" << std::endl;

    for (int target = 1; target < max; target++) {
        data << target << "\t" << value_probabilty(child, target, number_of_rolls) << std::endl;
    }
    data.close();

    return 0;
}
