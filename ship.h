#ifndef SHIP_H
#define SHIP_H

class Hero;

class Ship {
public:
    Hero* hero;
    Ship(Hero* hero);
};

#endif // SHIP_H
