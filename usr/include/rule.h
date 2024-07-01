#ifndef RULE_H
#define RULE_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>
#include <fcntl.h>

#define MAX_RULES 30
#define DEV_FILE "/dev/controlinfo"

typedef struct Rule {
   int id;
    char *protocol_type;
    char *interface_type;
    char *src_ip;
    char *src_port;
    char *dst_ip;
    char *dst_port;
    char *begin_time;
    char *end_time;
    bool action;
} Rule;

#endif