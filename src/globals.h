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

const float PI = 3.14159265358979323846f;

extern GLFWwindow* window;
extern int windowWidth;
extern int windowHeight;

extern GLint mpLocation;
extern GLint vPosLocation;
extern GLint colorLocation;

extern std::vector<glm::vec2> circle;

extern std::random_device randomDevice;
extern std::mt19937 randomGenerator;

// game related
extern float gameWidth;
extern float gameHeight;

extern int enemyCount;

extern std::list<std::unique_ptr<Blob>> blobs;
extern PlayerBlob& playerBlob;

extern struct KeyState {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
} keyState;

#endif //LD40_GLOBALS_H
