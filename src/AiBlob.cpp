//
// Created by fab on 03.12.17.
//

#include <iostream>
#include <glm/ext.hpp>
#include "AiBlob.h"
#include "globals.h"

glm::vec2 getRandomBorderPosition(int side) {
    if (side == 0) {
        std::uniform_real_distribution<float> posDist(-gameWidth, gameWidth);
        return glm::vec2(posDist(randomGenerator), gameHeight - 1.2);
    }
    else if (side == 1) {
        std::uniform_real_distribution<float> posDist(-gameWidth, gameWidth);
        return glm::vec2(posDist(randomGenerator), -gameHeight - 1.2);
    }
    else if (side == 2) {
        std::uniform_real_distribution<float> posDist(-gameHeight, gameHeight);
        return glm::vec2(gameWidth + 1.2, posDist(randomGenerator));
    }
    else if (side == 3) {
        std::uniform_real_distribution<float> posDist(-gameHeight, gameHeight);
        return glm::vec2(-gameWidth - 1.2, posDist(randomGenerator));
    }
    return glm::vec2();
}

void AiBlob::spawn() {
    std::uniform_int_distribution<> sideDist(0, 3);
    int startSide = sideDist(randomGenerator);
    int targetSide = -1;
    while ((targetSide = sideDist(randomGenerator)) == startSide) {}
    glm::vec2 start {getRandomBorderPosition(startSide)};
    glm::vec2 target {getRandomBorderPosition(targetSide)};
    std::uniform_real_distribution<> sizeDist(1,1.5);
    blobs.emplace_back(std::make_unique<AiBlob>(start, glm::normalize(target-start), sizeDist(randomGenerator)));
}

bool AiBlob::update() {
    PlayerBlob& playerBlob = static_cast<PlayerBlob&>(*blobs.front());
    glm::vec2 toPlayer = playerBlob.getPos() - position;
    if (glm::length(toPlayer) < size + playerBlob.getSize()) {
        playerBlob.grow(size *0.5);
        return false;
    }
    if (glm::angle(direction, glm::normalize(toPlayer)) < (PI/2.0)) {
    //if (glm::angle(direction, glm::normalize(toPlayer)) < (PI/10.0)) {
        direction = glm::normalize(direction + glm::normalize(toPlayer) * 0.1);
    }
    force = direction * 0.01;
    if (glm::length(checkBorder(2*size))) {
        return false;
    }
    baseUpdate();
    return true;
}
