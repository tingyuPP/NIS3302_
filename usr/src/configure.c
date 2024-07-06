#include "configure.h"
#include "function.h"
#include "rule.h"
#include "utils.h"

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
*/

// 使用说明
void displayUsage()
{
    printf("\n");
    printf("Usage: firewall [options]\n");
    printf("[Options]:\n");

    printf("  -a, --add            Add a new rule,input with another 9 parameters--ptc,itf,sip,spt,dip,dpt,btm,etm,act(default:'\')\n");
    printf("  -d, --delete         Delete a rule by ID,input with another parameters--id\n");
    printf("  -m, --modify         Modify a rule by ID,input with another 3 parameters--id,mdf_para,mdf_value\n");
    printf("  -l, --list           List all rules\n");
    printf("  -s, --save           Save rules to file,input with another parameters--file_name\n");
    printf("  -r, --read           Read rules from file,input with another parameters--file_name\n");
    printf("  -w, --write          Write rules to device\n");
    printf("  -h, --help           Display this help message\n");
    printf("\n");
}


//处理时间格式中为避免断句而加上的引号
char* removeQuotes(char* str) 
{
    int len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"') {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
    return str;
}


// 解析规则参数
Rule parseRule(int argc, char *argv[])
{
    Rule rule;
    memset(&rule, 0, sizeof(Rule));

    // 为字符指针成员分配内存
    rule.protocol_type = malloc(strlen(argv[0]) + 1);
    rule.interface_type = malloc(strlen(argv[1]) + 1);
    rule.src_ip = malloc(strlen(argv[2]) + 1);
    rule.src_port = malloc(strlen(argv[3]) + 1);
    rule.dst_ip = malloc(strlen(argv[4]) + 1);
    rule.dst_port = malloc(strlen(argv[5]) + 1);
    rule.begin_time = malloc(strlen(argv[6]) + 1);
    rule.end_time = malloc(strlen(argv[7]) + 1);

    // 复制字符串到字符指针成员
    if (!isValidProtocolType(argv[0]))
    {
        printf("\033[1;31m协议类型无效！\033[0m\n");
        exit(0);
    }
    else
    {
        strcpy(rule.protocol_type, argv[0]);
    }

    if (argv[1][0] == '\0')
    {
        strcpy(rule.interface_type, "");
    }
    else
    {
        strcpy(rule.interface_type, argv[1]);
    }

    if (!isValidIP(argv[2]) && !isValidIP(removeQuotes(argv[2])))
    {
        printf("\033[1;31m源IP地址无效！\033[0m\n");
        exit(0);
    }
    else
    {
        if (argv[2][0] == '\0')
        {
            strcpy(rule.src_ip, "");
        }
        else
        {
            strcpy(rule.src_ip, argv[2]);
        }
    }

    if (!isValidPort(argv[4]) && !isValidPort(removeQuotes(argv[3])))
    {
        printf("\033[1;31m源端口号无效，必须是数字且在0到65535之间！\033[0m\n");
        exit(0);
    }
    else
    {
        if (argv[3][0] == '\0')
        {
            strcpy(rule.src_port, "");
        }
        else
        {
            strcpy(rule.src_port, argv[3]);
        }
    }

    if (!isValidIP(argv[3]) && !isValidIP(removeQuotes(argv[4])))
    {
        printf("\033[1;31m目标IP地址无效！\033[0m\n");
        exit(0);
    }
    else
    {
        if (argv[3][0] == '\0')
        {
            strcpy(rule.dst_ip, "");
        }
        else
        {
            strcpy(rule.dst_ip, argv[4]);
        }
    }



    if (!isValidPort(argv[5]) && !isValidPort(removeQuotes(argv[5])))
    {
        printf("\033[1;31m目标端口号无效，必须是数字且在0到65535之间！\033[0m\n");
        exit(0);
    }
    else
    {
        if (argv[5][0] == '\0')
        {
            strcpy(rule.dst_port, "");
        }
        else
        {
            strcpy(rule.dst_port, argv[5]);
        }
    }

    if (!isValidTime(removeQuotes(argv[6])))
    {
        //printf("输入的开始时间无效\n");
        exit(0);
    }
    else if (!isValidTime(removeQuotes(argv[7])))
    {
        //printf("输入的结束时间无效\n");
        exit(0);
    }
    else if (!isBeginTimeBeforeEndTime(removeQuotes(argv[6]), removeQuotes(argv[7])))
    {
        //printf("时间无效，结束时间不晚于开始时间。\n");
        exit(0);
    }
    else
    {
        strcpy(rule.begin_time, removeQuotes(argv[6]));
        strcpy(rule.end_time, removeQuotes(argv[7]));
    }

    if (atoi(argv[8]) == 0 || atoi(argv[8]) == 1)
    {
        rule.action = atoi(argv[8]);
    }
    else
    {
        printf("\033[1;31m输入的动作无效！\033[0m\n");
        exit(0);
    }

    return rule;
}
