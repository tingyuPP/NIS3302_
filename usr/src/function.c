#include "rule.h"
#include "function.h"
#include "utils.h"

// 添加规则
bool addRule(const Rule *rule)
{
    if (isRuleExist(rule)) {
        printf("\033[1;31m规则已存在！\033[0m\n");
        return false;
    }

    FILE *fp = fopen(RULE_FILE, "a");
    if (fp == NULL) {
        printf("\033[1;31m打开文件失败！\033[0m\n");
        return false;
    }
    
    char *action = rule->action ? "1" : "0";

    // 获取规则数量
    int ruleNumber = 0;
    // 读取文件中规则数量，其中每行一个规则，规则的开头为数字字符
    char line[100];
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (isdigit(line[0])) {
            ruleNumber++;
        }
    }

    //判断规则数量是否已达上限
    if (ruleNumber > MAX_RULES) {
        printf("\033[1;31m规则数量已达上限！\033[0m\n");
        fclose(fp);
        return false;
    }

    char *id;
    id = malloc(sizeof(char) * 10);
    // 遍历文件，找到最大的id
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (isdigit(line[0])) {
            id = strtok(line, ",");
        }
    }
    
    //新id为最大id加1
    char *newID;
    newID = malloc(sizeof(char) * (strlen(id) + 1));
    strcpy(newID, id);
    int maxID = atoi(newID);
    maxID++;
    sprintf(newID, "%d", maxID);

    // 写入规则
    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", newID, rule->protocol_type, rule->interface_type, rule->src_ip, rule->src_port, rule->dst_ip, rule->dst_port, rule->begin_time, rule->end_time, action);
    
    free(id);
    free(newID);

    printf("\033[1;32m添加规则成功！\033[0m\n");

    fclose(fp);
    return true;
}

// 删除规则
bool deleteRule(int ruleID)
{
    FILE *fp = fopen(RULE_FILE, "r");
    if (fp == NULL) {
        printf("\033[1;31m打开文件失败！\033[0m\n");
        return false;
    }

    FILE *tmp = fopen("/tmp/firewall.txt", "w");
    if (tmp == NULL) {
        printf("\033[1;31m打开文件失败！\033[0m\n");
        fclose(fp);
        return false;
    }

    char line[100];
    bool isDeleted = false;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (isdigit(line[0])) {
            int id = atoi(strtok(line, ","));
            if (id == ruleID) {
                isDeleted = true;
                continue;
            }
        }
        fprintf(tmp, "%s", line);
    }

    fclose(fp);
    fclose(tmp);

    if (!isDeleted) {
        printf("\033[1;31m规则不存在！\033[0m\n");
        remove("/tmp/firewall.txt");
        return false;
    }

    remove(RULE_FILE);
    rename("/tmp/firewall.txt", RULE_FILE);

    printf("\033[1;32m删除规则成功！\033[0m\n");

    return true;
}

// 修改规则
bool modifyRule(int ruleID, char *field, char *value)
{
    FILE *fp = fopen(RULE_FILE, "r");
    if (fp == NULL) {
        printf("\033[1;31m打开文件失败！\033[0m\n");
        return false;
    }

    FILE *tmp = fopen("/tmp/firewall.txt", "w");
    if (tmp == NULL) {
        printf("\033[1;31m打开文件失败！\033[0m\n");
        fclose(fp);
        return false;
    }

    char line[100];
    bool isModified = false;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (isdigit(line[0])) {
            int id = atoi(strtok(line, ","));
            if (id == ruleID) {
                isModified = true;
                char *id = strtok(line, ",");
                char *protocol_type = strtok(NULL, ",");
                char *interface_type = strtok(NULL, ",");
                char *src_ip = strtok(NULL, ",");
                char *src_port = strtok(NULL, ",");
                char *dst_ip = strtok(NULL, ",");
                char *dst_port = strtok(NULL, ",");
                char *begin_time = strtok(NULL, ",");
                char *end_time = strtok(NULL, ",");
                char *action = strtok(NULL, ",");
                if (action[strlen(action) - 1] == '\n') {
                    action[strlen(action) - 1] = '\0';
                }
                if (strcmp(field, "protocol_type") == 0) {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, value, interface_type, src_ip, src_port, dst_ip, dst_port, begin_time, end_time, action);
                } else if (strcmp(field, "interface_type") == 0) {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, value, src_ip, src_port, dst_ip, dst_port, begin_time, end_time, action);
                } else if (strcmp(field, "src_ip") == 0) {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, value, src_port, dst_ip, dst_port, begin_time, end_time, action);
                } else if (strcmp(field, "src_port") == 0) {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, src_ip, value, dst_ip, dst_port, begin_time, end_time, action);
                } else if (strcmp(field, "dst_ip") == 0) {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, src_ip, src_port, value, dst_port, begin_time, end_time, action);
                } else if (strcmp(field, "dst_port") == 0) {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, src_ip, src_port, dst_ip, value, begin_time, end_time, action);
                } else if (strcmp(field, "begin_time") == 0) {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, src_ip, src_port, dst_ip, dst_port, value, end_time, action);
                } else if (strcmp(field, "end_time") == 0) {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, src_ip, src_port, dst_ip, dst_port, begin_time, value, action);
                } else if (strcmp(field, "action") == 0) {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, src_ip, src_port, dst_ip, dst_port, begin_time, end_time, value);
                }
            } else {
                fprintf(tmp, "%s", line);
            }
        }
    }
    fclose(fp);
    fclose(tmp);
    if (!isModified) {
        printf("\033[1;31m规则不存在！\033[0m\n");
        remove("/tmp/firewall.txt");
        return false;
    }
    remove(RULE_FILE);
    rename("/tmp/firewall.txt", RULE_FILE);
    printf("\033[1;32m修改规则成功！\033[0m\n");
    return true;
}
