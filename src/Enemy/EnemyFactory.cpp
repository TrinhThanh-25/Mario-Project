#include "Enemy/EnemyFactory.h"

Enemy* EnemyFactory::createEnemy(EnemyType type, Vector2 position, Direction direction) {
    Enemy* enemy = nullptr;
    Vector2 size = {32, 32};       // Default size
    Vector2 velocity = {0, 0};     // Default velocity
    Color color = WHITE;           // Default color

    switch (type) {
        case EnemyType::BANZAI_BILL:
            size = {64, 64};
            velocity = {direction == Direction::LEFT ? -4.0f : 4.0f, 0};
            enemy = new BanzaiBill(position, size, velocity, color);
            break;

        case EnemyType::BLUE_KOOPA_TROOPA:
            velocity = {direction == Direction::LEFT ? -1.0f : 1.0f, 0};
            enemy = new BlueKoopaTroopa(position, {32, 54}, velocity, color);
            break;

        case EnemyType::BOB_OMB:
            velocity = {direction == Direction::LEFT ? -0.9f : 0.9f, 0};
            enemy = new BobOmb(position, size, velocity, color);
            break;

        case EnemyType::BULLET_BILL:
            size = {64, 32};
            velocity = {direction == Direction::LEFT ? -5.0f : 5.0f, 0};
            enemy = new BulletBill(position, size, velocity, color);
            break;

        case EnemyType::BUZZY_BEETLE:
            velocity = {direction == Direction::LEFT ? -1.1f : 1.1f, 0};
            enemy = new BuzzyBeetle(position, size, velocity, color);
            break;

        case EnemyType::FLYING_GOOMBA:
            velocity = {direction == Direction::LEFT ? -0.8f : 0.8f, -1.8f};
            enemy = new FlyingGoomba(position, size, velocity, color);
            break;

        case EnemyType::GOOMBA:
            velocity = {direction == Direction::LEFT ? -1.2f : 1.2f, 0};
            enemy = new Goomba(position, size, velocity, color);
            break;

        case EnemyType::GREEN_KOOPA_TROOPA:
            velocity = {direction == Direction::LEFT ? -1.0f : 1.0f, 0};
            enemy = new GreenKoopaTroopa(position, size, velocity, color);
            break;

        case EnemyType::JUMPING_PIRANHA_PLANT:
            size = {32, 48};
            velocity = {0, -2.5f};
            enemy = new JumpingPiranhaPlant(position, size, velocity, color);
            break;

        case EnemyType::MONTY_MOLE:
            velocity = {direction == Direction::LEFT ? -1.5f : 1.5f, 0};
            enemy = new MontyMole(position, size, velocity, color);
            break;

        case EnemyType::MUMMY_BEETLE:
            velocity = {direction == Direction::LEFT ? -1.2f : 1.2f, 0};
            enemy = new MummyBeetle(position, size, velocity, color);
            break;

        case EnemyType::MUNCHER:
            // Muncher đứng yên và không cần velocity
            enemy = new Muncher(position, size, color);
            break;

        case EnemyType::PIRANHA_PLANT:
            size = {32, 48};
            velocity = {0, -1.2f};
            enemy = new PiranhaPlant(position, size, velocity, color);
            break;

        case EnemyType::RED_KOOPA_TROOPA:
            velocity = {direction == Direction::LEFT ? -0.9f : 0.9f, 0};
            enemy = new RedKoopaTroopa(position, size, velocity, color);
            break;

        case EnemyType::REX:
            velocity = {direction == Direction::LEFT ? -1.8f : 1.8f, 0};
            enemy = new Rex(position, size, velocity, color);
            break;

        case EnemyType::SWOOPER:
            velocity = {0, 1.5f};
            enemy = new Swooper(position, size, velocity, color);
            break;

        case EnemyType::YELLOW_KOOPA_TROOPA:
            velocity = {direction == Direction::LEFT ? -1.1f : 1.1f, 0};
            enemy = new YellowKoopaTroopa(position, size, velocity, color);
            break;
    }

    return enemy;
}
