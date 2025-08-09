#ifndef GAMESTATEFACTORY_H
#define GAMESTATEFACTORY_H

#include "GameState/GameState.h"
#include "GameState/TitleScreenState.h"
#include "GameState/ChooseCharacterState.h"
#include "GameState/PlayingState.h"
#include "GameState/CountingPointState.h"
#include "GameState/FinishedState.h"
#include "GameState/GameOverState.h"
#include "GameState/GoNextMapState.h"
#include "GameState/IrisOutState.h"
#include "GameState/SettingState.h"
#include "GameState/TimeUpState.h"
#include "GameState/CustomMapState.h"
#include "GameState/TestMapState.h"
#include "GameState/ChooseCustomizedMap.h"
#include "Game/World.h"

class GameStateFactory {
    public:
        static GameState* createGameState(World* world, GameStateType type);
};

#endif