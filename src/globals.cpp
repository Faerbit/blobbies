//
// Created by fab on 03.12.17.
//

#include "globals.h"

std::random_device randomDevice{};
std::mt19937 randomGenerator{randomDevice()};

float gameWidth{0.0f};
float gameHeight{0.0f};

int enemyCount{0};

std::list<std::unique_ptr<Blob>> blobs {};

KeyState keyState{};

