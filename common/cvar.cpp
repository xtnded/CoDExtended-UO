#include "../game/shared.hpp"

static Cvar_FindVar_t Cvar_FindVar = (Cvar_FindVar_t)0x8072FAE; // 1.5 0x8072996
Cvar_Set_t Cvar_Set = (Cvar_Set_t)0x8073798; // 1.5 0x8073180
Cvar_Get_t Cvar_Get = (Cvar_Get_t)0x8073114; // 1.5 0x8072AFC;

float Cvar_VariableValue(const char *var_name) {
    cvar_t  *var;

    var = Cvar_FindVar(var_name);
    if(!var) {
        return 0;
    }
    return var->value;
}

int Cvar_VariableIntegerValue(const char *var_name) {
    cvar_t  *var;

    var = Cvar_FindVar(var_name);
    if(!var) {
        return 0;
    }
    return var->integer;
}

char *Cvar_VariableString(const char *var_name) {
    cvar_t *var;

    var = Cvar_FindVar(var_name);
    if(!var) {
        return "";
    }
    return var->string;
}

void Cvar_VariableStringBuffer(const char *var_name, char *buffer, int bufsize) {
    cvar_t *var;

    var = Cvar_FindVar(var_name);
    if(!var) {
        *buffer = 0;
    } else {
        Q_strncpyz(buffer, var->string, bufsize);
    }
}

void Cvar_LatchedVariableStringBuffer(const char *var_name, char *buffer, int bufsize) {
    cvar_t *var;

    var = Cvar_FindVar(var_name);
    if(!var) {
        *buffer = 0;
    } else {
        if(var->latchedString) {
            Q_strncpyz(buffer, var->latchedString, bufsize);
        } else {
            Q_strncpyz(buffer, var->string, bufsize);
        }
    }
}