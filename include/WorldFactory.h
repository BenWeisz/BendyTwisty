#pragma once

#define GL_SILENCE_DEPRECATION

#include "World.h"

class WorldFactory {
    static World *SimpleWorld() {
        World *world = new World();
        return world;
    }

    static World *CheckerboardWorld(const unsigned int windowWidth, const unsigned int windowHeight) {
        World *world = new World();

        return world;
    }
};