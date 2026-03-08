#pragma once

// Task 2.1 - Barrel class for the alcohol/water mixing problem.
//
// Problem: two barrels of known volume - one filled with 96% alcohol,
// the other with water. Each iteration: scoop 1L from barrel A into barrel B,
// then scoop 1L from barrel B into barrel A. Find the iteration at which the
// alcohol concentration in barrel A drops below 50%.

class Barrel {
private:
    double volume;       // total volume currently in the barrel (litres)
    double alch_amount;  // amount of pure alcohol in the barrel (litres)

public:

    // Initialise with a given total volume and initial alcohol concentration (0..1)
    Barrel(double volume, double alch_concentration);

    // Returns current alcohol concentration as a fraction (0..1)
    double get_concentration() const;

    // Scoop cup_volume litres out of 'other' and pour them into this barrel
    void pour_from(Barrel& other, double cup_volume);

    double get_volume() const;
};