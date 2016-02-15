#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

FrackMan::FrackMan(int imageID, int startX, int startY) : Actor(IID_PLAYER, FRACKMAN_START_X, FRACKMAN_START_Y, right,
                                                                1.0, 0) {
    setVisible(true);


}

OilBarrel::OilBarrel(int locX, int locY) : Discovery(IID_BARREL, locX, locY) {
    setVisible(false);

}

void OilBarrel::doSomething() {
    if (!this->isAlive())
        return;

}
