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
const int PROTESTER_DAMAGE = 2;
const int HARDCORE_PROTESTER_HITPOINTS = 20;
const int REGULAR_PROTESTER_VALUE = 100;
const int HARDCORE_PROTESTER_VALUE = 250;

const GraphObject::Direction DIRT_DIR = GraphObject::right;
const float DIRT_SIZE = 0.25;
const int BACKGROUND = 3;

const float SQUIRT_SIZE = 1.0;
const int SQUIRT_DEPTH = 1;

const GraphObject::Direction BOULDER_START_DIR = GraphObject::down;
const float BOULDER_SIZE = 1.0;
const int BOULDER_DEPTH = 1;

const int SONAR_START_X = 0;
const int SONAR_START_Y = 60;

const int SQUIRT_DAMAGE = 2;
const int BOULDER_DAMAGE = 100;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, Direction startDir, float size, unsigned int depth, StudentWorld *sw);

    virtual void markRemoved();
    virtual bool toBeRemoved();
    virtual ~Actor();
    virtual void doSomething() = 0;
    virtual int getHealth();
    virtual bool obstructsProtesters(int x, int y);
    StudentWorld *getWorld(void);

    virtual bool validMovement(int &x, int &y, GraphObject::Direction dir);

    bool actThisTick();

    void setTicks(int ticks);

    virtual void hurt(int damage);

    void setValue(int val);
private:
    int m_ticksUntilAction;

    int m_ticksBetweenActions;
    StudentWorld *m_world;
    bool m_toBeRemoved;
    int m_value;
};

class Person : public Actor {
public:
    Person(int imageId, int startX, int startY, Direction startDir, int hitPoints, StudentWorld *sw);

    bool obstructsProtesters(int x, int y);

    int getHealth();
    virtual void hurt(int damage);

    virtual ~Person();

private:
    int m_hitPoints;
};

class FrackMan : public Person {
public:
    FrackMan(StudentWorld *sw);
    bool validMovement(int &x, int &y, GraphObject::Direction dir);
    void doSomething();
    void addSonar();

    void addGold();

    void addWater();

    int amtWater();

    int amtGold();

    int amtSonar();

    void hurt(int damage);
private:
    int m_water;
    int m_sonar;
    int m_gold;
    int m_lives;
};

class Protester : public Person {
public:
    Protester(int imageId, int startX, int startY, StudentWorld *sw);

    GraphObject::Direction getValidRandomDirection();

    virtual void doSomething();

    void hurt(int damage, bool play);

    bool validMovement(int &x, int &y, GraphObject::Direction dir);

    void stun();

    bool stunned();

    bool makePerpendicularTurn();

    bool moveForward();

    void resetMoveForward();
    bool yellThisTick();

    virtual void markRemoved();

    virtual ~Protester();

private:

    bool m_hardcore;
    int m_nSquaresToMove;
    int m_ticksBetweenYells;
    int m_lastPerpendicularTurn;
    bool m_first_run;
    int m_stunDuration;
};

class HardcoreProtester : public Protester {
public:
    HardcoreProtester(int imageId, int startX, int startY, StudentWorld *sw);
    void doSomething();

    void markRemoved();
    //void doSomething();
};

class Discovery : public Actor { //oil, gold, SONAR kit, Water.
public:
    Discovery(int imageId, int locX, int locY, StudentWorld *sw);

    virtual bool pickedUp();
    virtual void doSomething() = 0;

    virtual ~Discovery() { };

    virtual bool obstructsProtesters(int x, int y);
};

class OilBarrel : public Discovery {
public:
    OilBarrel(int locX, int locY, StudentWorld *sw);
    void doSomething();

    bool pickedUp();
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

    bool validMovement(int &x, int &y, GraphObject::Direction dir);
    void doSomething();
private:
    int m_distanceRemaining;
};

class Boulder : public Actor {
public:
    Boulder(int startX, int startY, StudentWorld *sw);

    void doSomething();

    bool obstructsProtesters(int x, int y);

    bool dirtOrRockBelow();

    ~Boulder() { }

private:
    bool m_mobile;
};

class GoldNugget : public Discovery {
public:
    GoldNugget(int locX, int locY, StudentWorld *sw, bool isBribe = false);

    void doSomething();

    bool isBribe();
private:
    bool m_isBribe;
};
class Sonar : public Discovery {
public:
    Sonar(StudentWorld *sw);

    void doSomething();
};

class Water : public Discovery {
public:
    Water(int locX, int locY, StudentWorld *sw);

    void doSomething();
};
#endif // ACTOR_H_


