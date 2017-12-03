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

    virtual bool update() = 0;
    void baseUpdate();
    glm::vec2 getPos() { return position; }

    float getSize() { return size; }

protected:
    glm::vec2 checkBorder(float tolerance);

    glm::vec2 force{0.0f, 0.0f};
    glm::vec2 speed{0.0f, 0.0f};
    glm::vec2 position{};
    float size{};
};


#endif //LD40_BLOB_H
