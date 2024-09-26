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
    virtual double expected_value(unsigned number_of_rolls = 1) {
        auto accum = 0llu;
        for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
            accum += Dice::roll();
        return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
    }
    virtual double value_probabilty(unsigned value, unsigned number_of_rolls = 1) {
        unsigned accum = 0;
        for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt) {
            if (Dice::roll() == value) accum += 1;
        }
        return static_cast<double>(accum) / number_of_rolls;
    
    }
    unsigned max, seed;
protected:
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

class PenaltyDice : public Dice {
public:
    PenaltyDice(unsigned max, 
        unsigned seed_1, unsigned seed_2):
        Dice(max, seed_1),
        dstr_2(1, max),
        reng_2(seed_2) {}

    unsigned roll() {
        return dstr(reng) >= dstr_2(reng_2) ? dstr(reng) : dstr_2(reng_2);
    }
protected:
    std::uniform_int_distribution<unsigned> dstr_2;
    std::default_random_engine reng_2;
};

class BonusDice : public Dice {
public:
    BonusDice(unsigned max, 
        unsigned seed_1, unsigned seed_2):
        Dice(max, seed_1),
        dstr_2(1, max),
        reng_2(seed_2) {}

    unsigned roll() {
        return dstr(reng) <= dstr_2(reng_2) ? dstr(reng) : dstr_2(reng_2);
    }
protected:
    std::uniform_int_distribution<unsigned> dstr_2;
    std::default_random_engine reng_2;
};

class DoubleDice : public PenaltyDice, public BonusDice {
public:
    DoubleDice(Dice& dice) : 
        PenaltyDice(dice.max, dice.seed, dice.seed), 
        BonusDice(dice.max, dice.seed, dice.seed) {}
    unsigned roll() {
        unsigned penalty_roll = PenaltyDice::roll();
        unsigned bonus_roll = BonusDice::roll();
        return (penalty_roll + bonus_roll) / 2;  
    }
    double expected_value(unsigned number_of_rolls = 1) {
        auto accum = 0llu;
        for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
            accum += DoubleDice::roll();
        return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
    }
    double value_probabilty(unsigned value, unsigned number_of_rolls = 1) {
        unsigned accum = 0;
        for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt) {
            if (DoubleDice::roll() == value) accum += 1;
        }
        return static_cast<double>(accum) / number_of_rolls;
    
    }
};

int main() {

    int max = 100, number_of_rolls = 1000;

    Dice base(max, 1123);
    DoubleDice child(base);

    std::cout << "Expected value: " << child.expected_value(number_of_rolls) << std::endl;

    std::ofstream data("third.csv");
    
    data << "Target\tDouble" << std::endl;

    for (int target = 1; target < max; target++) {
        data << target << "\t" << child.value_probabilty(target, number_of_rolls) << std::endl;
    }
    data.close();

    return 0;
}
