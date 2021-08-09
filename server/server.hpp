#ifndef SERVER_H
#define SERVER_H

#include "../game/shared.hpp"
#include "../game/script.hpp"

#define svsclients_ptr 0x84F7010 // 1.5 0x83CCDF0
#define svsclients_size 700596

typedef enum {
    CS_FREE,        // can be reused for a new connection
    CS_ZOMBIE,      // client has been disconnected, but don't reuse connection for a couple seconds
    CS_CONNECTED,   // has been assigned to a client_t, but no gamestate yet
    CS_PRIMED,      // gamestate has been sent, but client hasn't sent a usercmd
    CS_ACTIVE       // client is fully in game
} clientState_t;

typedef enum {
  UCMD_BUTTONS = 8, //for messagemode/console, cl_run (+speed) (aim down the sight)
  UCMD_WBUTTONS, //+reload, +leanright +leanleft
  UCMD_FORWARDMOVE = 23,
  UCMD_RIGHTMOVE,
  UCMD_UPMOVE
} usercmd_offset;

typedef struct usercmd_s {
    int serverTime;
    byte buttons;
    byte wbuttons;
    byte weapon;
    byte flags;
    byte unknown1[13];
    /*
        forward = 127
        back = 129
        right = 127
        left = 129
        up = 127
        prone = 129
    */
    signed char forwardmove, rightmove, upmove;
    byte doubleTap;             // Arnout: only 3 bits used

    // rain - in ET, this can be any entity, and it's used as an array
    // index, so make sure it's unsigned
    byte identClient;           // NERVE - SMF
} usercmd_t;

typedef enum {
    NA_BOT,
    NA_BAD,                 // an address lookup failed
    NA_LOOPBACK,
    NA_BROADCAST,
    NA_IP,
    NA_IPX,
    NA_BROADCAST_IPX
} netadrtype_t;

typedef enum {
    NS_CLIENT,
    NS_SERVER
} netsrc_t;

typedef struct {
    netadrtype_t type;
    union {
        byte ip[4];
        unsigned int _ip;
    };
    byte ipx[10];

    unsigned short port;
} netadr_t; //size = 0x14 (20)

typedef struct {
    netsrc_t sock;
    int dropped;
    netadr_t remoteAddress;
    int qport;
    /* lot more bs here */
} netchan_t;

typedef struct client_s {
    clientState_t state;
    int unknown4;
    int unknown8;
    char userinfo[MAX_INFO_STRING];
    byte unknown1032[66064];
    int challenge;
    byte inaccuratelastUserCmd[28];
    int lastClientCommand;
    char lastClientCommandString[MAX_STRING_CHARS];
    int* gentity; // sharedEntity_t *gentity;
    char name[MAX_NAME_LENGTH];
    byte unknown1[270024];
    int ping;
    int rate;
    int snapshotMsec; // requests a snapshot every snapshotMsec unless rate choked
    int pureAuthentic;
    netchan_t netchan;
    int unknown2;
    char lazy_to_figure_out_so_fill_it_up[362318];
} client_t;

extern int* clients;

typedef void (__cdecl *SV_StopDownload_f_t)(client_t*);
extern SV_StopDownload_f_t SV_StopDownload_f;

typedef void (*SV_BeginDownload_f_t)(client_t*);
extern SV_BeginDownload_f_t SV_BeginDownload_f;

typedef void (*SV_Trace_t)( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask, int capsule );
typedef void (QDECL *SV_SendServerCommand_t)(/*client_t**/int, int, const char*, ...); //actually is gamesendservercommand which then checks and then calls sendservercommand
extern SV_Trace_t SV_Trace;
extern SV_SendServerCommand_t SV_SendServerCommand;
#endif // SERVER_H
