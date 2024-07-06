#include "rule.h"
#include "function.h"
#include "utils.h"

// 添加规则
bool addRule(const Rule *rule)
{
    
    if (isRuleExist(rule))
    {
        printf("\033[1;31m规则已存在！\033[0m\n");
        return false;
    }

    FILE *fp = fopen(RULE_FILE, "a+");
    if (fp == NULL)
    {
        printf("\033[1;31m打开规则文件失败！\033[0m\n");
        return false;
    }

    char *action = rule->action ? "1" : "0";

    rewind(fp);

    // 获取规则数量
    int ruleNumber = 0;
    // 读取文件中规则数量，其中每行一个规则，规则的开头为数字字符
    char line[100];
    while (fgets(line, sizeof(line), fp) != NULL)
    {

        if (isdigit(line[0]))
        {
            ruleNumber++;
        }
    }

    // 判断规则数量是否已达上限
    if (ruleNumber > MAX_RULES)
    {
        printf("\033[1;31m规则数量已达上限！\033[0m\n");
        fclose(fp);
        return false;
    }

    rewind(fp);

    char *id = NULL;
    int maxID = 0;
    // 遍历文件，找到最大的id
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (isdigit(line[0]))
        {
            char *tempId = strtok(line, ",");
            if (tempId != NULL)
            {
                int currentID = atoi(tempId);
                if (currentID > maxID)
                {
                    maxID = currentID;
                }
            }
        }
    }
    maxID++;
    char *newID = malloc(sizeof(char) * 10);
    sprintf(newID, "%d", maxID);
    // 写入规则
    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", newID, rule->protocol_type, rule->interface_type, rule->src_ip, rule->src_port, rule->dst_ip, rule->dst_port, rule->begin_time, rule->end_time, action);

    free(newID);

    // printf("\033[1;32m添加规则成功！\033[0m\n");

    fclose(fp);
    return true;
}

// 删除规则
bool deleteRule(int ruleID)
{
    FILE *fp = fopen(RULE_FILE, "r");
    if (fp == NULL)
    {
        printf("\033[1;31m打开规则文件失败！\033[0m\n");
        return false;
    }

    FILE *tmp = fopen("/tmp/firewall.txt", "w");
    if (tmp == NULL)
    {
        printf("\033[1;31m打开临时文件失败！\033[0m\n");
        fclose(fp);
        return false;
    }

    char line[100];
    bool isDeleted = false;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *tmpline = strdup(line);
        if (isdigit(line[0]))
        {
            int id = atoi(strtok(line, ","));
            if (id == ruleID)
            {
                isDeleted = true;
                continue;
            }
        }
        fprintf(tmp, "%s", tmpline);
    }

    fclose(fp);
    fclose(tmp);

    if (!isDeleted)
    {
        // printf("\033[1;31m规则不存在！\033[0m\n");
        remove("/tmp/firewall.txt");
        return false;
    }

    remove(RULE_FILE);
    rename("/tmp/firewall.txt", RULE_FILE);

    // printf("\033[1;32m删除规则成功！\033[0m\n");

    return true;
}

// 修改规则
bool modifyRule(int ruleID, char *field, char *value)
{
    FILE *fp = fopen(RULE_FILE, "r");
    if (fp == NULL)
    {
        printf("\033[1;31m打开规则文件失败！\033[0m\n");
        return false;
    }

    FILE *tmp = fopen("/tmp/firewall.txt", "w");
    if (tmp == NULL)
    {
        printf("\033[1;31m打开临时文件失败！\033[0m\n");
        fclose(fp);
        return false;
    }

    char line[100];
    bool isModified = false;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *tmpline = strdup(line);
        char *tmpline1 = strdup(line);
        if (isdigit(line[0]))
        {
            int id = atoi(strtok(line, ","));
            if (id == ruleID)
            {
                isModified = true;
                // 解析单行规则
                char *id = strtok(tmpline1, ",");
                char *protocol_type = strtok(NULL, ",");
                char *interface_type = strtok(NULL, ",");
                char *src_ip = strtok(NULL, ",");
                char *src_port = strtok(NULL, ",");
                char *dst_ip = strtok(NULL, ",");
                char *dst_port = strtok(NULL, ",");
                char *begin_time = strtok(NULL, ",");
                char *end_time = strtok(NULL, ",");
                char *action = strtok(NULL, ",");
                if (action != NULL && strlen(action) > 0)
                {
                    if (action[strlen(action) - 1] == '\n')
                    {
                        action[strlen(action) - 1] = '\0';
                    }
                }
                // 对规则做修改并写入新文件
                if (strcmp(field, "ptc") == 0)
                {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, value, interface_type, src_ip, src_port, dst_ip, dst_port, begin_time, end_time, action);
                }
                else if (strcmp(field, "itf") == 0)
                {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, value, src_ip, src_port, dst_ip, dst_port, begin_time, end_time, action);
                }
                else if (strcmp(field, "sip") == 0)
                {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, value, src_port, dst_ip, dst_port, begin_time, end_time, action);
                }
                else if (strcmp(field, "spt") == 0)
                {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, src_ip, value, dst_ip, dst_port, begin_time, end_time, action);
                }
                else if (strcmp(field, "dip") == 0)
                {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, src_ip, src_port, value, dst_port, begin_time, end_time, action);
                }
                else if (strcmp(field, "dpt") == 0)
                {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, src_ip, src_port, dst_ip, value, begin_time, end_time, action);
                }
                else if (strcmp(field, "btm") == 0)
                {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, src_ip, src_port, dst_ip, dst_port, value, end_time, action);
                }
                else if (strcmp(field, "etm") == 0)
                {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, src_ip, src_port, dst_ip, dst_port, begin_time, value, action);
                }
                else if (strcmp(field, "act") == 0)
                {
                    fprintf(tmp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, protocol_type, interface_type, src_ip, src_port, dst_ip, dst_port, begin_time, end_time, value);
                }
            }
            else
            {
                fprintf(tmp, "%s", tmpline);
            }
        }
    }
    fclose(fp);
    fclose(tmp);
    if (!isModified)
    {
        // printf("\033[1;31m规则不存在！\033[0m\n");
        remove("/tmp/firewall.txt");
        return false;
    }
    remove(RULE_FILE);
    rename("/tmp/firewall.txt", RULE_FILE);
    // printf("\033[1;32m修改规则成功！\033[0m\n");
    return true;
}

// 导出规则到文件
void saveRulesToFile(const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("\033[1;31m打开待存入文件失败！\033[0m\n");
        return;
    }

    FILE *ruleFile = fopen(RULE_FILE, "r");
    if (ruleFile == NULL)
    {
        printf("\033[1;31m打开规则文件失败！\033[0m\n");
        fclose(fp);
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), ruleFile) != NULL)
    {
        fprintf(fp, "%s", line);
    }

    fclose(fp);
    fclose(ruleFile);

    printf("\033[1;32m导出规则成功！\033[0m\n");
}

// 从文件中读取规则
void readRulesFromFile(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("\033[1;31m打开待读取文件失败！\033[0m\n");
        return;
    }

    bool import = true;
    bool isEmpty = true;
    char line[100];
    int IndexOfRules = 1;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        isEmpty = false;
        Rule *rule = malloc(sizeof(Rule));
        rule->protocol_type = malloc(sizeof(char) * 100);
        rule->interface_type = malloc(sizeof(char) * 100);
        rule->src_ip = malloc(sizeof(char) * 100);
        rule->src_port = malloc(sizeof(char) * 100);
        rule->dst_ip = malloc(sizeof(char) * 100);
        rule->dst_port = malloc(sizeof(char) * 100);
        rule->begin_time = malloc(sizeof(char) * 100);
        rule->end_time = malloc(sizeof(char) * 100);

        char *id = strtok(line, ",");
        rule->id = atoi(id);
        strcpy(rule->protocol_type, strtok(NULL, ","));
        strcpy(rule->interface_type, strtok(NULL, ","));
        strcpy(rule->src_ip, strtok(NULL, ","));
        strcpy(rule->src_port, strtok(NULL, ","));
        strcpy(rule->dst_ip, strtok(NULL, ","));
        strcpy(rule->dst_port, strtok(NULL, ","));
        strcpy(rule->begin_time, strtok(NULL, ","));
        strcpy(rule->end_time, strtok(NULL, ","));
        char *action = strtok(NULL, ",");
        if (action != NULL && strlen(action) > 0)
        {
            if (action[strlen(action) - 1] == '\n')
            {
                action[strlen(action) - 1] = '\0';
            }
        }
        rule->action = strcmp(action, "1") == 0 ? true : false;

        if (!isValidProtocolType(rule->protocol_type) || !isValidInterfaceType(rule->interface_type) || !isValidIP(rule->src_ip) || !isValidPort(rule->src_port) || !isValidIP(rule->dst_ip) || !isValidPort(rule->dst_port) || !isValidTime(rule->begin_time) || !isValidTime(rule->end_time) || !isBeginTimeBeforeEndTime(rule->begin_time, rule->end_time))
        {
            import = false;
            printf("\033[1;31m第%d条规则不合法！\033[0m\n", IndexOfRules);
        }
        else
        {
            bool addResult = addRule(rule);
            if (!addResult)
            {
                import = false;
                printf("\033[1;31m第%d条规则添加失败！\033[0m\n", IndexOfRules);
            }
            else 
            {
                printf("\033[1;32m第%d条规则添加成功！\033[0m\n", IndexOfRules);
            }
        }

        free(rule->protocol_type);
        free(rule->interface_type);
        free(rule->src_ip);
        free(rule->src_port);
        free(rule->dst_ip);
        free(rule->dst_port);
        free(rule->begin_time);
        free(rule->end_time);
        free(rule);

        IndexOfRules++;
    }

    if (import)
    {
        if (isEmpty)
        {
            printf("\033[1;31m待导入文件为空！\033[0m\n");
        }
        else
        {
            printf("\033[1;32m导入规则成功！\033[0m\n");
        }
    }
    else
    {
        printf("\033[1;31m导入规则失败！\033[0m\n");
    }

    fclose(fp);
}

// 将规则写入设备文件
bool writeRulesToDevice()
{
    // 查看规则
    FILE *fp = fopen(RULE_FILE, "r");
    if (fp == NULL)
    {
        printf("\033[1;31m打开规则文件失败！\033[0m\n");
        return false;
    }

    // 打开设备文件
    int fd = open(DEV_FILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        printf("\033[1;31m打开设备文件失败！\033[0m\n");
        return false;
    }

    char bf[9999] = "";
    size_t bfLength = 0;

    // 读取规则文件
    char line[100];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
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
        if (action != NULL && strlen(action) > 0)
        {
            if (action[strlen(action) - 1] == '\n')
            {
                action[strlen(action) - 1] = '\0';
            }
        }

        if (strcmp(action, "1") == 0)
        {
            continue;
        }
        /*
        interface_type = (strcmp(interface_type, "") == 0) ? "$" : interface_type;
        src_ip = (strcmp(src_ip, "") == 0) ? "$" : src_ip;
        src_port = (strcmp(src_port, "") == 0) ? "$" : src_port;
        dst_ip = (strcmp(dst_ip, "") == 0) ? "$" : dst_ip;
        dst_port = (strcmp(dst_port, "") == 0) ? "$" : dst_port;
        begin_time = (strcmp(begin_time, "") == 0) ? "$" : begin_time;
        end_time = (strcmp(end_time, "") == 0) ? "$" : end_time;
        */

        // 将规则字段以空格间隔写入缓冲字段中,这里规则之间以分号间隔。
        int writtenLength = sprintf(bf + bfLength, "%s %s %s %s %s %s %s %s\n;", protocol_type, interface_type, src_ip, src_port, dst_ip, dst_port, begin_time, end_time);

        if (writtenLength < 0 || writtenLength >= (int)(sizeof(bf) - bfLength))
        {
            printf("\033[1;31m规则数据太长！缓冲区溢出！\033[0m\n");
            close(fd);
            fclose(fp);
            return false;
        }

        bfLength += writtenLength;
    }

    // 每次写入规则后，将缓冲区的内容写入设备文件
    ssize_t writtenBytes = write(fd, bf, bfLength);

    if (bfLength == 0)
    {
        write(fd, bf, bfLength + 1);
    }

    if (writtenBytes < 0)
    {
        printf("\033[1;31m写入设备文件失败！\033[0m\n");
        close(fd);
        fclose(fp);
        return false;
    }

    close(fd);
    fclose(fp);
    return true;
}

// 显示当前规则
void displayRules()
{
    FILE *fp = fopen(RULE_FILE, "r");
    if (fp == NULL)
    {
        printf("\033[1;31m打开文件失败或未找到规则文件！\033[0m\n");
        return;
    }

    // 打印表头
    printf("%-10s %-20s %-20s %-20s %-20s %-20s %-20s %-20s %-20s %-10s\n", "ID", "Protocol Type", "Interface Type", "Source IP", "Source Port", "Destination IP", "Destination Port", "Begin Time", "End Time", "Action");

    char line[100];
    // 读取规则文件并打印规则
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (isdigit(line[0]))
        {
            int id = atoi(strtok(line, ","));
            char *protocol_type = strtok(NULL, ",");
            char *interface_type = strtok(NULL, ",");
            char *src_ip = strtok(NULL, ",");
            char *src_port = strtok(NULL, ",");
            char *dst_ip = strtok(NULL, ",");
            char *dst_port = strtok(NULL, ",");
            char *begin_time = strtok(NULL, ",");
            char *end_time = strtok(NULL, ",");
            char *action = strtok(NULL, ",");

            printf("%-10d %-20s %-20s %-20s %-20s %-20s %-20s %-20s %-20s %-10s\n", id, protocol_type, interface_type, src_ip, src_port, dst_ip, dst_port, begin_time, end_time, action);
        }
    }

    fclose(fp);
}
