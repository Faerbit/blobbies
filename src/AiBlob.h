//
// Created by fab on 03.12.17.
//

#ifndef LD40_AIBLOB_H
#define LD40_AIBLOB_H


#include "Blob.h"

class AiBlob : public Blob {
public:
    AiBlob(glm::vec2 start, glm::vec2 direction, float size) : Blob(start, size), direction(direction) {}

    virtual bool update() override;
    static void spawn();

private:
    glm::vec2 direction{};
};


#endif //LD40_AIBLOB_H
