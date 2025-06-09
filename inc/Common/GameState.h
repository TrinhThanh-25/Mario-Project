#ifndef GAMESTATE_H
#define GAMESTATE_H

enum GameState {
    TITLE_SCREEN,

    PLAYING,
    PAUSED,

    TIME_UP,
    GAME_OVER,

    COUNTING_POINT,
    IRIS_OUT,
    GO_NEXT_MAP,
    FINISHED
};

#endif