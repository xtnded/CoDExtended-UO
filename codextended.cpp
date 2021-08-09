#include "codextended.hpp"
#include <math.h>

cvar_t* player_jumpheight;

int StuckInPlayer(int a1) {
    int tmp = (int)dlsym(gamelib, "PM_GetEffectiveStance");
    
    *(float*)(tmp+0x11AF) = player_jumpheight->value;
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
        sprintf(libn, "uo/game.mp.uo.i386.so");
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
    printf("sizeof(client_t): %d\n", sizeof(client_t));
    
    __call(0x0809E8ED, (int)Script_GetCustomFunction); // 1.5 0x809D975
    __call(0x0809EB29, (int)Script_GetCustomMethod); // 1.5 0x809DBB1
    __call(0x0809BE20, (int)Sys_LoadDll); // 1.5 0x809AEA8

    __nop(0x808A46F, 6); // Crack server.

    // Unpure fixes, but what fixes? Why fixes?
    *(byte*)0x808DDA7 = 0xeb;
    *(byte*)0x808DACE = 0xeb;

    Cvar_Set("sv_hostname", "^7CoDHost");
    // player_jumpheight = Cvar_Get("player_jumpheight", "39", 0);

    SV_AddOperatorCommands();
    svClientInit();
}

ENTITY* CODEXTENDED::getEntities() {
    return entities;
}

ENTITY* CODEXTENDED::getEntity(unsigned short index) {
    return &entities[index];
}

CODEXTENDED* game;
