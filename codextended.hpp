#ifndef CODEXTENDED_H
#define CODEXTENDED_H

#include "game/shared.hpp"
#include "game/script.hpp"
#include "entity.hpp"

struct CODEXTENDED {
    ENTITY entities[MAX_ENTITY_SIZE];
    CODEXTENDED();
    ENTITY* getEntities();
    ENTITY* getEntity(unsigned short);
};

extern CODEXTENDED* game;

#endif // CODEXTENDED_H