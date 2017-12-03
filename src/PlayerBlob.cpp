//
// Created by fab on 03.12.17.
//

#include <iostream>
#include <glm/ext.hpp>
#include "PlayerBlob.h"
#include "globals.h"

bool PlayerBlob::update() {
    const float accelV = 0.02f;
    glm::vec2 accel{};
    if (keyState.up)
        accel.y += accelV;
    if (keyState.down)
        accel.y -= accelV;
    if (keyState.left)
        accel.x -= accelV;
    if (keyState.right)
        accel.x += accelV;
    glm::vec2 border = checkBorder(0.0f);
    if (glm::length(border) > 0.0) {
        speed = glm::reflect(speed, border);
        if (border.x > 0) {
            accel.x = -accelV;
        }
        if (border.x < 0) {
            accel.x = accelV;
        }
        if (border.y > 0) {
            accel.y = accelV;
        }
        if (border.y < 0) {
            accel.y = -accelV;
        }
    }
    force = accel;
    baseUpdate();
    return true;
}
