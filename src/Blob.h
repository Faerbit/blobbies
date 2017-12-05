//
// Created by fab on 02.12.17.
//

#ifndef LD40_BLOB_H
#define LD40_BLOB_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "globals.h"

class Blob {
public:
    Blob() : Blob({0.0f, 0.0f,}, 1.0f, FRIEND_COLOR) {}
    Blob(glm::vec2 pos, float size, glm::vec3 color) : position(pos), size(size), color(color) {}

    virtual bool update() = 0;
    void baseUpdate();
    glm::vec2& getPos() { return position; }
    glm::vec3& getColor() { return color; }
    float getSize() { return size; }

protected:
    glm::vec2 checkBorder(float tolerance);

    glm::vec2 force{0.0f, 0.0f};
    glm::vec2 speed{0.0f, 0.0f};
    glm::vec3 color{0.0, 0.0, 0.0};
    glm::vec2 position{};
    float size{};
};


#endif //LD40_BLOB_H
