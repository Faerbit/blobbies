//
// Created by fab on 02.12.17.
//

#include "Blob.h"

#include <glm/geometric.hpp>

static const float MAX_SPEED = 0.5f;
static const float MAX_ACCEL = 0.25f;

void Blob::accel(const glm::vec2 p) {
    force += p;
    if (force.length() > MAX_ACCEL) {
        glm::normalize(force);
        force *= MAX_ACCEL;
    }
}

void Blob::update() {
    force *= 0.95;
    speed += force;
    if (glm::length(speed) > MAX_SPEED) {
        glm::normalize(speed);
        speed *= MAX_SPEED;
    }
    speed *= 0.95;
    position += speed;
}
