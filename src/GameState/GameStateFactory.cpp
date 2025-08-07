#include "GameState/GameStateFactory.h"
#include "GameState/GameStateType.h"

GameState* GameStateFactory::createGameState(World* world, GameStateType type) {
    switch (type) {
        case GameStateType::TITLE_SCREEN:
            return new TitleScreenState(world);
        case GameStateType::CHOOSE_CHARACTER:
            return new ChooseCharacterState(world);
        case GameStateType::PLAYING:
            return new PlayingState(world);
        case GameStateType::COUNTING_POINT:
            return new CountingPointState(world);
        case GameStateType::FINISHED:
            return new FinishedState(world);
        case GameStateType::GAME_OVER:
            return new GameOverState(world);
        case GameStateType::GO_NEXT_MAP:
            return new GoNextMapState(world);
        case GameStateType::IRIS_OUT:
            return new IrisOutState(world);
        case GameStateType::SETTING:
            return new SettingState(world);
        case GameStateType::TIME_UP:
            return new TimeUpState(world);
        case GameStateType::CUSTOM_MAP:
            return new CustomMapState(world);
        case GameStateType::TEST_MAP:
            return new TestMapState(world);
        case GameStateType::CHOOSE_CUSTOMIZED_MAP:
            return new ChooseCustomizedMapState(world);
        default:
            return nullptr;
    }
}