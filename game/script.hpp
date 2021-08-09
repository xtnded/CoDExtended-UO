#ifndef SCRIPT_H
#define SCRIPT_H

#include "../codextended.hpp"
#include "shared.hpp"
#include "../entity.hpp"
#include "../server/server.hpp"
#include <math.h>

typedef enum {
	OT_UNDEFINED, //0
	OT_STRING, //1
	OT_LOCALIZED_STRING, //2
	OT_VECTOR, //3
	OT_FLOAT, //4
	OT_INT, //5
	OT_CODEPOS, //6
	OT_OBJECT, //7
	OT_KEY_VALUE, //8
	OT_FUNCTION, //9
	OT_STACK, //10
	OT_ANIMATION, //11
	OT_THREAD, //12
	OT_ENTITY, //13
	OT_STRUCT, //14
	OT_ARRAY, //16
	OT_DEAD_THREAD, //15
	OT_DEAD_ENTITY, //17
	OT_DEAD_OBJECT //18
} objectType;

typedef void (__cdecl *SCRIPTFUNCTIONCALL)(int);

typedef struct {
	const char* name;
	SCRIPTFUNCTIONCALL call;
	int developer;
} SCRIPTFUNCTION;

typedef SCRIPTFUNCTIONCALL (*Script_GetFunction_t)(const char**, int*);
typedef SCRIPTFUNCTIONCALL (*Script_GetMethod_t)(const char**, int*);

extern Script_GetFunction_t Script_GetFunction;
extern Script_GetMethod_t Script_GetMethod;

SCRIPTFUNCTIONCALL Script_GetCustomFunction(const char**, int*);
SCRIPTFUNCTIONCALL Script_GetCustomMethod(const char**, int*);

typedef int (*Script_GetNumParam_t)();
typedef int (*Script_GetPointerType_t)(int);
typedef int (*Script_GetType_t)(int);

typedef void (*Script_MakeArray_t)();
typedef void (*Script_AddArray_t)();
typedef void (*Script_Error_t)(const char*);

typedef int (*Script_AddFloat_t)(float);
typedef int (*Script_AddInt_t)(int);
typedef int (*Script_AddBool_t)(bool);
typedef int (*Script_AddString_t)(char*);
typedef int (*Script_AddVector_t)(vec3_t);
typedef int (*Script_AddUndefined_t)();
typedef void (*Script_AddEntity_t)(int);

typedef bool (*Script_GetBool_t)(int);
typedef int (*Script_GetInt_t)(int);
typedef float (*Script_GetFloat_t)(int);
typedef int (*Script_GetVector_t)(int, vec3_t);
typedef char* (*Script_GetString_t)(int);
typedef int (*Script_GetFunc_t)(int);

extern Script_GetNumParam_t Script_GetNumParam;
extern Script_GetPointerType_t Script_GetPointerType;
extern Script_GetType_t Script_GetType;

extern Script_MakeArray_t Script_MakeArray;
extern Script_AddArray_t Script_AddArray;
extern Script_Error_t Script_Error;

extern Script_AddInt_t Script_AddInt;
extern Script_AddFloat_t Script_FloatInt;
extern Script_AddBool_t Script_AddBool;
extern Script_AddString_t Script_AddString;
extern Script_AddVector_t Script_AddVector;
extern Script_AddUndefined_t Script_AddUndefined;
extern Script_AddEntity_t Script_AddEntity;

extern Script_GetBool_t Script_GetBool;
extern Script_GetInt_t Script_GetInt;
extern Script_GetFloat_t Script_GetFloat;
extern Script_GetVector_t Script_GetVector;
extern Script_GetString_t Script_GetString;
extern Script_GetFunc_t Script_GetFunc;

extern int callbackTest;
extern int callbackPlayerCommand;
extern int callbackEntityDamage;
extern int callbackEntityKilled;
extern int callbackEntityUse;
extern int callbackEntityTouch;
extern int callbackEntityThink;

typedef int (*loadCallback_t)(char*,char*); //loadcallbackscript
typedef int (*Scr_ExecThread_t)(int, int);
typedef int (*Scr_ExecEntThread_t)(int, int, int, int);
typedef int (*Scr_FreeThread_t)(int);

typedef char* (*SL_ConvertToString_t)(unsigned int);

extern loadCallback_t loadCallback;
extern Scr_ExecThread_t Scr_ExecThread;
extern Scr_ExecEntThread_t Scr_ExecEntThread;
extern Scr_FreeThread_t Scr_FreeThread;
extern SL_ConvertToString_t SL_ConvertToString;

void scriptInitializing();

/*
=============
FUNCTIONS
=============
*/

void GScr_printconsole(int);


/*
=============
MATH
=============
*/

void MScr_cos(int);
void MScr_sin(int);

/*
=============
PLAYER METHODS
=============
*/

void PlayerCmd_useButtonPressedX(int);
void PlayerCmd_setVelocity(int);
void PlayerCmd_getVelocity(int);
void PlayerCmd_getPlayerAngles(int);
void PlayerCmd_getIP(int);
void PlayerCmd_backButtonPressed(int);
void PlayerCmd_forwardButtonPressed(int);
void PlayerCmd_leftButtonPressed(int);
void PlayerCmd_rightButtonPressed(int);
void PlayerCmd_moveupButtonPressed(int);
void PlayerCmd_movedownButtonPressed(int);
void PlayerCmd_aimButtonPressed(int);
void PlayerCmd_reloadButtonPressed(int);

/*
=============
ENTITY METHODS
=============
*/

void EntCmd_setBounds(int);
void EntCmd_setTakeDamage(int);
void EntCmd_callback(int);
void EntCmd_nextthink(int);

/*
=============
FILE FUNCTIONS
=============
*/

void GScr_fopen(int);
void GScr_fclose(int);
void GScr_fread(int);
void GScr_fexists(int);
void GScr_fsize(int);
void GScr_fwrite(int);

/*
=============
MYSQL FUNCTIONS
=============
*/

#ifdef uMYSQL

void GScr_mysql_init(int);
void GScr_mysql_close(int);
void GScr_mysql_affected_rows(int);
void GScr_mysql_errno(int);
void GScr_mysql_error(int);
void GScr_mysql_fetch_field(int);
void GScr_mysql_field_seek(int);
void GScr_mysql_free_result(int);
void GScr_mysql_num_fields(int);
void GScr_mysql_num_rows(int);
void GScr_mysql_query(int);
void GScr_mysql_real_connect(int);
void GScr_mysql_real_escape_string(int);
void GScr_mysql_store_result(int);

#endif //uMYSQL

#endif // SCRIPT_H