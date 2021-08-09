#include "../script.hpp"

void EntCmd_setBounds(int a1) {
    ENTITY* ent = game->getEntity(a1);
    float width = Script_GetFloat(0);
    float height = Script_GetFloat(1);
    if(ent) {
        ent->set(280, &height, sizeof(float));
        ent->set(276, &width, sizeof(float));
        ent->set(272, &width, sizeof(float));
        width *= -1.0;
        ent->set(264, &width, sizeof(float));
        ent->set(260, &width, sizeof(float));
    }
    Script_AddUndefined();
}