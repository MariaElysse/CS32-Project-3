#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

class Actor;

class Dirt;

class FrackMan;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetDir);

    ~StudentWorld();

    virtual int init();

    virtual int move() {
        // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
        decLives();
        // m_fm->doSomething();
        return GWSTATUS_PLAYER_DIED;
    }

    virtual void cleanUp() {
    }

private:
    std::vector<Actor> m_objects;
    FrackMan *m_fm;
    Dirt *m_dirt[VIEW_HEIGHT][VIEW_WIDTH];
};

#endif // STUDENTWORLD_H_
