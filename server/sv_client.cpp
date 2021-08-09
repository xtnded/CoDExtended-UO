#include "server.hpp"

typedef struct {
	char        *name;
	void ( *func )( client_t *cl );
} ucmd_t;

static ucmd_t* ucmds = (ucmd_t*)0x80F4F80; // 1.5 0x80EE6E0

int *clients = (int*)svsclients_ptr;

SV_StopDownload_f_t SV_StopDownload_f = (SV_StopDownload_f_t)0x808C09F; // 1.5 0x808B318
SV_BeginDownload_f_t SV_BeginDownload_f = (SV_BeginDownload_f_t)0x808C25D; // 1.5 0x808B4D6

void SV_BeginDownload(client_t* cl) {
    int argc = Cmd_Argc();

    if(argc > 1) {
        const char* arg1 = Cmd_Argv(1);
        char* dlname = (char*)(cl+68196);
        SV_StopDownload_f(cl);
        if(strstr(arg1, "..") != NULL)
            return;
			
        if(!strstr(arg1, ".pk3") || strstr(arg1, "svr") != NULL || strstr(arg1, "pak") != NULL) {
			printf("WARNING: %s is trying to download %s\n", cl->name, arg1);
            return;
        }
    }
    asm ("movl %0, 8(%%esp)"::"r"(cl));
    SV_BeginDownload_f(cl);
}

void SV_DumpUcmd() {
	printf("Listing ucmds\n");
	ucmd_t  *u;
	for( u = ucmds ; u->name ; u++ ) {
		printf("ucmd: %s [%x]\n", u->name, (int)u->func);
	}
}

/*
void G_Say(gentity_t *ent, gentity_t *target, int mode, const char *msg) {
	int result;
			
	void(*OG_Say)(gentity_t *ent, gentity_t *target, int mode, const char *chatText);
	*(int*)&OG_Say = (int)dlsym(gamelib, "G_Say");

	char line[1024] = {0};
	int i,j;
	
	j = 0;
	
	for(i = 0; i < strlen(msg); i++) {
		if(i >= 1023)
			break;
		if(msg[i] < 32 || msg[i] > 126)
			continue;
		line[j++] = msg[i];
	}
	
    /*
	if(callbackPlayerCommand) {
		Scr_AddString(line);
		result = Scr_ExecEntThread(ent->s.number, 0, callbackPlayerCommand, 1);
		Scr_FreeThread(result);
	}
    */
	
    /*
	if(!Scr_Continue())
		return;
    */
		
    /*
	int tmp = *(int*)( (int)ent->client + 8400);
	if(tmp && !x_deadchat->integer)
		return;
    */	
/*
	OG_Say(ent, NULL, mode, line);
}
*/

void svClientInit() {
	*(int*)0x80F4FA4 = (int)SV_BeginDownload; // 1.5 0x80EE704
	Cmd_AddCommand("dumpucmd", SV_DumpUcmd);

    /*
    Direction	Type	Address	                Text
    Down	    p	    sub_495A4+DD	        call    _G_Say
    Down	    p	    sub_4968F+DD	        call    _G_Say
    Down	    p	    sub_4977A+DD	        call    _G_Say
    Down	    p	    sub_4C037+66	        call    _G_Say
    Down	    p	    sub_4C0A8+1AC	        call    _G_Say
    Down	    p	    sub_4C0A8+1CD	        call    _G_Say
    Down	    p	    Cmd_GameCommand_f+102	call    _G_Say
    Down	    p	    Cmd_GameCommand_f+12A	call    _G_Say
    */
	
	/*
    int cmm_off = (int)dlsym(gamelib, "Cmd_GameCommand_f");
	__call(clientcommand_off + 0x6EE, (int)hG_Say);
	__call(clientcommand_off + 0x6FE, (int)hG_Say);
	
    int g_say_off = (int)dlsym(gamelib, "G_Say");
	__call(g_say_off + 0x5EA, (int)hG_Say);
	__call(g_say_off + 0x791, (int)hG_Say);
	__call(g_say_off + 0x77D, (int)hG_Say);
	*/
}