#include "Enemy/EnemyFactory.h"

Enemy* EnemyFactory::createEnemy(EnemyType type, Vector2 position, Direction direction) {
    Enemy* enemy = nullptr;
    Vector2 size = {32, 32};       // Default size
    Vector2 velocity = {0, 0};     // Default velocity
    Color color = WHITE;           // Default color

    switch (type) {
        case EnemyType::BANZAI_BILL:
            size = {128, 128};
            velocity = {direction == Direction::LEFT ? -200.0f : 200.0f, 0};
            enemy = new BanzaiBill(position, size, velocity, color);
            break;

        case EnemyType::BLUE_KOOPA_TROOPA:
<<<<<<< HEAD
            velocity = {direction == Direction::LEFT ? -1.0f : 1.0f, 0};
            enemy = new BlueKoopaTroopa(position, size, velocity, color);
=======
            size = {32, 54};
            velocity = {direction == Direction::LEFT ? -100.0f : 100.0f, 0};
            enemy = new BlueKoopaTroopa(position, {32, 54}, velocity, color);
>>>>>>> 61c96372c37f6dcbc7eddb26abd27043492e4338
            break;

        case EnemyType::BOB_OMB:
            size = {24, 30};
            velocity = {direction == Direction::LEFT ? -100.0f : 100.0f, 0};
            enemy = new BobOmb(position, size, velocity, color);
            break;

        case EnemyType::BULLET_BILL:
            size = {32, 28};
            velocity = {direction == Direction::LEFT ? -200.0f : 200.0f, 0};
            enemy = new BulletBill(position, size, velocity, color);
            break;

        case EnemyType::BUZZY_BEETLE: 
            velocity = {direction == Direction::LEFT ? -80.0f : 80.0f, 0};
            enemy = new BuzzyBeetle(position, size, velocity, color);
            break;

        case EnemyType::FLYING_GOOMBA:
            size = {66, 50};
            velocity = {direction == Direction::LEFT ? -100.0f : 100.0f, -1.8f};
            enemy = new FlyingGoomba(position, size, velocity, color);
            break;

        case EnemyType::GOOMBA:
            velocity = {direction == Direction::LEFT ? -100.0f : 100.0f, 0};
            enemy = new Goomba(position, size, velocity, color);
            break;

        case EnemyType::GREEN_KOOPA_TROOPA:
            size = {32, 54};
            velocity = {direction == Direction::LEFT ? -100.0f : 100.0f, 0};
            enemy = new GreenKoopaTroopa(position, size, velocity, color);
            break;

        case EnemyType::JUMPING_PIRANHA_PLANT:
            size = {32, 42};
            velocity = {0, 0};  // con này chạy thử xem
            enemy = new JumpingPiranhaPlant(position, size, velocity, color);
            break;

        case EnemyType::MONTY_MOLE:
            velocity = {direction == Direction::LEFT ? -200.0f : 200.0f, 0};
            enemy = new MontyMole(position, size, velocity, color);
            break;

        case EnemyType::MUMMY_BEETLE:
            velocity = {direction == Direction::LEFT ? -80.0f : 80.0f, 0};
            enemy = new MummyBeetle(position, size, velocity, color);
            break;

        case EnemyType::MUNCHER:
            // Muncher đứng yên và không cần velocity
            size = {32, 30};
            enemy = new Muncher(position, size, color);
            break;

        case EnemyType::PIRANHA_PLANT:
            size = {32, 66};  // chạy để xem
            velocity = {0.0f, 0.0f};
            enemy = new PiranhaPlant(position, size, velocity, color);
            break;

        case EnemyType::RED_KOOPA_TROOPA:
            size = {32, 54};
            velocity = {direction == Direction::LEFT ? -100.0f : 100.0f, 0};
            enemy = new RedKoopaTroopa(position, size, velocity, color);
            break;

        case EnemyType::REX:
            size = {40, 64};
            velocity = {direction == Direction::LEFT ? -100.0f : 100.0f, 0};
            enemy = new Rex(position, size, velocity, color);
            break;

        case EnemyType::SWOOPER:  // để chạy thử
            size = {32, 34};
            velocity = {0.0f, 0.0f};
            enemy = new Swooper(position, size, velocity, color);
            break;

        case EnemyType::YELLOW_KOOPA_TROOPA:
            size = {32, 54};
            velocity = {direction == Direction::LEFT ? -100.0f : 100.0f, 0};
            enemy = new YellowKoopaTroopa(position, size, velocity, color);
            break;
    }

    return enemy;
}
