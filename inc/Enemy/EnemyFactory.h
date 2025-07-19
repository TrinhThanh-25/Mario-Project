#ifndef ENEMYFACTORY_H
#define ENEMYFACTORY_H

#include "Enemy/Enemy.h"
#include "Enemy/BanzaiBill.h"
#include "Enemy/BlueKoopaTroopa.h"
#include "Enemy/BobOmb.h"
#include "Enemy/BulletBill.h"
#include "Enemy/BuzzyBeetle.h"
#include "Enemy/FlyingGoomba.h"
#include "Enemy/Goomba.h"
#include "Enemy/GreenKoopaTroopa.h"
#include "Enemy/JumpingPiranhaPlant.h"
#include "Enemy/MontyMole.h"
#include "Enemy/MummyBeetle.h"
#include "Enemy/Muncher.h"
#include "Enemy/PiranhaPlant.h"
#include "Enemy/RedKoopaTroopa.h"
#include "Enemy/Rex.h"
#include "Enemy/Swooper.h"
#include "Enemy/YellowKoopaTroopa.h"

class EnemyFactory {
public:
    static Enemy* createEnemy(EnemyType type, Vector2 position, Direction direction);
};

#endif // ENEMYFACTORY_H
