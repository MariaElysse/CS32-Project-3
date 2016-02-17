#include "Actor.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor
Actor::Actor(int imageID, int startX, int startY, GraphObject::Direction startDir, float size, unsigned int depth,
             StudentWorld *sw)
        : GraphObject(imageID, startX, startY, startDir, size, depth), m_world(sw), m_toBeRemoved(false) {

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
    int newX = getX();
    int newY = getY();
    getWorld()->deleteDirtAt(newX, newY);
    if (!getWorld()->getKey(keyp)) {//If I don't get a keypress, then nothing's going on
        getWorld()->clearDead(); //and I can delete all the garbage I've been saving up
        return; //and quit early.
    }

    GraphObject::Direction dir;
    switch (keyp) { //select the direction we wish to go. Also set the frackman's direction.
        case KEY_PRESS_LEFT:
            dir = GraphObject::left;
            setDirection(left);
            break;
        case KEY_PRESS_RIGHT:
            dir = GraphObject::right;
            setDirection(right);
            break;
        case KEY_PRESS_UP:
            dir = GraphObject::up;
            setDirection(up);
            break;
        case KEY_PRESS_DOWN:
            dir = GraphObject::down;
            setDirection(down);
            break;
        default:
            dir = GraphObject::none;
            break; //???how??? did you get here???
    }
    getWorld()->validMovement(newX, newY, dir);
    moveTo(newX, newY);
}
//OilBarrel
OilBarrel::OilBarrel(int locX, int locY, StudentWorld *sw)
        : Discovery(IID_BARREL, locX, locY, sw) {
    setVisible(false);
}

void OilBarrel::doSomething() {
    if (!this->isAlive())
        return; //well if it's dead I don't really expect a lot out of it.

}


//Protester - incidentally, these were initially named "Protestors", as the git history can tell you.
Protester::Protester(int imageId, int startX, int startY, StudentWorld *sw)
        : Person(imageId, startX, startY, PROTESTER_START_DIR, PROTESTER_START_HITPOINTS, sw) {

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

void Actor::markRemoved() {
    m_toBeRemoved = true;
    this->setVisible(false);
}

bool Actor::toBeRemoved() {
    return m_toBeRemoved;
}