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
          m_gold(FRACKMAN_START_GOLD), m_sonar(FRACKMAN_START_SONAR), m_water(FRACKMAN_START_WATER) {
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
            break;
        case KEY_PRESS_RIGHT:
            dir = GraphObject::right;
            break;
        case KEY_PRESS_UP:
            dir = GraphObject::up;
            break;
        case KEY_PRESS_DOWN:
            dir = GraphObject::down;
            break;
        default:
            dir = GraphObject::none;
            break; //???how??? did you get here???
    }
    if (dir == getDirection())
        getWorld()->validMovement(newX, newY, dir);
    setDirection(dir);
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
//post-post script Protestor is actually a valid spelling and I want to change it back,
//but I also really want that interview with Symantec
Protester::Protester(int imageId, int startX, int startY, StudentWorld *sw)
        : Person(imageId, startX, startY, PROTESTER_START_DIR, PROTESTER_START_HITPOINTS, sw) {

}

//Dirt
void Dirt::doSomething() { } //dirt does nothing

//Dirt, when made, sits there and is dirt
Dirt::Dirt(int locX, int locY, StudentWorld *sw)
        : Actor(IID_DIRT, locX, locY, DIRT_DIR, DIRT_SIZE, BACKGROUND, sw) {
    setVisible(true);
}

//The World Is Not Enough
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

Dirt::~Dirt() {}

Squirt::Squirt(int startX, int startY, GraphObject::Direction dir, StudentWorld *sw)
        : Actor(IID_WATER_SPURT, startX, startY, dir, SQUIRT_SIZE, SQUIRT_DEPTH, sw) {

}

bool Actor::obstructsProtesters(int x, int y) {
    //if it obstructs the movement of the Protestors
    int tmpX = getX() - x;
    int tmpY = getY() - y;
    return tmpX < SPRITE_WIDTH && tmpX > 0 && tmpY < SPRITE_HEIGHT && tmpY > 0;
    //if tmpX in [0..3] and tmpY in [0..3], the object is blocking the way of the protesters/FrackMan.
    // the "object", of course being just boulders.
}

bool Dirt::obstructsProtesters(int x, int y) {
    return (x == getX() && y == getY());
} //dirt obstructs the protesters, but only in its small square.

bool Discovery::obstructsProtesters(int x, int y) {
    return false;
} //Protesters' pathing isn't blocked by discoveries.

bool Person::obstructsProtesters(int x, int y) {
    return false;
    //Protesters' pathing isn't blocked by each other.
    //They have special interactions with the FrackMan, though I can't see why they would not
    // be allowed to walk through him, they just (should) never actually try to.
    //Also, the FrackMan can pass through protesters.
}


int Person::getHealth() {
    return m_hitPoints;
}

void Person::hurt(int damage) {
    m_hitPoints -= damage;
}
