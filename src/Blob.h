//
// Created by fab on 02.12.17.
//

#ifndef LD40_BLOB_H
#define LD40_BLOB_H

#include <glm/vec2.hpp>

class Blob {
public:
    Blob() : Blob({0.0f, 0.0f,}, 1.0f) {}
    Blob(glm::vec2 pos, float size) : position(pos), size(size) {}

    void accel(const glm::vec2 p);
    void update();
    glm::vec2 getPos() { return position; }

    float getSize() { return size; }

private:
    glm::vec2 force{0.0f, 0.0f};
    glm::vec2 speed{0.0f, 0.0f};
    glm::vec2 position{};
    float size{};
};


#endif //LD40_BLOB_H
