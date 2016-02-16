#include "StudentWorld.h"
#include "Actor.h"
#include <string>

using namespace std;

GameWorld *createStudentWorld(string assetDir) {
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir) {
    //add to vector m_objects
    //set up dirt
}

int StudentWorld::init() {

    m_fm = new FrackMan(this);
    for (int i = 0; i < VIEW_WIDTH; i++) {
        for (int j = 0; j < VIEW_HEIGHT; j++) {
            m_dirt[i][j] = new Dirt(i, j, this);
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

StudentWorld::~StudentWorld() {
    for (int i = 0; i < VIEW_WIDTH; i++) {
        for (int j = 0; j < VIEW_HEIGHT; j++) {
            delete m_dirt[i][j];
        }
    }
    delete m_fm;
}

void StudentWorld::cleanUp() { }

int StudentWorld::move() {

    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    // decLives();
    m_fm->doSomething();
    //int val;
    /*if (getLives()<=0)
        return GWSTATUS_PLAYER_DIED;
    else*/
    return GWSTATUS_CONTINUE_GAME;
}