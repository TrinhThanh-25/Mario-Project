#ifndef ENEMYFACTORY_H
#define ENEMYFACTORY_H

#include "EnemyType.h"
#include "Enemy.h"

#include "BanzaiBill.h"
#include "BlueKoopaTroopa.h"
#include "BobOmb.h"
#include "BulletBill.h"
#include "BuzzyBeetle.h"
#include "FlyingGoomba.h"
#include "Goomba.h"
#include "GreenKoopaTroopa.h"
#include "JumpingPiranhaPlant.h"
#include "MontyMole.h"
#include "MummyBeetle.h"
#include "Muncher.h"
#include "PiranhaPlant.h"
#include "RedKoopaTroopa.h"
#include "Rex.h"
#include "Swooper.h"
#include "YellowKoopaTroopa.h"

enum class EnemyType {
    BANZAI_BILL,
    BLUE_KOOPA_TROOPA,
    BOB_OMB,
    BULLET_BILL,
    BUZZY_BEETLE,
    FLYING_GOOMBA,
    GOOMBA,
    GREEN_KOOPA_TROOPA,
    JUMPING_PIRANHA_PLANT,
    MONTY_MOLE,
    MUMMY_BEETLE,
    MUNCHER,
    PIRANHA_PLANT,
    RED_KOOPA_TROOPA,
    REX,
    SWOOPER,
    YELLOW_KOOPA_TROOPA
};

class EnemyFactory {
public:
    static Enemy* createEnemy(EnemyType type, Vector2 position, Direction direction);
};

#endif // ENEMYFACTORY_H
