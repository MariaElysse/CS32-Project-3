#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <vector>
#include <stack>
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

    void clearDead();
private:
    std::string setDisplayText(void);
    struct IntPair {
        IntPair(int i, int j) : i(i), j(j) { }
        int i;
        int j;
    };
    std::vector<Actor *> m_objects;
    FrackMan *m_fm;
    Dirt *m_dirt[VIEW_HEIGHT][VIEW_WIDTH];
    bool thing_deleted;
    std::stack<IntPair> dirtToBeDeleted; //[i][j] values for the dirt that is to be deleted in the next tick.
};

#endif // STUDENTWORLD_H_
