#include "Block/BlockFactory.h"

Block* BlockFactory::createBlock(BlockType type, Vector2 position, const std::string& message)	
{
    Block* block = nullptr;
    Vector2 size = {32, 32}; // Default size for blocks
    Color color = WHITE; // Default color

    switch (type) {
        case BlockType::CLOUDBLOCK:
            block = new CloudBlock(position, size, color);
            break;
        case BlockType::EXCLAMATIONBLOCK:
            block = new ExclamationBlock(position, size, color);
            break;
        case BlockType::EYESCLOSED:
            block = new EyesClosedBlock(position, size, color);
            break;
        case BlockType::EYESOPENED:
            block = new EyesOpenedBlock(position, size, color);
            break;
        case BlockType::GLASSBLOCK:
            block = new GlassBlock(position, size, color);
            break;
        case BlockType::INVISIBLEBLOCK:
            block = new InvisibleBlock(position, size, color);
			break;
		case BlockType::MESSAGEBLOCK:
			block = new MessageBlock(position, size, color, message);
			break;
		case BlockType::STONEBLOCK:
			block = new StoneBlock(position, size, color);
			break;
		case BlockType::WOODBLOCK:
			block = new WoodBlock(position, size, color);
			break;
		case BlockType::QUESTIONBLOCK:
			block = new QuestionBlock(position, size, color);
			break;
		case BlockType::QUESTIONFIREFLOWERBLOCK:
			block = new QuestionFireFlowerBlock(position, size, color);
			break;
		case BlockType::QUESTIONMUSHROOMBLOCK:
			block = new QuestionMushroomBlock(position, size, color);
            break;
		block = new QuestionStarBlock(position, size, color);
		break;
	case BlockType::QUESTIONTHREEUPMOONBLOCK:
		block = new QuestionThreeUpMoonBlock(position, size, color);
		break;
	case BlockType::QUESTIONONEUPMUSHROOMBLOCK:
		block = new QuestionOneUpMushroomBlock(position, size, color);
		break;
    }
	
    return block;
}