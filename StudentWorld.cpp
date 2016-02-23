#include "StudentWorld.h"
#include "Actor.h"


using namespace std;

GameWorld *createStudentWorld(string assetDir) {
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir), m_dirtDeleted(false), m_level(0),
                                                   m_sonarPresent(false) {
    //add to vector m_objects
    //set up dirt
}

int StudentWorld::init() {
    delete m_fm;
    m_fm = new FrackMan(this);
    int numBoulders = min(getLevel() / 2 + 2, 6);
    m_barrelsRemaining = min(getLevel() / 2 + 2, 6);
    for (int i = 0; i < VIEW_WIDTH; i++) { //fill the entire world with dirt (me_irl)
        for (int j = 0; j < VIEW_HEIGHT; j++) {
            if ((j < (VIEW_HEIGHT - SPRITE_HEIGHT) &&
                 (i < MINESHAFT_LEFT_WALL_COL || i > MINESHAFT_RIGHT_WALL_COL))) //except the column in the centre
                m_dirt[i][j] = new Dirt(i, j, this);
            else
                m_dirt[i][j] = nullptr;
            if (j < MINESHAFT_BOTTOM_ROW && !m_dirt[i][j]) //except the bottom of the middle column.
                m_dirt[i][j] = new Dirt(i, j, this);
        }
    }
    for (int i = 0; i < numBoulders; i++) {
        IntPair boulderLoc = this->getRandomActorLocation();
        Boulder *boulder = new Boulder(boulderLoc.i, boulderLoc.j, this);
        m_objects.push_back(boulder);
    }

    for (int i = 0; i < m_barrelsRemaining; i++) {
        IntPair oilLoc = this->getRandomActorLocation();
        OilBarrel *oilBarrel = new OilBarrel(oilLoc.i, oilLoc.j, this);
        m_objects.push_back(oilBarrel);
        //oilBarrel->setVisible(true);
    }
    return GWSTATUS_CONTINUE_GAME;
}

StudentWorld::~StudentWorld() { //erase all the dirt. et cetera.
    cleanUp();
}

void StudentWorld::cleanUp() { //erase all the dirt. Erase all the items. Erase the FrackMan
    delete m_fm;
    m_fm = nullptr;
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
    clearDead();
}

void StudentWorld::insertActor(Actor *toBeAdded) {
    if (toBeAdded)
        this->m_objects.push_back(toBeAdded);
}
int StudentWorld::move() {
    if (m_barrelsRemaining == 0)
        return GWSTATUS_FINISHED_LEVEL;
    if (m_fm->getHealth() <= 0)
        return GWSTATUS_PLAYER_DIED;

    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    // decLives();
    m_fm->doSomething();
    for (std::list<Actor *>::iterator i = m_objects.begin(); i != m_objects.end(); ++i) {
        (*i)->doSomething();
    }
    int goodie_probability = rand() % 100;
    if ((goodie_probability) % (min(90, getLevel() * 10 + 30)) == 0) {
        if ((rand() % 5) == 0 && !m_sonarPresent) {
            Sonar *sonar = new Sonar(this);
            m_objects.push_back(sonar);
            m_sonarPresent = true;
        } else { ;//make water.
        }
    }
    setGameStatText("KT SCORE: " + std::to_string(this->getScore()));
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


void StudentWorld::deleteDirtAt(int x, int y, bool play) { //this can be made 4x faster
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
    if (dirt_deleted && play) {
        playSound(SOUND_DIG);
    }
}

std::string StudentWorld::setDisplayText(void) {
    return std::string("");
    //return std::string()
    //return __cxx11::basic_string<char, char_traits<_CharT>, allocator<_CharT>>();
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

bool StudentWorld::obstructionAt(int x, int y) {
    for (std::list<Actor *>::iterator i = m_objects.begin(); i != m_objects.end(); ++i) {
        if ((*i)->obstructsProtesters(x, y)) {
            return true;
        }
    }

    return false;
}

//bool StudentWorld::dirtAt(int x, int y){
//  return m_dirt[x][y]!= nullptr;
//}
float StudentWorld::frackManCornerRadius(Actor *actor) {
    return sqrtf(sqr(fabsf(actor->getX() - m_fm->getX())) + sqr(fabsf(actor->getY() - m_fm->getY())));
}

void StudentWorld::gotOil() {
    playSound(SOUND_GOT_GOODIE);
    m_oil++;
    m_barrelsRemaining--;
    increaseScore(BARREL_VALUE);
}


StudentWorld::IntPair StudentWorld::getRandomActorLocation() {
    int boulderX = rand() % 60;
    int boulderY = (rand() % 36) + 20;
    while ((boulderX > MINESHAFT_LEFT_WALL_COL + SPRITE_WIDTH && boulderX < MINESHAFT_RIGHT_WALL_COL + SPRITE_WIDTH)) {
        boulderX = rand() % 60;
    }
    return IntPair(boulderX, boulderY);
}

int StudentWorld::amtOfOil() {
    return m_oil;
}


void StudentWorld::sonarDespawned(bool byFrackMan) {
    if (byFrackMan) {
        increaseScore(SONAR_VALUE);
        m_fm->addSonar();
    }
    m_sonarPresent = false;
}

void StudentWorld::sonarPulse(int x, int y) {
    for (std::list<Actor *>::iterator i = m_objects.begin(); i != m_objects.end(); ++i) {
        if (frackManCornerRadius(*i) - 12 <= 0.5) {
            (*i)->setVisible(true);
        }
    }
}
