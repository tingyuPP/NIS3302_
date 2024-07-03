#include "configure.h"
#include "function.h"
#include "rule.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handleAdd(int argc, char *argv[]);
void handleDelete(int argc, char *argv[]);
void handleModify(int argc, char *argv[]);
void handleList(int argc, char *argv[]);
void handleSave(int argc, char *argv[]);
void handleRead(int argc, char *argv[]);
void handleWrite(int argc, char *argv[]);
void handleHelp(int argc, char *argv[]);

typedef void (*CommandHandler)(int, char *[]);

struct Command
{
    const char *command;
    const char *shortCommand;
    CommandHandler handler;
};

struct Command commands[] = {
    {"--add", "-a", handleAdd},
    {"--delete", "-d", handleDelete},
    {"--modify", "-m", handleModify},
    {"--list", "-l", handleList},
    {"--save", "-s", handleSave},
    {"--read", "-r", handleRead},
    {"--write", "-w", handleWrite},
    {"--help", "-h", handleHelp},
    {NULL, NULL, NULL}};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        displayUsage();
        return 1;
    }

    for (struct Command *cmd = commands; cmd->command != NULL; ++cmd)
    {
        if (strcmp(argv[1], cmd->command) == 0 || strcmp(argv[1], cmd->shortCommand) == 0)
        {
            cmd->handler(argc, argv);
            return 0;
        }
    }

    displayUsage();
    return 1;
}

void handleAdd(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("缺少参数\n");
        displayUsage();
        return;
    }
    Rule rule = parseRule(argc - 2, argv + 2);
    if (addRule(&rule))
    {
        printf("规则添加成功\n");
    }
    else
    {
        printf("规则添加失败\n");
    }
}

void handleDelete(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("缺少参数\n");
        displayUsage();
        return;
    }
    int ruleID = atoi(argv[2]);
    Rule rule = getRuleById(ruleID);
    if (!isRuleExist(&rule))
    {
        printf("规则不存在\n");
    }
    else if (deleteRule(ruleID))
    {
        printf("规则删除成功\n");
    }
    else
    {
        printf("规则删除失败\n");
    }
}

void handleModify(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("缺少参数\n");
        displayUsage();
        return;
    }
    int ruleID = atoi(argv[2]);
    char *mdf_para = argv[3];
    char *mdf_value = argv[4];

    Rule rule = getRuleById(ruleID);
    if (!isRuleExist(&rule))
    {
        printf("规则不存在\n");
        return;
    }

    if (strcmp(mdf_para, "ptc") == 0 && isValidProtocolType(mdf_value)) //modify the protocol_type
    {
        strcpy(rule.protocol_type, mdf_value);
    }
    else if (strcmp(mdf_para, "itf") == 0 && isValidInterfaceType(mdf_value)) //modify the interface_type
    {
        strcpy(rule.interface_type, mdf_value);
    }
    else if (strcmp(mdf_para, "sip") == 0 && (isValidIP(mdf_value) || isValidIP(removeQuotes(mdf_value)))) //modify the source_ip
    {
        strcpy(rule.src_ip, mdf_value);
    }
    else if (strcmp(mdf_para, "spt") == 0 && (isValidPort(mdf_value) || isValidPort(removeQuotes(mdf_value)))) //modify the source_port
    {
        strcpy(rule.src_port, mdf_value);
    }
    else if (strcmp(mdf_para, "dip") == 0 && (isValidIP(mdf_value) || isValidIP(removeQuotes(mdf_value)))) //modify the destination_ip
    {
        strcpy(rule.dst_ip, mdf_value);
    }
    else if (strcmp(mdf_para, "dpt") == 0 && (isValidPort(mdf_value) || isValidPort(removeQuotes(mdf_value)))) //modify the deatination_port
    {
        strcpy(rule.dst_port, mdf_value);
    }
    else if (strcmp(mdf_para, "btm") == 0 && isValidTime(removeQuotes(mdf_value)) && isBeginTimeBeforeEndTime(removeQuotes(mdf_value), rule.end_time)) //modify the begin_time
    {
        strcpy(rule.begin_time, removeQuotes(mdf_value));
    }
    else if (strcmp(mdf_para, "etm") == 0 && isValidTime(removeQuotes(mdf_value)) && isBeginTimeBeforeEndTime(rule.begin_time, removeQuotes(mdf_value))) //modify the end_time
    {
        strcpy(rule.end_time, removeQuotes(mdf_value));
    }
    else if (strcmp(mdf_para, "act") == 0 && (atoi(mdf_value) == 0 || atoi(mdf_value) == 1)) //modify the action
    {
        rule.action = atoi(mdf_value);
    }
    else
    {
        printf("无效的规则字段或值\n");
        return;
    }

    if (modifyRule(ruleID, mdf_para, mdf_value))
    {
        printf("规则修改成功\n");
    }
    else
    {
        printf("规则修改失败\n");
    }
}

void handleList(int argc, char *argv[])
{
    displayRules();
}

void handleSave(int argc, char *argv[])
{
    const char *filename = RULE_FILE;
    saveRulesToFile(filename);
    printf("规则保存到文件成功\n");
}

void handleRead(int argc, char *argv[])
{
    const char *filename = RULE_FILE;
    readRulesFromFile(filename);
    printf("从文件读取规则成功\n");
}

void handleWrite(int argc, char *argv[])
{
    if (writeRulesToDevice())
    {
        printf("规则写入设备成功\n");
    }
    else
    {
        printf("规则写入设备失败\n");
    }
}

void handleHelp(int argc, char *argv[])
{
    displayUsage();
}
