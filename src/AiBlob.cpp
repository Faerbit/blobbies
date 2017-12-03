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
    std::cout << startSide << "\n";
    int targetSide = -1;
    while ((targetSide = sideDist(randomGenerator)) == startSide) {}
    glm::vec2 start {getRandomBorderPosition(startSide)};
    glm::vec2 target {getRandomBorderPosition(targetSide)};
    blobs.emplace_back(std::make_unique<AiBlob>(start, glm::normalize(target-start), 1.5f));
}

bool AiBlob::update() {
    force = direction * 0.01;
    if (glm::length(checkBorder(2*size))) {
        return false;
    }
    baseUpdate();
    return true;
}
