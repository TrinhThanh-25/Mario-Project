#ifndef BLOCKFACTORY_H
#define BLOCKFACTORY_H

#include "Block.h"
#include "CloudBlock.h"
#include "ExclamationBlock.h"
#include "EyesClosedBlock.h"
#include "EyesOpenedBlock.h"
#include "GlassBlock.h"
#include "InvisibleBlock.h"
#include "MessageBlock.h"
#include "QuestionBlock.h"
#include "StoneBlock.h"
#include "WoodBlock.h"
#include "QuestionFireFlowerBlock.h"
#include "QuestionMushroomBlock.h"
#include "QuestionStarBlock.h"
#include "QuestionThreeUpMoonBlock.h"
#include "QuestionOneUpMushroomBlock.h"
#include <string>

enum class BlockType {
		CLOUDBLOCK,
		EXCLAMATIONBLOCK,
		EYESCLOSED,
		EYESOPENED,
		GLASSBLOCK,
		INVISIBLEBLOCK,
		MESSAGEBLOCK,
		STONEBLOCK,
		WOODBLOCK,
		QUESTIONBLOCK,
		QUESTIONFIREFLOWERBLOCK,
		QUESTIONMUSHROOMBLOCK,
		QUESTIONSTARBLOCK,
		QUESTIONTHREEUPMOONBLOCK,
		QUESTIONONEUPMUSHROOMBLOCK
	};

class BlockFactory {
public:
    static Block* createBlock(BlockType type, Vector2 position, const std::string& message = "");
};

#endif // BLOCKFACTORY_H