#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
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

    virtual int move();

    virtual void cleanUp();

    void deleteDirtAt(int x, int y);

    bool validMovement(int &x, int &y, GraphObject::Direction);
private:
    std::vector<Actor *> m_objects;
    FrackMan *m_fm;
    Dirt *m_dirt[VIEW_HEIGHT][VIEW_WIDTH];

    void clearDead();
};

#endif // STUDENTWORLD_H_
