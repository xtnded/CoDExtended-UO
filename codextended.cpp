#include "codextended.hpp"
#include <math.h>

cvar_t* player_jumpheight;
cvar_t* jump_slowdownenable;
cvar_t* g_steep;

int StuckInPlayer(int a1) {
    int tmp = (int)dlsym(gamelib, "PM_GetEffectiveStance");
    if(jump_slowdownenable->integer)
        *(byte*)(tmp+0x106F) = 0x7f;
    else
        *(byte*)(tmp+0x106F) = 0xeb;
    
    if(g_steep->integer) {
        *(byte*)(tmp+0x36D2) = 0x90;
        *(byte*)(tmp+0x36D3) = 0x90;
    }
    
    *(float*)(tmp+0x10AC) = player_jumpheight->value;
    return 0;
}

int unprotect_lib(char *libname) {
    char buf[512];
    char flags[4];
    void *low, *high;
    FILE *fp;
    fp = fopen("/proc/self/maps", "r");
    if(!fp) return 0;

    while(fgets(buf, sizeof(buf), fp)) {
        if(!strstr(buf, libname)) continue;
        if(sscanf(buf, "%p-%p %4c", &low, &high, flags) != 3) continue;

        mprotect((void *)low, (int)high-(int)low, PROT_READ | PROT_WRITE | PROT_EXEC);
    }

    fclose(fp);
    return 1;
}

void *Sys_LoadDll(char *name, char *dest, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...)) {
    char *err;
    char *error;
    char *fn;
    char *gamedir;
    char *basepath;
    char *homepath;
    char *pwdpath;
    char fname[100];
    void *dllEntry;
    void *libHandle;
    
    void *(*OSys_LoadDll)(char *name, char *dest, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...));
    *(int *)&OSys_LoadDll = 0x80C9071; // 1.5 0x080D3DAD
    void *ret = OSys_LoadDll(name, dest, entryPoint, systemcalls);

    char libn[512];
    char* check = Cvar_VariableString("fs_game");
    if(check[0] == '\0')
        sprintf(libn, "main/game.mp.uo.i386.so");
    else
        sprintf(libn, "%s/game.mp.uo.i386.so", check);
    unprotect_lib(libn);
    
    gamelib = ret;
    base = (int)dlsym(ret, "vmMain");
    gentities = (int)dlsym(ret, "g_entities");
    zpml = (char (*)[140])dlsym(ret, "pml");
    zpm = (int)dlsym(ret, "pm");
    for(int i = 0; i < MAX_ENTITY_SIZE; i++) {
        game->entities[i].index = i;
        game->entities[i].base = gentities + GENTITY_SIZE * i;
        game->entities[i].ptr = gentities + GENTITY_SIZE * i;
    }
    scriptInitializing();
    
    // int stuck = (int)dlsym(ret, "StuckInClient");
    // __jmp(stuck, (int)StuckInPlayer);
    
    return ret;
}

CODEXTENDED::CODEXTENDED() {
    setbuf(stdout, NULL);
    mprotect((void *)0x08048000, 0x135000, PROT_READ | PROT_WRITE | PROT_EXEC);
    printf("Compiled: " __DATE__ " " __TIME__ "\n");
    
    __call(0x0809E8ED, (int)Script_GetCustomFunction); // 1.5 0x809D975
    __call(0x0809EB29, (int)Script_GetCustomMethod); // 1.5 0x809DBB1
    __call(0x0809BE20, (int)Sys_LoadDll); // 1.5 0x809AEA8
    SV_AddOperatorCommands();
    
    Cvar_Set("sv_hostname", "^7CoDHost");
    player_jumpheight = Cvar_Get("player_jumpheight", "39", 0);
    jump_slowdownenable = Cvar_Get("jump_slowdownenable", "1", 0);
    g_steep = Cvar_Get("g_steep", "0", 0);

    svClientInit();
}

ENTITY* CODEXTENDED::getEntities() {
    return entities;
}

ENTITY* CODEXTENDED::getEntity(unsigned short index) {
    return &entities[index];
}

CODEXTENDED* game;