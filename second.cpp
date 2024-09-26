#include <random>
#include <fstream>

class Dice {
public:
    Dice(unsigned max, unsigned seed):
        max(max), dstr(1, max), reng(seed) {}
    
    virtual unsigned roll() {
        return dstr(reng);
    }
    double value_probabilty(unsigned value, unsigned number_of_rolls = 1) {
        unsigned accum = 0;
        for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt) {
            if (Dice::roll() == value) accum += 1;
        }
        return static_cast<double>(accum) / number_of_rolls;
    }
protected: 
    unsigned max;
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
private:
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
private:
    std::uniform_int_distribution<unsigned> dstr_2;
    std::default_random_engine reng_2;
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

    int max = 100, three_max = 6, number_of_rolls = 100000;

    Dice standart(max, 132);
    ThreeDicePool three(three_max, 13234, 2434, 1213);

    PenaltyDice penalty(max, 123, 3112); 
    BonusDice bonus(max, 1231, 1234234);

    std::ofstream data("second.csv");

    data << "Target\tStandart\tThree\tPenalty\tBonus" << std::endl;

    for (int target = 1; target < max; target++) {
        data << target << "\t" << standart.value_probabilty(target, number_of_rolls) << "\t";
        data << three.value_probabilty(target, number_of_rolls) << "\t";
        data << penalty.value_probabilty(target, number_of_rolls) << "\t";
        data << bonus.value_probabilty(target, number_of_rolls);
        data << std::endl;
    }

    data.close();

    return 0;
}