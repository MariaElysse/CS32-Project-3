#include "StudentWorld.h"
#include "Actor.h"
#include <time.h>

using namespace std;

GameWorld *createStudentWorld(string assetDir) {
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir), m_dirtDeleted(false), m_level(0),
                                                   m_sonarPresent(false), initialized(false) {
    srand(time(NULL));
    //add to vector m_objects
    //set up dirt
}

int StudentWorld::init() {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            m_distanceMaps[i][j] = 8192000; //has_dirt
        }
    }
    m_distanceMaps[60][60] = 0; //EXIT STAGE RIGHT, PROTESTER
    m_fm = new FrackMan(this);
    int numBoulders = min(getLevel() / 2 + 2, 6);
    m_barrelsRemaining = min(getLevel() / 2 + 2, 6);
    int numGold = max(5 - getLevel() / 2, 2);
    for (int i = 0; i < VIEW_WIDTH; i++) { //fill the entire world with dirt (me_irl)
        for (int j = 0; j < VIEW_HEIGHT; j++) {
            m_dirt[i][j] = nullptr;
            if ((j < (VIEW_HEIGHT - SPRITE_HEIGHT) &&
                 (i < MINESHAFT_LEFT_WALL_COL || i > MINESHAFT_RIGHT_WALL_COL))) { //except the column in the centre
                m_dirt[i][j] = new Dirt(i, j, this);
                m_distanceMaps[i][j] = 4096000; //no dirt, but unmapped.
            }
            if (j < MINESHAFT_BOTTOM_ROW && !m_dirt[i][j]) { //except the bottom of the middle column.
                m_dirt[i][j] = new Dirt(i, j, this);
            }
        }
    }
    addRandomDiscoveries<OilBarrel>(m_barrelsRemaining);
    addRandomDiscoveries<Boulder>(numBoulders);
    addRandomDiscoveries<GoldNugget>(numGold);
    Protester *prot = new Protester(IID_PROTESTER, 60, 60, this);
    Protester *hcprot = new HardcoreProtester(60,60, this);
    m_objects.push_back(prot);
    m_objects.push_back(hcprot);
    mapCurrentPaths(59, 60, 60, 60);
    initialized = true;
    return GWSTATUS_CONTINUE_GAME;
}

StudentWorld::~StudentWorld() { //erase all the dirt. et cetera.
    if (initialized)
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


    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    // decLives();
    if (m_barrelsRemaining == 0)
        return GWSTATUS_FINISHED_LEVEL;
    if (m_fm->getHealth() <= 0)
        return GWSTATUS_PLAYER_DIED;
    m_fm->doSomething();
    for (std::list<Actor *>::iterator i = m_objects.begin(); i != m_objects.end(); ++i) {
        (*i)->doSomething();
    }
    int goodie_randnum = (rand() % 100) + 1;
    if (((getLevel() * 25 + 300) + 1) % goodie_randnum == 0) {
        if ((rand() % 5) == 0 && !m_sonarPresent) {
            Sonar *sonar = new Sonar(this);
            m_objects.push_back(sonar);
            m_sonarPresent = true;
        } else {
            IntPair ip = getRandomMineshaftCoord();
            Water *water = new Water(ip.i, ip.j, this);//make water.
            m_objects.push_back(water);
        }
    }

    //setGameStatText("KT SCORE: " + std::to_string(this->getScore()));
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
                if (!play) {
                    m_distanceMaps[i][j] = 8192000;
                }
                if (dirtAt(i, j) && m_distanceMaps[i][j] == 8192000) {
                    m_distanceMaps[i][j] = minAround(x, y) + 1;
                }
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
    //return false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (m_dirt[x + i][y + j] != nullptr /*&& !m_dirt[x+i][y+j]->toBeRemoved()*/) {
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
    m_oil++;
    m_barrelsRemaining--;
    increaseScore(BARREL_VALUE);
}

void StudentWorld::gotWater() {
    increaseScore(100);
    m_fm->addWater();
};


StudentWorld::IntPair StudentWorld::getRandomActorLocation() {
    int boulderX = rand() % 60;
    int boulderY = (rand() % 36) + 20;
    while ((boulderX + SPRITE_WIDTH > MINESHAFT_LEFT_WALL_COL && boulderX - SPRITE_WIDTH < MINESHAFT_RIGHT_WALL_COL)) {
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

void StudentWorld::gotGold() {
    m_fm->addGold();
    increaseScore(GOLD_VALUE);
}

template<class T>
void StudentWorld::addRandomDiscoveries(int num) {
    for (int i = 0; i < num; i++) {
        IntPair discLoc = this->getRandomActorLocation();
        T *discovery = new T(discLoc.i, discLoc.j, this);
        m_objects.push_back(discovery);
        //oilBarrel->setVisible(true);
    }
}

GraphObject::Direction StudentWorld::frackManDirection() {
    return m_fm->getDirection();
}

void StudentWorld::damageFrackMan(int damage) {
    m_fm->hurt(damage);
}

bool StudentWorld::lineOfSightWithFrackMan(Person *person) { //todo: fix to include indirect line of sight p.42 Footnotes
    int x = max(m_fm->getX(), person->getX());
    int y = max(m_fm->getY(), person->getY());
    int otherx = min(m_fm->getX(), person->getX());
    int othery = min(m_fm->getY(), person->getY());
    if (abs(person->getY() - m_fm->getY()) <= 4) {
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < x; i++) {
                if (!obstructionAt(otherx + i, othery + j) && !dirtAt(otherx + i, othery + j)) {
                    return true;
                }
            }
        }
    }
    if (abs(person->getX() - m_fm->getX()) <= 4) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < y; j++) {
                if (!obstructionAt(otherx + i, othery + j) && !dirtAt(otherx + i, othery + j)) {
                    return true;
                }
            }
        }
    }
    return false;
}

GraphObject::Direction StudentWorld::directionToFrackMan(Person *person) {
    if (person->getX() == m_fm->getX() && person->getY() == m_fm->getY())
        return person->getDirection();
    if (abs(person->getX() - m_fm->getX()) <= 4.0) {
        if (person->getY() >= m_fm->getY())
            return GraphObject::down;
        else
            return GraphObject::up;
    } else if (abs(person->getY() - m_fm->getY()) <= 4.0) {
        if (person->getX() >= m_fm->getX())
            return GraphObject::left;
        else
            return GraphObject::right;
    } else {
        return m_fm->getDirection(); //as good a default direction as any, besides,
        // you're never supposed to call this function when they're not in the same row or column
    }
}

void StudentWorld::damageOneActorAt(int x, int y, Squirt *squirt) {
    for (Actor *obj : m_objects) {
        if (abs(obj->getX() - x) <= 3.0 && abs(obj->getY() - y <= 3.0)) { //todo:euclidian distance
            obj->hurt(SQUIRT_DAMAGE);
            if (dynamic_cast<Protester *>(obj) && !dynamic_cast<Squirt *> (obj)) {
                squirt->markRemoved();
                return;
            }
        }
    }
}

void StudentWorld::damageAllActorsAt(int x, int y) {
    for (Actor *obj: m_objects) {
        if ((std::abs(x - obj->getX()) <= 3.0) && (std::abs(y - obj->getY()) <= 3.0) &&
            obj->getHealth() != 0) { //todo: same as above
            obj->hurt(obj->getHealth());
        }
        if ((abs(x - m_fm->getX()) <= 3.0) && (abs(y - m_fm->getY()) <= 3.0)) {
            m_fm->hurt(m_fm->getHealth());
        }
    }
    this->mapCurrentPaths(59, 60, 60, 60);
}

GraphObject::Direction StudentWorld::leaveOilField(int &x, int &y) {
    GraphObject::Direction dir = minAroundDirection(x, y);
    m_fm->validMovement(x, y, dir);
    return dir;
}

GraphObject::Direction StudentWorld::minAroundDirection(int x, int y) {
    int min = 9000001;
    GraphObject::Direction mindir = GraphObject::none;
    int tempx = x;
    int tempy = y;
    if (m_fm->validMovement(tempx, tempy, GraphObject::down)) { //get the smallest distance stored in the thing

        if (m_distanceMaps[tempx][tempy] < min) {
            if (tempx == x && tempy == y);
            else {
                min = m_distanceMaps[tempx][tempy];
                mindir = GraphObject::down;
            }
        }
        tempx = x;
        tempy = y;
    }
    if (m_fm->validMovement(tempx, tempy, GraphObject::up)) {
        if (m_distanceMaps[tempx][tempy] < min) {
            if (!(tempx == x && tempy == y)) {
                min = m_distanceMaps[tempx][tempy];
                mindir = GraphObject::up;
            }
            tempx = x;
            tempy = y;

        }
    }
    if (m_fm->validMovement(tempx, tempy, GraphObject::left)) {
        if (m_distanceMaps[tempx][tempy] < min) {
            if (tempx == x && tempy == y);
            else {
                min = m_distanceMaps[tempx][tempy];
                mindir = GraphObject::left;
            }
        }
        tempx = x;
        tempy = y;
    }
    if (m_fm->validMovement(tempx, tempy, GraphObject::right)) {
        if (m_distanceMaps[tempx][tempy] < min) {
            if (tempx == x && tempy == y);
            else {
                min = m_distanceMaps[tempx][tempy];
                mindir = GraphObject::right;
            }
        }
        tempx = x;
        tempy = y;

    }

    return mindir;

}

int StudentWorld::minAround(int x, int y) {
    int tmpx = x;
    int tmpy = y;
    if (minAroundDirection(x, y) != GraphObject::none)
        m_fm->validMovement(x, y, minAroundDirection(x, y));
    return m_distanceMaps[tmpx][tmpy];
}


void StudentWorld::mapCurrentPaths(int x, int y, int fromX, int fromY) {
    //if (x==60 && y==60) {
    //    m_distanceMaps[60][60] = 0;
    //    return;

    if (x < 0 || y < 0 || x > 62 || y > 62) {
        return;
    }
    //}
    if (m_distanceMaps[x][y] < 4096000)
        return;

    if (x == 60 && y == 60) {
        m_distanceMaps[x][y] = 0;
        return;
    }
    m_distanceMaps[x][y] = m_distanceMaps[fromX][fromY] + 1;
    mapCurrentPaths(x - 1, y, x, y);
    mapCurrentPaths(x + 1, y, x, y);
    mapCurrentPaths(x, y - 1, x, y);
    mapCurrentPaths(x, y + 1, x, y);
}

StudentWorld::IntPair StudentWorld::getRandomMineshaftCoord() {
    IntPair ip = IntPair(rand() % 64, rand() % 60);
    while (dirtAt(ip.i, ip.j)) {
        ip = IntPair(rand() % 64, rand() % 60); //todo: make this not shitty
    }
    return ip;
}
