#include "GameState/SettingState.h"
#include "GameState/PlayingState.h"
#include "GameState/TitleScreenState.h"
#include "GameState/CountingPointState.h"
#include "GameState/GoNextMapState.h"
#include "GameState/IrisOutState.h"
#include "GameState/TimeUpState.h"
#include "Common/ResourceManager.h"
#include "Common/AudioManager.h"
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
    DrawText(displayText, buttonRect.x + buttonRect.width/2 - textWidth/2, 
             buttonRect.y + buttonRect.height/2 - fontSize/2, fontSize, textColor);
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
    camera(world->getCamera()), keyManager(world->getKeyManager()){
        camera->offset = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
        camera->target = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
        camera->rotation = 0.0f;
        camera->zoom = 1.0f;
        updateConflictStatus();
}

SettingState::~SettingState() {
    
}

void SettingState::update() {
    if(isDefaultNotified) {
        updateConfirmDefaultSetting();
        return;
    }
    else if(isKeyConflictedNotified) {
        updateKeyConflictNotification();
        return;
    }
    float availableHeight = GetScreenHeight() - 100;
    float contentHeight = (stateBeforeSetting == GameStateType::TITLE_SCREEN) ? 1200.0f : 1350.0f;
    float maxHeight = contentHeight;
    float wheelMove = GetMouseWheelMove();
    if(wheelMove != 0) {
        camera->target.y -= wheelMove * 80.0f;
        if(camera->target.y < 450.0f) {
            camera->target.y = 450.0f; 
        } else if(camera->target.y > maxHeight) {
            camera->target.y = maxHeight; 
        }
    }
    updateVolumeAndButtonSetting();
    updateKeyControlSetting();
    updateRestoreDefaultButton();
}

void SettingState::draw() {
    DrawTextureRec(ResourceManager::getTexture()["SettingBackground"], Rectangle{(float)backgroundPositionx, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, Vector2{0, 0}, WHITE);
    BeginScissorMode(0, 50, GetScreenWidth(), GetScreenHeight() - 150);
    BeginMode2D(*camera);
    drawVolumeAndButtonSetting();
    drawKeyControlSetting();
    EndMode2D();
    EndScissorMode();
    drawRestoreDefaultButton();
    if(isDefaultNotified) {
        drawConfirmDefaultSetting();
    } else if(isKeyConflictedNotified) {
        drawKeyConflictNotification();
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
        musicVolumeSlider.update(camera);
        sfxVolumeSlider.update(camera);
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
            returnButton.update(camera);
            exitButton.update(camera);
        }
    } else {
        if (mouseInClipping) {
            resumeButton.update(camera);
            restartButton.update(camera);
            returnButton.update(camera);
            exitButton.update(camera);
        }
    }
    if (mouseInClipping) {
        if(resumeButton.isPressed(camera) && stateBeforeSetting != GameStateType::TITLE_SCREEN) {
            if(world->getGamePlay() == GamePlay::PLAYDEVELOPEDMAP) {
                SaveGame::loadGame(*world);
            }
            else {
                SaveGame::loadGame(*world, "../resources/SaveGame/" + world->getMap()->getMapFileName() + ".json");
            }
        }
        else if(restartButton.isPressed(camera) && stateBeforeSetting != GameStateType::TITLE_SCREEN) {
            world->resetMap();
        }
        else if(returnButton.isPressed(camera)) {
            if (isConflict) {
                isKeyConflictedNotified = true;
            } else {
                world->resetGame();
            }
        }
        else if(exitButton.isPressed(camera)) {
            world->setIsClosed(true);
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (isConflict) {
            isKeyConflictedNotified = true;
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
    Vector2 worldMousePos = GetScreenToWorld2D(mousePos, *camera);
    bool mouseInClipping = isMouseInClippingArea();
    
    if(isEditingKey) {
        curKeyValue = KeyboardKey::KEY_NULL;
        for (int keyValue = 0; keyValue < 512; keyValue++) {
            if (IsKeyPressed(keyValue)) {
                curKeyValue = static_cast<KeyboardKey>(keyValue);
                break;
            }
        }
        if (curKeyValue != KeyboardKey::KEY_NULL) {
            keyManager->setKey(curModePlayer, curKeyName, static_cast<int>(curKeyValue));
            keyManager->saveCurrentKeyManager();
            
            // Cập nhật trạng thái conflict của toàn bộ hệ thống
            updateConflictStatus();
            
            isEditingKey = false;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseInClipping && !CheckCollisionPointRec(worldMousePos, curRect)) {
            isEditingKey = false;
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
            checkKeyButtonClick(worldMousePos, Rectangle{startColumn2X, rowY, 2*columnWidth + columnSpacing, buttonHeight}, ModePlayer::ONEPLAYER, keyNames[i]);
        }
        
        currentY += (6 * rowHeight) + 50.0f;
        currentY += 50.0f;
        currentY += 50.0f;
        for (int i = 0; i < 6; i++) {
            float rowY = currentY + (i * rowHeight);
            checkKeyButtonClick(worldMousePos, Rectangle{startColumn2X, rowY, columnWidth, buttonHeight}, ModePlayer::FIRSTPLAYER, keyNames[i]);
            checkKeyButtonClick(worldMousePos, Rectangle{startColumn3X, rowY, columnWidth, buttonHeight}, ModePlayer::SECONDPLAYER, keyNames[i]);
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
    DrawText("KEY CONTROLS", GetScreenWidth()/2.0f - MeasureText("KEY CONTROLS", 40)/2, startPositionY + 20, 40, WHITE);

    float currentY = startPositionY + 50.0f;
    std::vector<std::string> keyNames = {"LEFT", "RIGHT", "UP", "DOWN", "CONTROL", "SHIFT"};
    float fadeX = 100.0f;
    float fadeWidth = 1400.0f;
    
    DrawText("SINGLE PLAYER", GetScreenWidth()/2.0f - MeasureText("SINGLE PLAYER", 30)/2, currentY, 30, YELLOW);
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
        DrawText(functionName.c_str(), startColumn1X, rowY, 40, WHITE);
        drawKeyButton(ModePlayer::ONEPLAYER, keyName, onePlayerKeys[keyName], startColumn2X, rowY, 2*columnWidth + columnSpacing, buttonHeight);
    }
    currentY += (6 * rowHeight) + 50.0f;
    
    DrawText("MULTIPLAYER", GetScreenWidth()/2.0f - MeasureText("MULTIPLAYER", 30)/2, currentY, 30, YELLOW);
    currentY += 50.0f;
    
    DrawText("FIRST PLAYER", startColumn2X + columnWidth/2.0f - MeasureText("FIRST PLAYER", 20), currentY, 20, WHITE);
    DrawText("SECOND PLAYER", startColumn3X + columnWidth/2.0f - MeasureText("SECOND PLAYER", 20), currentY, 20, WHITE);
    currentY += 50.0f;
    
    auto& firstPlayerKeys = keyManager->getKeys(ModePlayer::FIRSTPLAYER);
    auto& secondPlayerKeys = keyManager->getKeys(ModePlayer::SECONDPLAYER);
    
    for (int i = 0; i < 6; i++) {
        std::string keyName = keyNames[i];
        float rowY = currentY + (i * rowHeight);
        
        std::string functionName = getFunctionName(keyName);
        DrawText(functionName.c_str(), startColumn1X, rowY + 10, 40, WHITE);
        drawKeyButton(ModePlayer::FIRSTPLAYER, keyName, firstPlayerKeys[keyName], startColumn2X, rowY, columnWidth, buttonHeight);
        drawKeyButton(ModePlayer::SECONDPLAYER, keyName, secondPlayerKeys[keyName], startColumn3X, rowY, columnWidth, buttonHeight);
    }
}

void SettingState::drawKeyButton(ModePlayer modePlayer, const std::string& keyName, int keyValue, float x, float y, float width, float height) {
    Vector2 mousePos = GetMousePosition();
    mousePos.x += camera->target.x - camera->offset.x;
    mousePos.y += camera->target.y - camera->offset.y;
    
    bool mouseInClipping = isMouseInClippingArea();
    bool isHovering = mouseInClipping && (mousePos.x >= x && mousePos.x <= x + width &&mousePos.y >= y && mousePos.y <= y + height);
    bool isEditing = (isEditingKey && curKeyName == keyName && curModePlayer == modePlayer);
    bool hasConflict = keyManager->isKeyConflicted(modePlayer, keyName, keyValue);
    
    Color buttonColor, borderColor, textColor;
    if (isEditing) {
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
    
    std::string displayText = isEditing ? "Press Key..." : getKeyName(keyValue);
    int fontSize = (isHovering || isEditing) ? 18 : 16; 
    int textWidth = MeasureText(displayText.c_str(), fontSize);
    DrawText(displayText.c_str(), x + width/2 - textWidth/2, y + height/2 - fontSize/2, fontSize, textColor);
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

void SettingState::checkKeyButtonClick(Vector2 mousePos, Rectangle buttonRect, ModePlayer curModePlayer, std::string curKeyName) {
    if (CheckCollisionPointRec(mousePos, buttonRect)) {
        if (isEditingKey) {
            isEditingKey = false;
        } else {
            isEditingKey = true;
            this->curModePlayer = curModePlayer;
            this->curKeyName = curKeyName;
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
}

void SettingState::updateConfirmDefaultSetting() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        Rectangle yesButton = {(float)GetScreenWidth()/2 - 120, (float)GetScreenHeight()/2 + 80, 100, 50};
        if (CheckCollisionPointRec(mousePos, yesButton)) {
            keyManager->setDefaultKeyManager();
            keyManager->saveCurrentKeyManager();
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
    Rectangle dialogBox = {(float)GetScreenWidth()/2 - 250, (float)GetScreenHeight()/2 - 100, 500, 200};
    DrawRectangleRounded(dialogBox, 0.2f, 0, Fade(DARKGRAY, 0.9f));
    
    const char* titleText = "Initialize setting?";
    int titleFontSize = 36;
    int titleWidth = MeasureText(titleText, titleFontSize);
    DrawText(titleText, GetScreenWidth()/2 - titleWidth/2, GetScreenHeight()/2 - 60, titleFontSize, WHITE);

    Rectangle yesButton = {(float)GetScreenWidth()/2 - 120, (float)GetScreenHeight()/2 + 80, 100, 50};
    Vector2 mousePos = GetMousePosition();
    bool yesHover = CheckCollisionPointRec(mousePos, yesButton);
    
    Color yesColor = yesHover ? Fade(GREEN, 0.8f) : Fade(DARKGREEN, 0.6f);
    DrawRectangleRounded(yesButton, 0.3f, 0, yesColor);
    
    const char* yesText = "Yes";
    int yesFontSize = 24;
    int yesTextWidth = MeasureText(yesText, yesFontSize);
    DrawText(yesText, yesButton.x + yesButton.width/2 - yesTextWidth/2, 
             yesButton.y + yesButton.height/2 - yesFontSize/2, yesFontSize, WHITE);
    
    // No button  
    Rectangle noButton = {(float)GetScreenWidth()/2 + 20, (float)GetScreenHeight()/2 + 80, 100, 50};
    bool noHover = CheckCollisionPointRec(mousePos, noButton);
    
    Color noColor = noHover ? Fade(RED, 0.8f) : Fade(DARKGRAY, 0.6f);
    DrawRectangleRounded(noButton, 0.3f, 0, noColor);
    
    const char* noText = "No";
    int noFontSize = 24;
    int noTextWidth = MeasureText(noText, noFontSize);
    DrawText(noText, noButton.x + noButton.width/2 - noTextWidth/2,
             noButton.y + noButton.height/2 - noFontSize/2, noFontSize, WHITE);
}

void SettingState::updateKeyConflictNotification() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        Rectangle okButton = {(float)GetScreenWidth()/2 - 50, (float)GetScreenHeight()/2 + 80, 100, 50};
        if (CheckCollisionPointRec(mousePos, okButton)) {
            isKeyConflictedNotified = false;
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        isKeyConflictedNotified = false;
    }
}

void SettingState::drawKeyConflictNotification() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
    
    // Main dialog box
    Rectangle dialogBox = {(float)GetScreenWidth()/2 - 300, (float)GetScreenHeight()/2 - 100, 600, 200};
    DrawRectangleRounded(dialogBox, 0.2f, 0, Fade(DARKGRAY, 0.9f));
    
    // Warning icon (simple exclamation mark)
    DrawText("!", GetScreenWidth()/2 - 300 + 30, GetScreenHeight()/2 - 80, 60, RED);
    
    // Title text
    const char* titleText = "Cannot save. Key already in use.";
    int titleFontSize = 32;
    int titleWidth = MeasureText(titleText, titleFontSize);
    DrawText(titleText, GetScreenWidth()/2 - titleWidth/2 + 20, GetScreenHeight()/2 - 60, titleFontSize, WHITE);
    
    // OK button
    Rectangle okButton = {(float)GetScreenWidth()/2 - 50, (float)GetScreenHeight()/2 + 80, 100, 50};
    Vector2 mousePos = GetMousePosition();
    bool okHover = CheckCollisionPointRec(mousePos, okButton);
    
    Color okColor = okHover ? Fade(BLUE, 0.8f) : Fade(DARKBLUE, 0.6f);
    DrawRectangleRounded(okButton, 0.3f, 0, okColor);
    
    const char* okText = "OK";
    int okFontSize = 24;
    int okTextWidth = MeasureText(okText, okFontSize);
    DrawText(okText, okButton.x + okButton.width/2 - okTextWidth/2,
             okButton.y + okButton.height/2 - okFontSize/2, okFontSize, WHITE);
}