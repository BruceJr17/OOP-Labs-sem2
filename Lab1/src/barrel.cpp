#include "barrel.h"

Barrel::Barrel(double volume, double alch_concentration)
    : volume(volume), alch_amount(volume* alch_concentration) {
}

double Barrel::get_concentration() const {
    if (volume <= 0) return 0.0;
    return alch_amount / volume;
}

double Barrel::get_volume() const {
    return volume;
}

// Transfer cup_volume litres from 'other' into this barrel.
// If 'other' has less than cup_volume left, we take everything it has.

void Barrel::pour_from(Barrel& other, double cup_volume) {

    // how much can we actually scoop?
    double actual = (cup_volume > other.volume) ? other.volume : cup_volume;

    // alcohol carried in the scooped liquid
    double alch_in_cup = other.get_concentration() * actual;

    // remove from source
    other.volume -= actual;
    other.alch_amount -= alch_in_cup;

    // add to this barrel
    volume += actual;
    alch_amount += alch_in_cup;
}