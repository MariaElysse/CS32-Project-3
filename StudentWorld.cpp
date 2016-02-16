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
        for (int j = 0; i < VIEW_HEIGHT; j++) {
            m_dirt[i][j] = new Dirt(i, j, this);
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

StudentWorld::~StudentWorld() {
    for (int i = 0; i < VIEW_WIDTH; i++) {
        for (int j = 0; i < VIEW_HEIGHT; j++) {
            delete m_dirt[i][j];
        }
    }
}