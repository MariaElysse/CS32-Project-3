#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor
Actor::Actor(int imageID, int startX, int startY, GraphObject::Direction startDir, float size, unsigned int depth,
             StudentWorld *sw)
        : GraphObject(imageID, startX, startY, startDir, size, depth), m_world(sw) {

}

Actor::~Actor() { }

//Person
Person::Person(int imageId, int startX, int startY, GraphObject::Direction startDir, int hitPoints, StudentWorld *sw)
        : Actor(imageId, startX, startY, startDir, DEFAULT_SIZE, FOREGROUND, sw), m_hitPoints(hitPoints) {
    setVisible(true);
}
//Discovery

Discovery::Discovery(int imageId, int locX, int locY, StudentWorld *sw)
        : Actor(imageId, locX, locY, DISCOVERY_START_DIR, DEFAULT_SIZE, DISCOVERY_DEPTH, sw) {

}

//FrackMan
FrackMan::FrackMan(StudentWorld *sw)
        : Person(IID_PLAYER, FRACKMAN_START_X, FRACKMAN_START_Y, FRACKMAN_START_DIR, FRACKMAN_HITPOINTS, sw),
          m_gold(0), m_sonar(1), m_water(5) {
}

void FrackMan::doSomething() {
    int keyp;
    if (!getWorld()->getKey(keyp))
        return;

    switch (keyp) {
        case KEY_PRESS_LEFT:
            if (getX() > 4)
                moveTo(getX() - 4, getY());
            setDirection(left);
            break;
        case KEY_PRESS_RIGHT:
            if (getX() < 60)
                moveTo(getX() + 4, getY());
            setDirection(right);
            break;
        case KEY_PRESS_UP:
            if (getY() < 60)
                moveTo(getX(), getY() + 4);
            setDirection(up);
            break;
        case KEY_PRESS_DOWN:
            if (getY() > 4)
                moveTo(getX(), getY() - 4);
            setDirection(down);
            break;
    }
}
//OilBarrel
OilBarrel::OilBarrel(int locX, int locY, StudentWorld *sw)
        : Discovery(IID_BARREL, locX, locY, sw) {
    setVisible(false);

}

void OilBarrel::doSomething() {
    if (!this->isAlive())

        return;

}


//Protestor
Protestor::Protestor(int imageId, int startX, int startY, StudentWorld *sw)
        : Person(imageId, startX, startY, PROTESTER_START_DIR, PROTESTOR_START_HITPOINTS, sw) {

}

//Dirt
void Dirt::doSomething() { } //dirt does nothing

Dirt::Dirt(int locX, int locY, StudentWorld *sw)
        : Actor(IID_DIRT, locX, locY, DIRT_DIR, DIRT_SIZE, BACKGROUND, sw) {
    setVisible(true);
}

StudentWorld *Actor::getWorld(void) {
    return m_world;
}
