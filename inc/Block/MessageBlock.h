#ifndef MESSAGE_BLOCK_H
#define MESSAGE_BLOCK_H

#include "Block/Block.h"
#include "Game/Map.h"
#include "Character/Character.h"
#include <string>

class MessageBlock : public Block {
private:
	std::string message;
	float moveTime;
	float moveAcum;
	float moveY;
	bool hasMoved;
	bool showMessage;
	float messageTimer;
public:
    MessageBlock(Vector2 pos, Vector2 size, Color color, std::string message);
	MessageBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames, std::string message);
    ~MessageBlock() override;

    void update() override;
    void draw() override;
	void doHit(Character& character, Map* map) override;
};

#endif