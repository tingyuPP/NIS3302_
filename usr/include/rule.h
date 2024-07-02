#ifndef RULE_H
#define RULE_H

#define _GNU_SOURCE  // 启用GNU扩展

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>
#include <fcntl.h>

#define MAX_RULES 30 //最大规则数量
#define DEV_FILE "/dev/controlinfo" //设备文件名
#define RULE_FILE "/etc/firewall.txt" //保存规则的文件名

typedef struct Rule {
   int id;//规则编号
    char *protocol_type;//协议类型
    char *interface_type;//网络接口类型
    char *src_ip;//源IP地址
    char *src_port;//源端口号
    char *dst_ip;//目的IP地址
    char *dst_port;//目的端口号
    char *begin_time;//开始时间
    char *end_time;//结束时间
    bool action;//执行动作，0代表拒绝通过，1代表允许通过
} Rule;

#endif