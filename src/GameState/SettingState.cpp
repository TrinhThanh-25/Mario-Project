#include "GameState/SettingState.h"
#include "GameState/PlayingState.h"
#include "GameState/TitleScreenState.h"
#include "GameState/CountingPointState.h"
#include "GameState/GoNextMapState.h"
#include "GameState/IrisOutState.h"
#include "GameState/TimeUpState.h"
#include "Common/ResourceManager.h"
#include "Common/AudioManager.h"
#include "Common/GamepadManager.h"
#include "SaveGame.h"
#include "GameState/GameStateFactory.h"

Rectangle SettingState::getRestoreDefaultButtonRect() const {
    return Rectangle{(float)GetScreenWidth() - 220, (float)GetScreenHeight() - 80, 200, 50};
}

void SettingState::drawRestoreDefaultButton() {
    Rectangle buttonRect = getRestoreDefaultButtonRect();
    Vector2 mousePos = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mousePos, buttonRect);
    
    Color buttonColor, textColor;
    if (isHovering) {
        buttonColor = Fade(LIGHTGRAY, 0.8f);
        textColor = BLACK;
    } else {
        buttonColor = Fade(DARKGRAY, 0.6f);
        textColor = WHITE;
    }
    
    float roundness = 0.3f;
    int segments = 0;
    DrawRectangleRounded(buttonRect, roundness, segments, buttonColor);
    
    const char* displayText = "Restore to Default";
    int fontSize = 18;
    int textWidth = MeasureText(displayText, fontSize);
    std::unordered_map<std::string, Font> font = ResourceManager::getFont();
    DrawTextEx(font["DejavuSans"], displayText, 
        {(float)(buttonRect.x + buttonRect.width/2 - textWidth/2), (float)(buttonRect.y + buttonRect.height/2 - fontSize/2)}, 
        (float)fontSize, 0, textColor);
}

void SettingState::updateRestoreDefaultButton() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        Rectangle buttonRect = getRestoreDefaultButtonRect();
        if (CheckCollisionPointRec(mousePos, buttonRect)) {
            isDefaultNotified = true;
        }
    }
}

SettingState::SettingState(World* world)
    : GameState(world, GameStateType::SETTING),
    resumeButton({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 50, 300, 50}, "Resume", 36),
    restartButton({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 110, 300, 50}, "Restart", 36),
    returnButton({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 170, 300, 50}, "Return", 36),
    exitButton({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 230, 300, 50}, "Exit", 36),
    musicVolumeSlider("MUSIC",{(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 - 50}, 300, 0.0f, 1.0f, AudioManager::getMusicVolume(), 36),
    sfxVolumeSlider("SFX",{(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 10}, 300, 0.0f, 1.0f, AudioManager::getSfxVolume(), 36),
    backgroundPositionx(0.0f), speed(40.0f),
    keyManager(world->getKeyManager()), gamepadManager(world->getGamepadManager()){
        camera.offset = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
        camera.target = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
        updateConflictStatus();
}

SettingState::~SettingState() {
    
}

void SettingState::update() {
    if(isDefaultNotified) {
        updateConfirmDefaultSetting();
        return;
    }
    if(isConflictNotified) {
        updateConflictNotification();
        return;
    }
    // Only update settings UI if no notification is shown
    float availableHeight = GetScreenHeight() - 100;
    float contentHeight = (stateBeforeSetting == GameStateType::TITLE_SCREEN) ? 2500.0f : 2650.0f;  // Updated for new gamepad layout
    float maxHeight = contentHeight;
    float wheelMove = GetMouseWheelMove();
    if(wheelMove != 0) {
        camera.target.y -= wheelMove * 80.0f;
        if(camera.target.y < 450.0f) {
            camera.target.y = 450.0f; 
        } else if(camera.target.y > maxHeight) {
            camera.target.y = maxHeight; 
        }
    }
    updateVolumeAndButtonSetting();
    updateKeyControlSetting();
    updateGamepadControlSetting();
    updateRestoreDefaultButton();
}

void SettingState::draw() {
    DrawTextureRec(ResourceManager::getTexture()["SettingBackground"], Rectangle{(float)backgroundPositionx, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, Vector2{0, 0}, WHITE);
    BeginScissorMode(0, 50, GetScreenWidth(), GetScreenHeight() - 150);
    BeginMode2D(camera);
    drawVolumeAndButtonSetting();
    drawKeyControlSetting();
    drawGamepadControlSetting();
    EndMode2D();
    EndScissorMode();
    drawRestoreDefaultButton();
    if(isDefaultNotified) {
        drawConfirmDefaultSetting();
    } else if(isConflictNotified) {
        drawConflictNotification();
    }
}

void SettingState::setStateBeforeSetting(GameStateType stateBeforeSetting) {
    this->stateBeforeSetting = stateBeforeSetting;
    if (stateBeforeSetting == GameStateType::TITLE_SCREEN) {
        returnButton.setPosition({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 50});
        exitButton.setPosition({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 110});
        world->setGamePlay(GamePlay::PLAYDEVELOPEDMAP);
    }
    else {
        resumeButton.setPosition({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 50});
        restartButton.setPosition({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 110});
        returnButton.setPosition({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 170});
        exitButton.setPosition({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 230});
    }
}

json SettingState::saveToJson() const {
    json j;
    GameState* tempState = GameStateFactory::createGameState(world, stateBeforeSetting);
    if (tempState) {
        j = tempState->saveToJson();
        delete tempState;
        tempState = nullptr;
    }
    return j;
}

void SettingState::updateVolumeAndButtonSetting() {
    bool mouseInClipping = isMouseInClippingArea();
    if (mouseInClipping) {
        musicVolumeSlider.update(&camera);
        sfxVolumeSlider.update(&camera);
    }
    AudioManager::setMusicVolume(musicVolumeSlider.getValue());
    AudioManager::setSfxVolume(sfxVolumeSlider.getValue());
    
    if(backgroundPositionx > 400) {
        speed *=(-1.0f);
    } else if(backgroundPositionx < 0) {
        speed *=(-1.0f);
    }
    float deltaTime = GetFrameTime();
    backgroundPositionx += speed * deltaTime;
    if(stateBeforeSetting == GameStateType::TITLE_SCREEN) {
        if (mouseInClipping) {
            returnButton.update(&camera);
            exitButton.update(&camera);
        }
    } else {
        if (mouseInClipping) {
            resumeButton.update(&camera);
            restartButton.update(&camera);
            returnButton.update(&camera);
            exitButton.update(&camera);
        }
    }
    if (mouseInClipping) {
    if(resumeButton.isPressed(&camera) && stateBeforeSetting != GameStateType::TITLE_SCREEN) {
            if(world->getGamePlay() == GamePlay::PLAYDEVELOPEDMAP) {
                SaveGame::loadGame(*world);
            }
            else {
                SaveGame::loadGame(*world, "../resources/SaveGame/" + world->getMap()->getMapFileName() + ".json");
            }
        }
    else if(restartButton.isPressed(&camera) && stateBeforeSetting != GameStateType::TITLE_SCREEN) {
            world->resetMap();
        }
    else if(returnButton.isPressed(&camera)) {
            if (isConflict) {
                isConflictNotified = true;
            } else {
                world->resetGame();
            }
        }
    else if(exitButton.isPressed(&camera)) {
            world->setIsClosed(true);
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (isConflict) {
            isConflictNotified = true;
        } else {
            world->resetGame();
        }
    }
}

void SettingState::drawVolumeAndButtonSetting() {
    if(stateBeforeSetting == GameStateType::TITLE_SCREEN) {
        returnButton.draw();
        exitButton.draw();
    }
    else {
        resumeButton.draw();
        restartButton.draw();
        returnButton.draw();
        exitButton.draw();
    }
    musicVolumeSlider.draw();
    sfxVolumeSlider.draw();
}

void SettingState::updateKeyControlSetting() {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldMousePos = GetScreenToWorld2D(mousePos, camera);
    bool mouseInClipping = isMouseInClippingArea();
    
    if(isEditing && !isEditingGamepad) {
        curKeyValue = KeyboardKey::KEY_NULL;
        for (int keyValue = 0; keyValue < 512; keyValue++) {
            if (IsKeyPressed(keyValue)) {
                curKeyValue = static_cast<KeyboardKey>(keyValue);
                break;
            }
        }
        if (curKeyValue != KeyboardKey::KEY_NULL) {
            keyManager->setKey(curModePlayer, curInputName, static_cast<int>(curKeyValue));
            keyManager->saveCurrentKeyManager();
            
            updateConflictStatus();
            
            isEditing = false;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseInClipping && !CheckCollisionPointRec(worldMousePos, curRect)) {
            isEditing = false;
        }
    }
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseInClipping) {

        float startY;
        if(stateBeforeSetting == GameStateType::TITLE_SCREEN) {
            startY = 650.0f;
        } else {
            startY = 800.0f;
        }
        float currentY = startY + 50.0f;
        float fadeX = 100.0f;
        float fadeWidth = 1400.0f;
        currentY += 50.0f;
        float columnSpacing = 50.0f;
        float columnWidth = 400.0f;
        float startColumn1X = fadeX + columnSpacing;
        float startColumn2X = startColumn1X + columnWidth + columnSpacing;
        float startColumn3X = startColumn2X + columnWidth + columnSpacing;
        float rowHeight = 50.0f;
        float buttonHeight = 50.0f;
        std::vector<std::string> keyNames = {"LEFT", "RIGHT", "UP", "DOWN", "CONTROL", "SHIFT"};
        
        for (int i = 0; i < 6; i++) {
            float rowY = currentY + (i * rowHeight);
            checkInputButtonClick(worldMousePos, Rectangle{startColumn2X, rowY, 2*columnWidth + columnSpacing, buttonHeight}, ModePlayer::ONEPLAYER, keyNames[i], false);
        }
        
        currentY += (6 * rowHeight) + 50.0f;
        currentY += 50.0f;
        currentY += 50.0f;
        for (int i = 0; i < 6; i++) {
            float rowY = currentY + (i * rowHeight);
            checkInputButtonClick(worldMousePos, Rectangle{startColumn2X, rowY, columnWidth, buttonHeight}, ModePlayer::FIRSTPLAYER, keyNames[i], false);
            checkInputButtonClick(worldMousePos, Rectangle{startColumn3X, rowY, columnWidth, buttonHeight}, ModePlayer::SECONDPLAYER, keyNames[i], false);
        }
    }
}

void SettingState::drawKeyControlSetting() {
    float startPositionY;
    if(stateBeforeSetting == GameStateType::TITLE_SCREEN) {
        DrawRectangle (100, 650, 1400, 900, Fade(BLACK, 0.7f));
        startPositionY = 650.0f;
    } else {
        DrawRectangle (100, 800, 1400, 900, Fade(BLACK, 0.7f));
        startPositionY = 800.0f;
    }
    std::unordered_map<std::string, Font> font = ResourceManager::getFont();
    DrawTextEx(font["DejavuSans"], "KEY CONTROLS", {(float)(GetScreenWidth()/2.0f - MeasureText("KEY CONTROLS", 40)/2), (float)(startPositionY + 20)}, 40.0f, 0, WHITE);

    float currentY = startPositionY + 50.0f;
    std::vector<std::string> keyNames = {"LEFT", "RIGHT", "UP", "DOWN", "CONTROL", "SHIFT"};
    float fadeX = 100.0f;
    float fadeWidth = 1400.0f;
    
    DrawTextEx(font["DejavuSans"], "SINGLE PLAYER", {(float)(GetScreenWidth()/2.0f - MeasureText("SINGLE PLAYER", 30)/2), (float)currentY}, 30.0f, 0, YELLOW);
    currentY += 50.0f;
    
    auto& onePlayerKeys = keyManager->getKeys(ModePlayer::ONEPLAYER);
    
    float columnSpacing = 50.0f;
    float columnWidth = 400.0f;
    float startColumn1X = fadeX + columnSpacing;
    float startColumn2X = startColumn1X + columnWidth + columnSpacing;
    float startColumn3X = startColumn2X + columnWidth + columnSpacing;
    float rowHeight = 50.0f;
    float buttonHeight = 50.0f;
    
    for (int i = 0; i < 6; i++) {
        std::string keyName = keyNames[i];
        float rowY = currentY + (i * rowHeight);
        
        std::string functionName = getFunctionName(keyName);
        DrawTextEx(font["DejavuSans"], functionName.c_str(), {(float)startColumn1X, (float)rowY}, 40.0f, 0, WHITE);
        drawInputButton(ModePlayer::ONEPLAYER, keyName, onePlayerKeys[keyName], startColumn2X, rowY, 2*columnWidth + columnSpacing, buttonHeight, false);
    }
    currentY += (6 * rowHeight) + 50.0f;
    
    DrawTextEx(font["DejavuSans"], "MULTIPLAYER", {(float)(GetScreenWidth()/2.0f - MeasureText("MULTIPLAYER", 30)/2), (float)currentY}, 30.0f, 0, YELLOW);
    currentY += 50.0f;
    
    DrawTextEx(font["DejavuSans"], "FIRST PLAYER", {(float)(startColumn2X + columnWidth/2.0f - MeasureText("FIRST PLAYER", 20)/2), (float)currentY}, 20.0f, 0, WHITE);
    DrawTextEx(font["DejavuSans"], "SECOND PLAYER", {(float)(startColumn3X + columnWidth/2.0f - MeasureText("SECOND PLAYER", 20)/2), (float)currentY}, 20.0f, 0, WHITE);
    currentY += 50.0f;
    
    auto& firstPlayerKeys = keyManager->getKeys(ModePlayer::FIRSTPLAYER);
    auto& secondPlayerKeys = keyManager->getKeys(ModePlayer::SECONDPLAYER);
    
    for (int i = 0; i < 6; i++) {
        std::string keyName = keyNames[i];
        float rowY = currentY + (i * rowHeight);
        
        std::string functionName = getFunctionName(keyName);
        DrawTextEx(font["DejavuSans"], functionName.c_str(), {(float)startColumn1X, (float)(rowY + 10)}, 40.0f, 0, WHITE);
        drawInputButton(ModePlayer::FIRSTPLAYER, keyName, firstPlayerKeys[keyName], startColumn2X, rowY, columnWidth, buttonHeight, false);
        drawInputButton(ModePlayer::SECONDPLAYER, keyName, secondPlayerKeys[keyName], startColumn3X, rowY, columnWidth, buttonHeight, false);
    }
}

void SettingState::drawInputButton(ModePlayer modePlayer, const std::string& inputName, int inputValue, float x, float y, float width, float height, bool isGamepad) {
    Vector2 mousePos = GetMousePosition();
    mousePos.x += camera.target.x - camera.offset.x;
    mousePos.y += camera.target.y - camera.offset.y;
    
    bool mouseInClipping = isMouseInClippingArea();
    bool isHovering = mouseInClipping && (mousePos.x >= x && mousePos.x <= x + width &&mousePos.y >= y && mousePos.y <= y + height);
    //
    bool isEditingThis, hasConflict;
    if (isGamepad) {
        isEditingThis = (isEditing && isEditingGamepad && curInputName == inputName && curModePlayer == modePlayer);
        hasConflict = gamepadManager->isButtonConflicted(modePlayer, inputName, inputValue);
    } else {
        isEditingThis = (isEditing && !isEditingGamepad && curInputName == inputName && curModePlayer == modePlayer);
        hasConflict = keyManager->isKeyConflicted(modePlayer, inputName, inputValue);
    }
    
    Color buttonColor, borderColor, textColor;
    if (isEditingThis) {
        buttonColor = Fade(RED, 0.7f);
        textColor = WHITE;
    } else if (hasConflict) {
        buttonColor = Fade(RED, 0.4f);
        textColor = WHITE;
    } else if (isHovering) {
        buttonColor = Fade(LIGHTGRAY, 0.8f);
        textColor = BLACK;
    } else {
        buttonColor = Fade(DARKGRAY, 0.0f);
        textColor = WHITE;
    }
    
    float roundness = 0.3f;
    int segments = 0;
    DrawRectangleRounded(Rectangle{x, y, width, height}, roundness, segments, buttonColor);
    
    std::string displayText = isEditingThis ? (isGamepad ? "Press Button..." : "Press Key...") : getInputName(inputValue, isGamepad);
    int fontSize = (isHovering || isEditingThis) ? 18 : 16; 
    int textWidth = MeasureText(displayText.c_str(), fontSize);
    std::unordered_map<std::string, Font> font = ResourceManager::getFont();
    DrawTextEx(font["DejavuSans"], displayText.c_str(), {(float)(x + width/2 - textWidth/2), (float)(y + height/2 - fontSize/2)}, (float)fontSize, 0, textColor);
}

std::string SettingState::getInputName(int inputValue, bool isGamepad) {
    if (isGamepad) {
        return getGamepadButtonName(inputValue);
    } else {
        return getKeyName(inputValue);
    }
}

std::string SettingState::getKeyName(int keyValue) {
    switch (keyValue) {
        case KEY_A: return "A";
        case KEY_B: return "B";
        case KEY_C: return "C";
        case KEY_D: return "D";
        case KEY_E: return "E";
        case KEY_F: return "F";
        case KEY_G: return "G";
        case KEY_H: return "H";
        case KEY_I: return "I";
        case KEY_J: return "J";
        case KEY_K: return "K";
        case KEY_L: return "L";
        case KEY_M: return "M";
        case KEY_N: return "N";
        case KEY_O: return "O";
        case KEY_P: return "P";
        case KEY_Q: return "Q";
        case KEY_R: return "R";
        case KEY_S: return "S";
        case KEY_T: return "T";
        case KEY_U: return "U";
        case KEY_V: return "V";
        case KEY_W: return "W";
        case KEY_X: return "X";
        case KEY_Y: return "Y";
        case KEY_Z: return "Z";
        case KEY_SPACE: return "SPACE";
        case KEY_LEFT: return "←";
        case KEY_RIGHT: return "→";
        case KEY_UP: return "↑";
        case KEY_DOWN: return "↓";
        case KEY_LEFT_CONTROL: return "L-CTRL";
        case KEY_RIGHT_CONTROL: return "R-CTRL";
        case KEY_LEFT_SHIFT: return "L-SHIFT";
        case KEY_RIGHT_SHIFT: return "R-SHIFT";
        case KEY_ENTER: return "ENTER";
        case KEY_TAB: return "TAB";
        case KEY_BACKSPACE: return "BACKSPACE";
        case KEY_DELETE: return "DELETE";
        case KEY_HOME: return "HOME";
        case KEY_END: return "END";
        case KEY_PAGE_UP: return "PG UP";
        case KEY_PAGE_DOWN: return "PG DN";
        case KEY_INSERT: return "INSERT";
        case KEY_F1: return "F1";
        case KEY_F2: return "F2";
        case KEY_F3: return "F3";
        case KEY_F4: return "F4";
        case KEY_F5: return "F5";
        case KEY_F6: return "F6";
        case KEY_F7: return "F7";
        case KEY_F8: return "F8";
        case KEY_F9: return "F9";
        case KEY_F10: return "F10";
        case KEY_F11: return "F11";
        case KEY_F12: return "F12";
        case KEY_ZERO: return "0";
        case KEY_ONE: return "1";
        case KEY_TWO: return "2";
        case KEY_THREE: return "3";
        case KEY_FOUR: return "4";
        case KEY_FIVE: return "5";
        case KEY_SIX: return "6";
        case KEY_SEVEN: return "7";
        case KEY_EIGHT: return "8";
        case KEY_NINE: return "9";
        default: return "UNKNOWN";
    }
}

std::string SettingState::getFunctionName(const std::string& keyName) {
    if (keyName == "LEFT") return "Move Left";
    else if (keyName == "RIGHT") return "Move Right";
    else if (keyName == "UP") return "Jump";
    else if (keyName == "DOWN") return "Duck";
    else if (keyName == "CONTROL") return "Attack";
    else if (keyName == "SHIFT") return "Run";
    return "Unknown";
}

void SettingState::checkInputButtonClick(Vector2 mousePos, Rectangle buttonRect, ModePlayer curModePlayer, std::string inputName, bool isGamepad) {
    if (CheckCollisionPointRec(mousePos, buttonRect)) {
        if (isEditing) {
            isEditing = false;
        } else {
            isEditing = true;
            isEditingGamepad = isGamepad;
            this->curModePlayer = curModePlayer;
            this->curInputName = inputName;
            this->curRect = buttonRect;
        }
    }
}

bool SettingState::isMouseInClippingArea() const {
    Vector2 mousePos = GetMousePosition();
    return (mousePos.y >= 50 && mousePos.y <= GetScreenHeight() - 100);
}

void SettingState::updateConflictStatus() {
    isConflict = false;
    std::vector<std::string> keyNames = {"LEFT", "RIGHT", "UP", "DOWN", "CONTROL", "SHIFT"};
    std::vector<ModePlayer> modes = {ModePlayer::ONEPLAYER, ModePlayer::FIRSTPLAYER, ModePlayer::SECONDPLAYER};
    for (ModePlayer mode : modes) {
        auto& keys = keyManager->getKeys(mode);
        for (const std::string& keyName : keyNames) {
            if (keys.find(keyName) != keys.end()) {
                int keyValue = keys[keyName];
                if (keyManager->isKeyConflicted(mode, keyName, keyValue)) {
                    isConflict = true;
                    return;
                }
            }
        }
    }
    for (ModePlayer mode : modes) {
        auto& buttons = gamepadManager->getButtons(mode);
        for (const std::string& buttonName : keyNames) {
            if (buttons.find(buttonName) != buttons.end()) {
                int buttonValue = buttons[buttonName];
                if (gamepadManager->isButtonConflicted(mode, buttonName, buttonValue)) {
                    isConflict = true;
                    return;
                }
            }
        }
    }
}

void SettingState::updateConfirmDefaultSetting() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        Rectangle yesButton = {(float)GetScreenWidth()/2 - 120, (float)GetScreenHeight()/2 + 80, 100, 50};
        if (CheckCollisionPointRec(mousePos, yesButton)) {
            keyManager->setDefaultKeyManager();
            keyManager->saveCurrentKeyManager();
            gamepadManager->setDefaultGamepadManager();
            gamepadManager->saveCurrentGamepadManager();
            updateConflictStatus();
            isDefaultNotified = false;
        }
        Rectangle noButton = {(float)GetScreenWidth()/2 + 20, (float)GetScreenHeight()/2 + 80, 100, 50};
        if (CheckCollisionPointRec(mousePos, noButton)) {
            isDefaultNotified = false;
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        isDefaultNotified = false;
    }
}

void SettingState::drawConfirmDefaultSetting() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
    Rectangle dialogBox = {(float)GetScreenWidth()/2 - 350, (float)GetScreenHeight()/2 - 150, 700, 300};
    DrawRectangleRounded(dialogBox, 0.2f, 0, Fade(DARKGRAY, 0.9f));
    
    std::unordered_map<std::string, Font> font = ResourceManager::getFont();
    const char* titleText = "Initialize setting?";
    int titleFontSize = 36;
    int titleWidth = MeasureText(titleText, titleFontSize);
    DrawTextEx(font["DejavuSans"], titleText, {(float)(GetScreenWidth()/2 - titleWidth/2), (float)(GetScreenHeight()/2 - 60)}, (float)titleFontSize, 0, WHITE);

    Rectangle yesButton = {(float)GetScreenWidth()/2 - 120, (float)GetScreenHeight()/2 + 80, 100, 50};
    Vector2 mousePos = GetMousePosition();
    bool yesHover = CheckCollisionPointRec(mousePos, yesButton);

    Color yesColor = yesHover ? Fade(GREEN, 0.8f) : Fade(DARKGREEN, 0.6f);
    DrawRectangleRounded(yesButton, 0.3f, 0, yesColor);

    const char* yesText = "Yes";
    int yesFontSize = 24;
    int yesTextWidth = MeasureText(yesText, yesFontSize);
    DrawTextEx(font["DejavuSans"], yesText, {(float)(yesButton.x + yesButton.width/2 - yesTextWidth/2), (float)(yesButton.y + yesButton.height/2 - yesFontSize/2)}, (float)yesFontSize, 0, WHITE);

    // No button  
    Rectangle noButton = {(float)GetScreenWidth()/2 + 20, (float)GetScreenHeight()/2 + 80, 100, 50};
    bool noHover = CheckCollisionPointRec(mousePos, noButton);

    Color noColor = noHover ? Fade(RED, 0.8f) : Fade(DARKGRAY, 0.6f);
    DrawRectangleRounded(noButton, 0.3f, 0, noColor);

    const char* noText = "No";
    int noFontSize = 24;
    int noTextWidth = MeasureText(noText, noFontSize);
    DrawTextEx(font["DejavuSans"], noText, {(float)(noButton.x + noButton.width/2 - noTextWidth/2), (float)(noButton.y + noButton.height/2 - noFontSize/2)}, (float)noFontSize, 0, WHITE);
}

void SettingState::updateConflictNotification() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        Rectangle okButton = {(float)GetScreenWidth()/2 - 50, (float)GetScreenHeight()/2 + 80, 100, 50};
        if (CheckCollisionPointRec(mousePos, okButton)) {
            isConflictNotified = false;
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        isConflictNotified = false;
    }
}

void SettingState::drawConflictNotification() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
    
    // Main dialog box
    Rectangle dialogBox = {(float)GetScreenWidth()/2 - 350, (float)GetScreenHeight()/2 - 150, 700, 300};
    DrawRectangleRounded(dialogBox, 0.2f, 0, Fade(DARKGRAY, 0.9f));
    
    std::unordered_map<std::string, Font> font = ResourceManager::getFont();
    // Title text
    const char* titleText = "Cannot save. Settings conflict detected.";
    int titleFontSize = 32;
    int titleWidth = MeasureText(titleText, titleFontSize);
    DrawTextEx(font["DejavuSans"], titleText, {(float)(GetScreenWidth()/2 - titleWidth/2 + 20), (float)(GetScreenHeight()/2 - 60)}, (float)titleFontSize, 0, WHITE);

    // OK button
    Rectangle okButton = {(float)GetScreenWidth()/2 - 50, (float)GetScreenHeight()/2 + 80, 100, 50};
    Vector2 mousePos = GetMousePosition();
    bool okHover = CheckCollisionPointRec(mousePos, okButton);

    Color okColor = okHover ? Fade(BLUE, 0.8f) : Fade(DARKBLUE, 0.6f);
    DrawRectangleRounded(okButton, 0.3f, 0, okColor);

    const char* okText = "OK";
    int okFontSize = 24;
    int okTextWidth = MeasureText(okText, okFontSize);
    DrawTextEx(font["DejavuSans"], okText, {(float)(okButton.x + okButton.width/2 - okTextWidth/2), (float)(okButton.y + okButton.height/2 - okFontSize/2)}, (float)okFontSize, 0, WHITE);
}

void SettingState::updateGamepadControlSetting() {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldMousePos = GetScreenToWorld2D(mousePos, camera);
    bool mouseInClipping = isMouseInClippingArea();
    
    if(isEditing && isEditingGamepad) {
        curGamepadButtonValue = -1;
        // Check all gamepad buttons
        for (int buttonValue = 0; buttonValue < 32; buttonValue++) {
            int gamepadID = (curModePlayer == ModePlayer::SECONDPLAYER) ? 1 : 0;
            if (IsGamepadAvailable(gamepadID) && IsGamepadButtonPressed(gamepadID, buttonValue)) {
                curGamepadButtonValue = buttonValue;
                break;
            }
        }
        
        if (curGamepadButtonValue != -1) {
            gamepadManager->setButton(curModePlayer, curInputName, curGamepadButtonValue);
            gamepadManager->saveCurrentGamepadManager();
            
            // Cập nhật trạng thái conflict của toàn bộ hệ thống
            updateConflictStatus();
            
            isEditing = false;
        }
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseInClipping && !CheckCollisionPointRec(worldMousePos, curRect)) {
            isEditing = false;
        }
    }
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseInClipping) {
        float startY;
        if(stateBeforeSetting == GameStateType::TITLE_SCREEN) {
            startY = 1600.0f;  // 650 + 900 + 50 buffer
        } else {
            startY = 1750.0f;  // 800 + 900 + 50 buffer
        }
        float currentY = startY + 50.0f;
        float fadeX = 100.0f;
        currentY += 50.0f;
        float columnSpacing = 50.0f;
        float columnWidth = 400.0f;
        float startColumn1X = fadeX + columnSpacing;
        float startColumn2X = startColumn1X + columnWidth + columnSpacing;
        float startColumn3X = startColumn2X + columnWidth + columnSpacing;
        float rowHeight = 50.0f;
        float buttonHeight = 50.0f;
        std::vector<std::string> buttonNames = {"UP", "DOWN", "CONTROL", "SHIFT"};
        
        for (int i = 0; i < 4; i++) {
            float rowY = currentY + (i * rowHeight);
            checkInputButtonClick(worldMousePos, Rectangle{startColumn2X, rowY, 2*columnWidth + columnSpacing, buttonHeight}, ModePlayer::ONEPLAYER, buttonNames[i], true);
        }
        
        currentY += (4 * rowHeight) + 50.0f;
        currentY += 50.0f;
        currentY += 50.0f;
        for (int i = 0; i < 4; i++) {
            float rowY = currentY + (i * rowHeight);
            checkInputButtonClick(worldMousePos, Rectangle{startColumn2X, rowY, columnWidth, buttonHeight}, ModePlayer::FIRSTPLAYER, buttonNames[i], true);
            checkInputButtonClick(worldMousePos, Rectangle{startColumn3X, rowY, columnWidth, buttonHeight}, ModePlayer::SECONDPLAYER, buttonNames[i], true);
        }
    }
}

void SettingState::drawGamepadControlSetting() {
    std::unordered_map<std::string, Font> font = ResourceManager::getFont();
    float startPositionY;
    float fadeX = 100.0f;
    float fadeWidth = 1400.0f;
    float fadeHeight = 400.0f + 4 * 50.0f + 50.0f + 50.0f + 4 * 50.0f + 50.0f + 50.0f; // estimate for both single/multi player
    if(stateBeforeSetting == GameStateType::TITLE_SCREEN) {
        startPositionY = 1600.0f;
        DrawRectangle(fadeX, startPositionY, fadeWidth, 400 + 4 * 50.0f + 200.0f, Fade(BLACK, 0.7f));
    } else {
        startPositionY = 1750.0f;
        DrawRectangle(fadeX, startPositionY, fadeWidth, 400 + 4 * 50.0f + 200.0f, Fade(BLACK, 0.7f));
    }
    float currentY = startPositionY + 50.0f;
    std::vector<std::string> buttonNames = {"UP", "DOWN", "CONTROL", "SHIFT"};
    float columnSpacing = 50.0f;
    float columnWidth = 400.0f;
    float startColumn1X = fadeX + columnSpacing;
    float startColumn2X = startColumn1X + columnWidth + columnSpacing;
    float startColumn3X = startColumn2X + columnWidth + columnSpacing;
    float rowHeight = 50.0f;
    float buttonHeight = 50.0f;

    DrawTextEx(font["DejavuSans"], "GAMEPAD CONTROLS", {(float)(GetScreenWidth()/2.0f - MeasureText("GAMEPAD CONTROLS", 40)/2), (float)(startPositionY + 20)}, 40.0f, 0, WHITE);
    
    DrawTextEx(font["DejavuSans"], "SINGLE PLAYER", {(float)(GetScreenWidth()/2.0f - MeasureText("SINGLE PLAYER", 30)/2), (float)currentY}, 30.0f, 0, YELLOW);
    currentY += 50.0f;

    auto& onePlayerButtons = gamepadManager->getButtons(ModePlayer::ONEPLAYER);

    for (int i = 0; i < 4; i++) {
        std::string buttonName = buttonNames[i];
        float rowY = currentY + (i * rowHeight);

        std::string functionName = getFunctionName(buttonName);
        DrawTextEx(font["DejavuSans"], functionName.c_str(), {(float)startColumn1X, (float)rowY}, 40.0f, 0, WHITE);
        drawInputButton(ModePlayer::ONEPLAYER, buttonName, onePlayerButtons[buttonName], startColumn2X, rowY, 2*columnWidth + columnSpacing, buttonHeight, true);
    }
    currentY += (4 * rowHeight) + 50.0f;

    DrawTextEx(font["DejavuSans"], "MULTIPLAYER", {(float)(GetScreenWidth()/2.0f - MeasureText("MULTIPLAYER", 30)/2), (float)currentY}, 30.0f, 0, YELLOW);
    currentY += 50.0f;
    
    DrawTextEx(font["DejavuSans"], "FIRST PLAYER", {(float)(startColumn2X + columnWidth/2.0f - MeasureText("FIRST PLAYER", 20)/2), (float)currentY}, 20.0f, 0, WHITE);
    DrawTextEx(font["DejavuSans"], "SECOND PLAYER", {(float)(startColumn3X + columnWidth/2.0f - MeasureText("SECOND PLAYER", 20)/2), (float)currentY}, 20.0f, 0, WHITE);
    currentY += 50.0f;
    
    auto& firstPlayerButtons = gamepadManager->getButtons(ModePlayer::FIRSTPLAYER);
    auto& secondPlayerButtons = gamepadManager->getButtons(ModePlayer::SECONDPLAYER);
    
    for (int i = 0; i < 4; i++) {
        std::string buttonName = buttonNames[i];
        float rowY = currentY + (i * rowHeight);
        
        std::string functionName = getFunctionName(buttonName);
        DrawTextEx(font["DejavuSans"], functionName.c_str(), {(float)startColumn1X, (float)(rowY + 10)}, 40.0f, 0, WHITE);
        drawInputButton(ModePlayer::FIRSTPLAYER, buttonName, firstPlayerButtons[buttonName], startColumn2X, rowY, columnWidth, buttonHeight, true);
        drawInputButton(ModePlayer::SECONDPLAYER, buttonName, secondPlayerButtons[buttonName], startColumn3X, rowY, columnWidth, buttonHeight, true);
    }
}

std::string SettingState::getGamepadButtonName(int buttonValue) {
    switch (buttonValue) {
        case GAMEPAD_BUTTON_LEFT_FACE_UP: return "↑";
        case GAMEPAD_BUTTON_LEFT_FACE_RIGHT: return "→";
        case GAMEPAD_BUTTON_LEFT_FACE_DOWN: return "↓";
        case GAMEPAD_BUTTON_LEFT_FACE_LEFT: return "←";
        case GAMEPAD_BUTTON_RIGHT_FACE_UP: return "Y";
        case GAMEPAD_BUTTON_RIGHT_FACE_RIGHT: return "B";
        case GAMEPAD_BUTTON_RIGHT_FACE_DOWN: return "A";
        case GAMEPAD_BUTTON_RIGHT_FACE_LEFT: return "X";
        case GAMEPAD_BUTTON_LEFT_TRIGGER_1: return "LB";
        case GAMEPAD_BUTTON_LEFT_TRIGGER_2: return "LT";
        case GAMEPAD_BUTTON_RIGHT_TRIGGER_1: return "RB";
        case GAMEPAD_BUTTON_RIGHT_TRIGGER_2: return "RT";
        case GAMEPAD_BUTTON_MIDDLE_LEFT: return "SELECT";
        case GAMEPAD_BUTTON_MIDDLE: return "HOME";
        case GAMEPAD_BUTTON_MIDDLE_RIGHT: return "START";
        case GAMEPAD_BUTTON_LEFT_THUMB: return "L3";
        case GAMEPAD_BUTTON_RIGHT_THUMB: return "R3";
        default: return "BUTTON_" + std::to_string(buttonValue);
    }
}