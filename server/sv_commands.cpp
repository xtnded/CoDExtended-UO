#include "server.hpp"

void Cmd_Version() {
    printf("Call of Duty Extended by riicchhaarrd\nA lot of thanks to kungfooman and libcod\n");
}

void Cmd_HelloWorld() {
    printf("Hello world!\n");
}

void Com_sprintf(char *dest, int size, const char *fmt, ...) {
    int ret;
    va_list argptr;

    va_start(argptr,fmt);
    ret = vsnprintf(dest, size, fmt, argptr); //Q_vsnprintf
    va_end(argptr);
    if (ret == -1) {
        printf("Com_sprintf: overflow of %i bytes buffer\n", size);
    }
}

void Cmd_XSay() {
    if(Cmd_Argc() != 2) {
        printf("Usage: xsay <message>\n");
    }

    char *msg = Cmd_Argv(1);
    if(msg == "") {
        printf("Invalid message.\n");
        return;
    }

    char buf[1024];
    Com_sprintf(buf, 1024, "i \"%s\"", msg);

    SV_SendServerCommand(NULL, 1, buf);
}

void SV_AddOperatorCommands(void) {
    Cmd_AddCommand("codextended", Cmd_Version);
    Cmd_AddCommand("helloworld", Cmd_HelloWorld);
    Cmd_AddCommand("xsay", Cmd_XSay);
}