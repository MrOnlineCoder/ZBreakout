/*
  ZBreakout Project

  Copyright (c) 2018 Nikita Kogut (Telegram @MrOnlineCoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
  All rights reserved.
  
  Only original author is allowed to edit and share this file.
  Unauthorized copying or editing this file, via any medium is strictly prohibited
  Proprietary and confidential
*/

//STD includes

//Thirdparty includes

//Project includes
#include "Engine.h"

int main(int argc, char* argv[]) {
	Engine* gameEngine = new Engine(argc, argv);
	return gameEngine->run();
}