#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

const int FRACKMAN_START_X = 30;
const int FRACKMAN_START_Y = 60;
const float DEFAULT_SIZE = 1.0;
const int DISCOVERY_DEPTH = 2;
const GraphObject::Direction FRACKMAN_START_DIR = GraphObject::right;
const GraphObject::Direction DISCOVERY_START_DIR = GraphObject::right;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, Direction startDir, float size = 1.0, unsigned int depth = 0)
            : GraphObject(imageID, startX, startY, startDir, size, depth) { }
};

class FrackMan : public Actor {
public:
    FrackMan(int imageID, int startX, int startY);

};

class Discovery : public Actor { //oil, gold, SONAR kit, Water.
public:
    Discovery(int imageId, int locX, int locY) : Actor(imageId, locX, locY, DISCOVERY_START_DIR, DEFAULT_SIZE,
                                                       DISCOVERY_DEPTH) { }

    virtual void doSomething() = 0;

    virtual bool isAlive() = 0;
};

class OilBarrel : public Discovery {
public:
    OilBarrel(int locX, int locY);

    void doSomething();
};

class Dirt : public Actor
#endif // ACTOR_H_


