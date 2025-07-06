#include <iostream>
#include "raylib.h"
#include "Block/CloudBlock.h"
#include "Block/WoodBlock.h"
#include "Block/GlassBlock.h"
#include "Common/ResourceManager.h"
#include "Block/StoneBlock.h"
#include "Block/QuestionBlock.h"
#include "Block/EyesOpenedBlock.h"
#include "Block/ExclamationBlock.h"
#include "Block/EyesClosedBlock.h"
#include "Block/MessageBlock.h"
#include "Block/QuestionFireFlowerBlock.h"
#include "Block/QuestionMushroomBlock.h"
#include "Block/QuestionOneUpMushroomBlock.h"
#include "Block/QuestionThreeUpMoonBlock.h"
#include "Block/QuestionStarBlock.h"
#include "Block/InvisibleBlock.h"
void test() {
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);
	InitWindow(800, 600, "Test Window");
	SetTargetFPS(60);
	CloudBlock cb9({100, 100}, {64, 64}, WHITE);
	WoodBlock wb9({200, 100}, {64, 64}, WHITE);
	GlassBlock gb9({300, 100}, {64, 64}, WHITE);
	QuestionBlock qb9({350, 100}, {64, 64}, WHITE);
	StoneBlock sb9({400, 100}, {64, 64}, WHITE);
	EyesOpenedBlock eb9({500, 100}, {64, 64}, WHITE);
	ExclamationBlock exb9({600, 100}, {64, 64}, WHITE);
	EyesClosedBlock ecb9({700, 100}, {64, 64}, WHITE);
	MessageBlock mb9({200, 200}, {64, 64}, WHITE, "Hello, Mario!");
	QuestionFireFlowerBlock qf9({100, 300}, {64, 64}, WHITE);
	QuestionMushroomBlock qm9({200, 300}, {64, 64}, WHITE);
	QuestionOneUpMushroomBlock qom9({300, 300}, {64, 64}, WHITE);
	QuestionThreeUpMoonBlock qtm9({400, 300}, {64, 64}, WHITE);
	QuestionStarBlock qs9({500, 300}, {64, 64}, WHITE);	
	EyesClosedBlock ecb9_2({600, 300}, {64, 64}, WHITE);
	InvisibleBlock ib9({700, 300}, {64, 64}, WHITE);
	ResourceManager::loadResources();
	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		cb9.draw();
		wb9.draw();
		gb9.draw();
		exb9.draw();
		qb9.draw();
		sb9.draw();
		eb9.draw();
		ecb9.draw();
		mb9.draw();
		qf9.draw();
		qm9.draw();
		qom9.draw();
		qtm9.draw();
		qs9.draw();
		ecb9_2.draw();
		ib9.draw();
		EndDrawing();
	}
	ResourceManager::unloadResources();
	CloseWindow();
}

// int main() {
//     std::cout << "Hello, World!" << std::endl;
// 	test();
// }
#include "Game/World.h"

int main() {
		SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    World world(1600, 900, "Project Mario", 60);
    world.run();
    return 0;
}
