//
// Created by fab on 02.12.17.
//

#include "Blob.h"
#include "globals.h"

#include <glm/geometric.hpp>

static const float MAX_SPEED = 0.5f;

glm::vec2 Blob::checkBorder(float tolerance) {
    if (position.x + (size - tolerance) > gameWidth)
        return glm::vec2(1.0, 0.0);
    if (position.x + (size - tolerance) < -gameWidth)
        return glm::vec2(-1.0, 0.0);
    if (position.y + (size - tolerance) > gameHeight)
        return glm::vec2(0.0, -1.0);
    if (position.y + (size - tolerance) < -gameHeight)
        return glm::vec2(0.0, 1.0);
    return glm::vec2();
}

void Blob::baseUpdate() {
    force *= 0.95;
    speed += force;
    if (glm::length(speed) > MAX_SPEED) {
        glm::normalize(speed);
        speed *= MAX_SPEED;
    }
    speed *= 0.98;
    position += speed;
}
