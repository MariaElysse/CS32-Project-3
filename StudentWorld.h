#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetDir);


    virtual int init() {

        return GWSTATUS_CONTINUE_GAME;
    }

    virtual int move() {
        // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    virtual void cleanUp() {
    }

private:
    std::vector<Actor> m_objects;
    FrackMan *m_fm;
    Dirt *m_dirt;
};

#endif // STUDENTWORLD_H_
