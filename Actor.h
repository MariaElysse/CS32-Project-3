#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;
const int FRACKMAN_START_X = 30;
const int FRACKMAN_START_Y = 60;
const int FRACKMAN_HITPOINTS = 10;
const int FRACKMAN_START_WATER = 5;
const int FRACKMAN_START_SONAR = 1;
const int FRACKMAN_START_GOLD = 0;
const GraphObject::Direction FRACKMAN_START_DIR = GraphObject::right;

const float DEFAULT_SIZE = 1.0;
const int FOREGROUND = 0;

const int DISCOVERY_DEPTH = 2;
const GraphObject::Direction DISCOVERY_START_DIR = GraphObject::right;

const GraphObject::Direction PROTESTER_START_DIR = GraphObject::left;
const int PROTESTER_START_HITPOINTS = 5;
const int HARDCORE_PROTESTER_HITPOINTS = 20;

const GraphObject::Direction DIRT_DIR = GraphObject::right;
const float DIRT_SIZE = 0.25;
const int BACKGROUND = 3;

const float SQUIRT_SIZE = 1.0;
const int SQUIRT_DEPTH = 1;

const GraphObject::Direction BOULDER_START_DIR = GraphObject::down;
const float BOULDER_SIZE = 1.0;
const int BOULDER_DEPTH = 1;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, Direction startDir, float size, unsigned int depth, StudentWorld *sw);

    virtual float minDistanceFrom(int x, int y);
    virtual void markRemoved();
    virtual bool toBeRemoved();
    virtual ~Actor();
    virtual void doSomething() = 0;

    virtual bool obstructsProtesters(int x, int y);
    StudentWorld *getWorld(void);

    bool validMovement(int &x, int &y, GraphObject::Direction dir);


    bool actThisTick();
private:
    int m_ticksUntilAction;
    int m_ticksBetweenActions;
    StudentWorld *m_world;
    bool m_toBeRemoved;
};

class Person : public Actor {
public:
    Person(int imageId, int startX, int startY, Direction startDir, int hitPoints, StudentWorld *sw);

    virtual bool obstructsProtesters(int x, int y);

    virtual int getHealth();

    virtual void hurt(int damage);
private:
    int m_hitPoints;
};

class FrackMan : public Person {
public:
    FrackMan(StudentWorld *sw);

    bool validMovement(int &x, int &y, GraphObject::Direction dir);
    void doSomething();
private:
    int m_water;
    int m_sonar;
    int m_gold;
    int m_lives;
};

class Protester : public Person {
public:
    Protester(int imageId, int startX, int startY, StudentWorld *sw);
};

class Discovery : public Actor { //oil, gold, SONAR kit, Water.
public:
    Discovery(int imageId, int locX, int locY, StudentWorld *sw);

    virtual void doSomething() = 0;

    virtual bool isAlive() = 0;

    virtual ~Discovery() { };

    virtual bool obstructsProtesters(int x, int y);
};

class OilBarrel : public Discovery {
public:
    OilBarrel(int locX, int locY, StudentWorld *sw);

    void doSomething();
};

class Dirt : public Actor {
public:
    Dirt(int locX, int locY, StudentWorld *sw);
    void doSomething();

    bool obstructsProtesters(int x, int y);
    ~Dirt();
};

class Squirt : public Actor {
public:
    Squirt(int startX, int startY, GraphObject::Direction dir, StudentWorld *sw);

    void doSomething();
private:
    int m_distanceRemaining;
};

class Boulder : public Actor {
public:
    Boulder(int startX, int startY, StudentWorld *sw);

    void doSomething();

    bool dirtOrProtesterBelow();

private:
    bool m_mobile;
};
#endif // ACTOR_H_


