#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor
Actor::Actor(int imageID, int startX, int startY, GraphObject::Direction startDir, float size, unsigned int depth)
        : GraphObject(imageID, startX, startY, startDir, size, depth) {

}

Actor::~Actor() { }

//Person
Person::Person(int imageId, int startX, int startY, GraphObject::Direction startDir, int hitPoints)
        : Actor(imageId, startX, startY, startDir, DEFAULT_SIZE, FOREGROUND), m_hitPoints(hitPoints) {

}
//Discovery

Discovery::Discovery(int imageId, int locX, int locY)
        : Actor(imageId, locX, locY, DISCOVERY_START_DIR, DEFAULT_SIZE, DISCOVERY_DEPTH) {

}

//FrackMan
FrackMan::FrackMan()
        : Person(IID_PLAYER, FRACKMAN_START_X, FRACKMAN_START_Y, FRACKMAN_START_DIR, FRACKMAN_HITPOINTS),
          m_gold(0), m_sonar(1), m_water(5) {
}

//OilBarrel
OilBarrel::OilBarrel(int locX, int locY)
        : Discovery(IID_BARREL, locX, locY) {
    setVisible(false);

}

void OilBarrel::doSomething() {
    if (!this->isAlive())
        return;

}


//Protestor
Protestor::Protestor(int imageId, int startX, int startY)
        : Person(imageId, startX, startY, PROTESTER_START_DIR, PROTESTOR_START_HITPOINTS) {

}

//Dirt
void Dirt::doSomething() { } //dirt does nothing

Dirt::Dirt(int locX, int locY)
        : Actor(IID_DIRT, locX, locY, DIRT_DIR, DIRT_SIZE, BACKGROUND) {
    setVisible(true);
}
