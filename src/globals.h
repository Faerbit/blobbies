//
// Created by fab on 03.12.17.
//

#ifndef LD40_GLOBALS_H
#define LD40_GLOBALS_H

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/vec2.hpp>
#include <vector>
#include <memory>
#include <random>
#include <list>
#include <chrono>
#include "PlayerBlob.h"

const float PI = 3.14159265358979323846f;

extern GLFWwindow* window;
extern int windowWidth;
extern int windowHeight;

extern FT_Library ftLibrary;
extern FT_Face ftFace;

extern GLuint circleVbo;
extern GLuint textVbo;

extern GLint mpLocation;
extern GLint vPosLocation;
extern GLint colorLocation;
extern GLint textCoordLocation;

extern std::vector<glm::vec2> circle;

extern std::random_device randomDevice;
extern std::mt19937 randomGenerator;

// game related
extern bool gameRunning;
extern std::chrono::high_resolution_clock::time_point gameStart;

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
