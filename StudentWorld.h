#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <list>
#include <stack>

#define sqr(x) ((x)*(x))
#define min(x, y) ((x) <= (y)) ? (x) : (y)
#define max(x, y) ((x) >= (y)) ? (x) : (y)


//#include <pair> yeah honestly this is probably too much
class Actor;
class Dirt;
class FrackMan;

class Person;

class Squirt;
//struct IntPair;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
const int MINESHAFT_LEFT_WALL_COL = 30;
const int MINESHAFT_RIGHT_WALL_COL = 33;
const int MINESHAFT_BOTTOM_ROW = 4;

const unsigned int BARREL_VALUE = 1000;
const unsigned int SONAR_VALUE = 75;
const int GOLD_VALUE = 10;

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetDir);

    ~StudentWorld();

    virtual int init();

    virtual int move();

    virtual void cleanUp();

    void deleteDirtAt(int x, int y, bool playSound = true);

    void clearDead();

    void insertActor(Actor *toBeAdded);

    void sonarPulse(int x, int y);
    //int getScore();
//
//    void incScore(int amount);
//
//    void resetScore();
//
//    int getLevel(); //on my
//
//    void incLevel();

    bool obstructionAt(int x, int y);

    bool dirtAt(int x, int y);

    void damageOneActorAt(int x, int y, Squirt *squirt);

    void damageAllActorsAt(int x, int y);

    float frackManCornerRadius(Actor *actor);

    GraphObject::Direction frackManDirection();

    bool lineOfSightWithFrackMan(Person *person);

    void damageFrackMan(int damage);

    GraphObject::Direction directionToFrackMan(Person *person);
    void gotOil();

    GraphObject::Direction leaveOilField(int &x, int &y);
    int amtOfOil();

    void gotGold();

    void gotWater();
    void sonarDespawned(bool byFrackMan);


private:
    struct IntPair {
        IntPair(int i, int j) : i(i), j(j) { }

        int i;
        int j;
    };

    StudentWorld::IntPair getRandomMineshaftCoord();
    std::string setDisplayText(void);


    StudentWorld::IntPair getRandomActorLocation();

    template<class T>
    void addRandomDiscoveries(int num);
    //int m_score;
    int m_level;
    int m_barrelsRemaining;
    std::list<Actor *> m_objects;
    FrackMan *m_fm;
    Dirt *m_dirt[VIEW_HEIGHT][VIEW_WIDTH];
    int m_distanceMaps[VIEW_HEIGHT][VIEW_WIDTH];
    bool m_dirtDeleted;
    int m_oil;
    bool m_sonarPresent;
    std::stack<IntPair> dirtToBeDeleted; //[i][j] values for the dirt that is to be deleted the next time we get a chance.
    bool initialized;

    GraphObject::Direction minAroundDirection(int x, int y);

    int minAround(int x, int y);

    void mapCurrentPaths(int x, int y, int fromX, int fromY);

};

#endif // STUDENTWORLD_H_
