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

class PenaltyDice : public AbstractDice {
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

class BonusDice : public AbstractDice {
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

double value_probabilty(AbstractDice &d, unsigned value, unsigned number_of_rolls = 1) {
    unsigned accum = 0;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt) {
        if (d.roll() == value) accum += 1;
    }
    return static_cast<double>(accum) / number_of_rolls;
}

double expected_value(AbstractDice &d, unsigned number_of_rolls = 1) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += d.roll();
    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

int main() {

    int max = 100, three_max = 6, number_of_rolls = 100000;

    Dice alpha(max, 12414);
    Dice beta(max, 12423414);
    Dice gamma(max, 122414);

    ThreeDicePool three(alpha, beta, gamma);

    PenaltyDice penalty(alpha); 
    BonusDice bonus(alpha);

    std::cout << expected_value(alpha, number_of_rolls) << " ";
    std::cout << expected_value(penalty, number_of_rolls) << " ";
    std::cout << expected_value(bonus, number_of_rolls) << std::endl;
    std::ofstream data("second.csv");

    data << "Target\tStandart\tThree\tPenalty\tBonus" << std::endl;

    for (int target = 1; target < max; target++) {
        data << target << "\t" << value_probabilty(alpha, target, number_of_rolls) << "\t";
        data << value_probabilty(three, target, number_of_rolls) << "\t";
        data << value_probabilty(penalty, target, number_of_rolls) << "\t";
        data << value_probabilty(bonus, target, number_of_rolls);
        data << std::endl;
    }

    data.close();

    return 0;
}