#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

const int FRACKMAN_START_X = 30;
const int FRACKMAN_START_Y = 60;
const int FRACKMAN_HITPOINTS = 10;
const GraphObject::Direction FRACKMAN_START_DIR = GraphObject::right;

const float DEFAULT_SIZE = 1.0;
const int FOREGROUND = 0;

const int DISCOVERY_DEPTH = 2;
const GraphObject::Direction DISCOVERY_START_DIR = GraphObject::right;

const GraphObject::Direction PROTESTER_START_DIR = GraphObject::left;
const int PROTESTOR_START_HITPOINTS = 5;
const int HARDCORE_PROTESTOR_HITPOINTS = 20;
const GraphObject::Direction DIRT_DIR = GraphObject::right;
const float DIRT_SIZE = 0.25;
const int BACKGROUND = 3;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, Direction startDir, float size, unsigned int depth);

    virtual ~Actor();

    virtual void doSomething() = 0;

private:

};

class Person : public Actor {
public:
    Person(int imageId, int startX, int startY, Direction startDir, int hitPoints);

private:
    int m_hitPoints;
};

class FrackMan : public Person {
public:
    FrackMan();

    void doSomething() { }

private:
    int m_water;
    int m_sonar;
    int m_gold;
};

class Protestor : public Person {
    Protestor(int imageId, int startX, int startY);
};

class Discovery : public Actor { //oil, gold, SONAR kit, Water.
public:
    Discovery(int imageId, int locX, int locY);

    virtual void doSomething() = 0;

    virtual bool isAlive() = 0;

    virtual ~Discovery() { };
};

class OilBarrel : public Discovery {
public:
    OilBarrel(int locX, int locY);

    void doSomething();
};

class Dirt : public Actor {
public:
    Dirt(int locX, int locY);

    void doSomething();

};
#endif // ACTOR_H_


