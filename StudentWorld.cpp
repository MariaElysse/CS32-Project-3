#include "StudentWorld.h"
#include "Actor.h"

using namespace std;

GameWorld *createStudentWorld(string assetDir) {
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir), m_dirtDeleted(false) {
    //add to vector m_objects
    //set up dirt
}

int StudentWorld::init() {
    m_fm = new FrackMan(this);
    for (int i = 0; i < VIEW_WIDTH; i++) { //fill the entire world with dirt (me_irl)
        for (int j = 0; j < VIEW_HEIGHT; j++) {
            if (j < (VIEW_HEIGHT - SPRITE_HEIGHT) &&
                (i < MINESHAFT_LEFT_WALL_COL || i > MINESHAFT_RIGHT_WALL_COL)) //except the column in the centre
                m_dirt[i][j] = new Dirt(i, j, this);
            else
                m_dirt[i][j] = nullptr;
            if (j < MINESHAFT_BOTTOM_ROW && !m_dirt[i][j]) //except the bottom of the middle column.
                m_dirt[i][j] = new Dirt(i, j, this);
        }
    }
    int numBoulders = min(getLevel() / 2 + 2, 6);

    for (int i = 0; i < numBoulders; i++) {
        int boulderX = rand() % 60;
        int boulderY = (rand() % 36) + 20;
        while ((boulderX > MINESHAFT_LEFT_WALL_COL && boulderX < MINESHAFT_RIGHT_WALL_COL)) {
            boulderX = rand() % 60;
        }
        Boulder *boulder = new Boulder(boulderX, boulderY, this);
        deleteDirtAt(boulder->getX(), boulder->getY());
        m_objects.push_back(boulder);
    }

    return GWSTATUS_CONTINUE_GAME;
}

StudentWorld::~StudentWorld() { //erase all the dirt. et cetera.
    cleanUp();
}

void StudentWorld::cleanUp() { //erase all the dirt. Erase all the items. Erase the FrackMan
    while (!m_objects.empty()) {
        Actor *tmp = *(m_objects.begin());
        m_objects.erase(m_objects.begin());
        delete tmp;
    }
    for (int i = 0; i < VIEW_WIDTH; i++) {
        for (int j = 0; j < VIEW_HEIGHT; j++) {
            delete m_dirt[i][j];
            m_dirt[i][j] = nullptr;
        }
    }
    delete m_fm;
    m_fm = nullptr;
}

void StudentWorld::insertActor(Actor *toBeAdded) {
    if (toBeAdded)
        this->m_objects.push_back(toBeAdded);
}
int StudentWorld::move() {

    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    // decLives();
    m_fm->doSomething();
    for (std::list<Actor *>::iterator i = m_objects.begin(); i != m_objects.end(); ++i) {
        (*i)->doSomething();
    }
    //clearDead(); //this is in frackman::dosomething
    //int val;
    /*if (getLives()<=0)
        return GWSTATUS_PLAYER_DIED;
    else*/
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::clearDead() {
    //delete all the objects marked as dead.
    std::list<Actor *>::iterator i = m_objects.begin();
    while (i != m_objects.end()) {
        //Actor *tmp = *i;
        if ((*i)->toBeRemoved()) {
            Actor *tmp = *i;
            i = m_objects.erase(i);
            delete tmp;
        } else {
            ++i;
        }
    }
    if (!m_dirtDeleted) //allows for lazy evaluation, and the game to not be slow as molasses
        return; //very important to set this every time you call Actor::markRemoved()
    m_dirtDeleted = false;
    while (!dirtToBeDeleted.empty()) { //removed a nested for loop: winning.
        StudentWorld::IntPair toDelete = dirtToBeDeleted.top();
        dirtToBeDeleted.pop(); //everything inside of the stack is to be deleted
        delete m_dirt[toDelete.i][toDelete.j];
        m_dirt[toDelete.i][toDelete.j] = nullptr;
    }
}


void StudentWorld::deleteDirtAt(int x, int y) { //this can be made 4x faster
// by only erasing the dirt I'm walking into
    //erase all the dirt that the FrackMan is standing on
    //or,more generally, erase all the dirt in a 4x4 square
    // given by its bottom-right coordinate, x,y
    bool dirt_deleted = false;
    for (int i = x + 3; i >= x; i--) {
        for (int j = y + 3; j >= y; j--) {
            if (!m_dirt[i][j])
                continue;
            if (m_dirt[i][j]->toBeRemoved())
                continue;
            if (i >= 0 && j >= 0) {
                m_dirt[i][j]->markRemoved();
                IntPair toDelete(i, j);
                dirtToBeDeleted.push(toDelete);
                dirt_deleted = true;
            }
        }
    }
    m_dirtDeleted = true;
    if (dirt_deleted) {
        playSound(SOUND_DIG);
    }
}

std::string StudentWorld::setDisplayText(void) {
    return std::string("");
    //return std::string()
    //return __cxx11::basic_string<char, char_traits<_CharT>, allocator<_CharT>>();
}

int StudentWorld::getScore() {
    return m_score;
}

void StudentWorld::incScore(int amount) {
    m_score = m_score + amount;
}

void StudentWorld::resetScore() {
    m_score = 0;
}

int StudentWorld::getLevel() {
    return m_level;
}

void StudentWorld::incLevel() {
    ++m_level;
}

bool StudentWorld::dirtAt(int x, int y) {
    for (int i = x + 3; i >= x; i--) {
        for (int j = y + 3; j >= y; j--) {
            if (i < 0 && j < 0)
                continue;
            if (!m_dirt[i][j] || m_dirt[i][j]->toBeRemoved()) {
                continue;
            } else {
                return true;
            }

        }
    }
    return false;
}

bool StudentWorld::boulderAt(int x, int y) {
    for (std::list<Actor *>::iterator i = m_objects.begin(); i != m_objects.end(); ++i) {
        if ((*i)->obstructsProtesters(x, y)) {
            return true;
        }
    }
    /*
     * TODO: Check the Boulders/other objects. Write a function that determines if there's something that blocks the
     * way of the protester trying to go there.
     */
    return false;
}

//bool StudentWorld::dirtAt(int x, int y){
//  return m_dirt[x][y]!= nullptr;
//}