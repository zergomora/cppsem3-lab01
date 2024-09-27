#include <random>
#include <fstream>
#include <iostream>

class Dice {
public:
    Dice(unsigned max, unsigned seed):
        max(max), seed(seed), dstr(1, max), reng(seed) {}
    virtual unsigned roll() {
        return dstr(reng);
    }
    unsigned max, seed;
protected:
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

class PenaltyDice : virtual public Dice {
public:
    PenaltyDice(unsigned max, 
        unsigned seed):
        Dice(max, seed) {}
    unsigned roll() {
        auto first = Dice::roll(), second = Dice::roll();
        return first >= second ? first : second;
    }
};

class BonusDice : virtual public Dice {
public:
    BonusDice(unsigned max, 
        unsigned seed):
        Dice(max, seed) {}
    unsigned roll() {
        auto first = Dice::roll(), second = Dice::roll();
        return first <= second ? first : second;
    }
private:
    std::uniform_int_distribution<unsigned> dstr_2;
    std::default_random_engine reng_2;
};

class DoubleDice : public PenaltyDice, public BonusDice {
public:
    DoubleDice(Dice& dice) : 
        PenaltyDice(dice.max, dice.seed), 
        BonusDice(dice.max, dice.seed),
        Dice(dice.max, dice.seed) {}
    unsigned roll() {
        return (PenaltyDice::roll() + BonusDice::roll()) / 2;  
    }
};

double expected_value(Dice &d, unsigned number_of_rolls = 1) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += d.roll();
    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

double value_probabilty(Dice &d, unsigned value, unsigned number_of_rolls = 1) {
    unsigned accum = 0;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt) {
        if (d.roll() == value) accum += 1;
    }
    return static_cast<double>(accum) / number_of_rolls;
}

int main() {

    int max = 100, number_of_rolls = 1000;

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
