#include <random>
#include <fstream>
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

class PenaltyDice : public Dice {
public:
    PenaltyDice(unsigned max, 
        unsigned seed):
        Dice(max, seed) {}
    unsigned roll() {
        auto first = Dice::roll(), second = Dice::roll();
        return first >= second ? first : second;
    }
};

class BonusDice : public Dice {
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

class ThreeDicePool : public Dice {
public:
    ThreeDicePool(unsigned max,
        unsigned seed):
        Dice(max, seed) {}
    unsigned roll() {
        return Dice::roll() + Dice::roll() + Dice::roll(); 
    }
};

double value_probabilty(Dice &d, unsigned value, unsigned number_of_rolls = 1) {
    unsigned accum = 0;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt) {
        if (d.roll() == value) accum += 1;
    }
    return static_cast<double>(accum) / number_of_rolls;
}

int main() {

    int max = 100, three_max = 6, number_of_rolls = 100000;

    Dice standart(max, 132);
    ThreeDicePool three(three_max, 13234);

    PenaltyDice penalty(max, 123); 
    BonusDice bonus(max, 1231);

    std::ofstream data("second.csv");

    data << "Target\tStandart\tThree\tPenalty\tBonus" << std::endl;

    for (int target = 1; target < max; target++) {
        data << target << "\t" << value_probabilty(standart, target, number_of_rolls) << "\t";
        data << value_probabilty(three, target, number_of_rolls) << "\t";
        data << value_probabilty(penalty, target, number_of_rolls) << "\t";
        data << value_probabilty(bonus, target, number_of_rolls);
        data << std::endl;
    }

    data.close();

    return 0;
}