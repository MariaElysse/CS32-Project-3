#include "StudentWorld.h"
#include "Actor.h"

using namespace std;

GameWorld *createStudentWorld(string assetDir) {
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir), thing_deleted(false) {
    //add to vector m_objects
    //set up dirt
}

int StudentWorld::init() {

    m_fm = new FrackMan(this);
    for (int i = 0; i < VIEW_WIDTH; i++) { //fill the entire world with dirt (me_irl)
        for (int j = 0; j < VIEW_HEIGHT; j++) {
            if (j < (VIEW_HEIGHT - SPRITE_HEIGHT) && (i < 29 || i > 32)) //except the column in the centre
                m_dirt[i][j] = new Dirt(i, j, this);
            else
                m_dirt[i][j] = nullptr;
            if (j <= 4 && !m_dirt[i][j]) //except the bottom of the middle column.
                m_dirt[i][j] = new Dirt(i, j, this);
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

StudentWorld::~StudentWorld() { //erase all the dirt. et cetera.
    cleanUp();
    delete m_fm;
    m_fm = nullptr;

}

void StudentWorld::cleanUp() { //erase all the dirt.
    for (int i = 0; i < VIEW_WIDTH; i++) {
        for (int j = 0; j < VIEW_HEIGHT; j++) {
            delete m_dirt[i][j];
            m_dirt[i][j] = nullptr;
        }
    }
}

int StudentWorld::move() {

    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    // decLives();
    m_fm->doSomething();
    clearDead();
    //int val;
    /*if (getLives()<=0)
        return GWSTATUS_PLAYER_DIED;
    else*/
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::clearDead() {
    if (!thing_deleted) //allows for lazy evaluation, and the game to not be slow as molasses
        return; //very important to set this every time you call Actor::markRemoved()
    thing_deleted = false;
    //delete all the objects marked as dead.
    for (int i = 0; i < m_objects.size(); i++) {
        Actor *object = m_objects[i];
        Actor *tmp = object;
        if (object->toBeRemoved()) {
            m_objects.erase(m_objects.begin() + i);
            //TODO: fix this shit to actually use a god damn iterator
        }
        delete tmp;
    }
    while (!dirtToBeDeleted.empty()) { //removed a nested for loop: winning.
        StudentWorld::IntPair toDelete = dirtToBeDeleted.top();
        dirtToBeDeleted.pop();
        delete m_dirt[toDelete.i][toDelete.j];
        m_dirt[toDelete.i][toDelete.j] = nullptr;
    }
}


bool StudentWorld::validMovement(int &x, int &y, GraphObject::Direction direction) {
    //determine if a Person at a particular place can make a particular movement.
    //the integers passed in by reference actually change, to represent the movement.
    switch (direction) {
        case GraphObject::up:
            if (y < VIEW_HEIGHT - SPRITE_HEIGHT) {
                y = y + 1;
                return true;
            }
            break;
        case GraphObject::down:
            if (y > 0) {
                y = y - 1;
                return true;
            }
            break;
        case GraphObject::left:
            if (x > 0) {
                x = x - 1;
                return true;
            }
            break;
        case GraphObject::right:
            if (x < VIEW_WIDTH - SPRITE_WIDTH) {
                x = x + 1;
                return true;
            }
            break;
        default:
            return false;
    }
    return false;
}

void StudentWorld::deleteDirtAt(int x, int y) { //this can be made4x faster
// by only erasing the dirt I'mwaslking into
    //eraseall the dirt that the FrackMan is standing on
    //or,more generally, erase all the dirt in a 4x4 square
    // given by its bottom-right coordinate, x,y
    for (int i = x + 3; i >= x; i--) {
        for (int j = y + 3; j >= y; j--) {
            if (m_dirt[i][j] && i >= 0 && j >= 0) {
                m_dirt[i][j]->markRemoved();
                IntPair toDelete;
                toDelete.i = i;
                toDelete.j = j;
                dirtToBeDeleted.push(toDelete);
            }
        }
    }
    thing_deleted = true;
}
