//
// Created by fab on 03.12.17.
//

#ifndef LD40_GLOBALS_H
#define LD40_GLOBALS_H

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <vector>
#include <memory>
#include <random>
#include <list>
#include "PlayerBlob.h"

static GLFWwindow* window = nullptr;
static int windowWidth = 0;
static int windowHeight= 0;

static GLint mpLocation = 0;
static GLint vPosLocation = 0;
static std::vector<glm::vec2> circle {};

extern std::random_device randomDevice;
extern std::mt19937 randomGenerator;

// game related
extern float gameWidth;
extern float gameHeight;

extern int enemyCount;

extern std::list<std::unique_ptr<Blob>> blobs;

extern struct KeyState {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
} keyState;

#endif //LD40_GLOBALS_H
