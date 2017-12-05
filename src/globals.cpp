//
// Created by fab on 03.12.17.
//

#include <glm/vec3.hpp>
#include "globals.h"
#include "Blob.h"


GLFWwindow* window{nullptr};
int windowWidth{0};
int windowHeight{0};

FT_Library ftLibrary;
FT_Face ftFace;

GLuint circleVbo{0};
GLuint textVbo{0};

GLint mpLocation{0};
GLint vPosLocation{0};
GLint colorLocation{0};
GLint textCoordLocation{0};

std::vector<glm::vec2> circle {};

std::random_device randomDevice{};
std::mt19937 randomGenerator{randomDevice()};

const glm::vec3 FRIEND_COLOR{13/255.0, 7/255.0, 196/255.0};
const glm::vec3 ENEMY_COLOR{200/255.0, 30/255.0, 7/255.0};

bool gameRunning{true};
std::chrono::high_resolution_clock::time_point gameStart{};

float gameWidth{0.0f};
float gameHeight{10.0f};

int enemyCount{0};

std::list<std::unique_ptr<Blob>> blobs {};

KeyState keyState{};

