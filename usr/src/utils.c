#include "utils.h"
#include "rule.h"

Rule getRuleById(int ruleID) {
    FILE *fp = fopen(RULE_FILE, "r");
    if (!fp) {
        //perror("Failed to open rule file");
        return (Rule){0};  
    }

    Rule rule;
    memset(&rule, 0, sizeof(Rule));
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        char *tmp = strtok(line, ",");
        int id = atoi(tmp);
        if (id == ruleID) {
            rule.id = id;
            rule.protocol_type = strdup(strtok(NULL, ","));
            rule.interface_type = strdup(strtok(NULL, ","));
            rule.src_ip = strdup(strtok(NULL, ","));
            rule.src_port = strdup(strtok(NULL, ","));
            rule.dst_ip = strdup(strtok(NULL, ","));
            rule.dst_port = strdup(strtok(NULL, ","));
            rule.begin_time = strdup(strtok(NULL, ","));
            rule.end_time = strdup(strtok(NULL, ","));
            rule.action = atoi(strtok(NULL, ","));
            fclose(fp);
            return rule;
        }
    }
    fclose(fp);
    return (Rule){0};  
}

bool isRuleExist(const Rule *rule) {
    FILE *fp = fopen(RULE_FILE, "r");
    if (!fp) {
        perror("\033[1;31m打开规则文件失败！\033[0m\n");
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        // 去掉换行符
        line[strcspn(line, "\n")] = '\0';

        char *token;
        char *line_copy = strdup(line); // 复制一份 line，以免 strtok 修改原始数据
        if (!line_copy) {
            perror("\033[1;31m内存分配失败！\033[0m\n");
            fclose(fp);
            return false;
        }
        char *rest = line_copy;
	
	// 分割ID
        token = strtok_r(rest, ",", &rest);
     
        // 比较协议类型
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("\033[1;31m协议类型无效！\033[0m\n");
            free(line_copy);
            continue;
        }
        if (rule->protocol_type == NULL || strcmp(rule->protocol_type, token) != 0) {
            free(line_copy);
            continue;
        }

        // 比较网络接口类型
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("\033[1;31m网络接口类型无效！\033[0m\n");
            free(line_copy);
            continue;
        }
        if (rule->interface_type == NULL || strcmp(rule->interface_type, token) != 0) {
            free(line_copy);
            continue;
        }

        // 比较源IP地址
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("\033[1;31m源IP地址无效！\033[0m\n");
            free(line_copy);
            continue;
        }
        if (rule->src_ip == NULL || strcmp(rule->src_ip, token) != 0) {
            free(line_copy);
            continue;
        }

        // 比较源端口号
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("\033[1;31m源端口号无效！\033[0m\n");
            free(line_copy);
            continue;
        }
        if (rule->src_port == NULL || strcmp(rule->src_port, token) != 0) {
            free(line_copy);
            continue;
        }

        // 比较目的IP地址
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("\033[1;31m目的IP地址无效！\033[0m\n");
            free(line_copy);
            continue;
        }
        if (rule->dst_ip == NULL || strcmp(rule->dst_ip, token) != 0) {
            free(line_copy);
            continue;
        }

        // 比较目的端口号
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("\033[1;31m目的端口号无效！\033[0m\n");
            free(line_copy);
            continue;
        }
        if (rule->dst_port == NULL || strcmp(rule->dst_port, token) != 0) {
            free(line_copy);
            continue;
        }

        // 比较开始时间
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("\033[1;31m开始时间无效！\033[0m\n");
            free(line_copy);
            continue;
        }
        if (rule->begin_time == NULL || strcmp(rule->begin_time, token) != 0) {
            free(line_copy);
            continue;
        }

        // 比较结束时间
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("\033[1;31m结束时间无效！\033[0m\n");
            free(line_copy);
            continue;
        }
        if (rule->end_time == NULL || strcmp(rule->end_time, token) != 0) {
            free(line_copy);
            continue;
        }

        // 比较执行动作
        token = strtok_r(rest, ",", &rest);
        if (!token) {
            printf("\033[1;31m执行动作无效！\033[0m\n");
            free(line_copy);
            continue;
        }
        bool action = atoi(token);
        if (rule->action != action) {
            free(line_copy);
            continue;
        }

        free(line_copy);
        fclose(fp);
        return true;
    }
    fclose(fp);
    return false;
}

bool isRuleEmpty(const Rule *rule) {
    return rule->protocol_type == NULL &&
           rule->interface_type == NULL &&
           rule->src_ip == NULL &&
           rule->dst_ip == NULL &&
           rule->src_port == NULL &&
           rule->dst_port == NULL &&
           rule->begin_time == NULL &&
           rule->end_time == NULL &&
           rule->action == 0;
}

bool isValidProtocolType(const char* protocolType) {
    const char* valid_protocols[] = {"tcp", "udp", "icmp", "$"};
    for (int i = 0; valid_protocols[i] != NULL; i++) {
        if (strcmp(protocolType, valid_protocols[i]) == 0) {
            return true;
        }
    }
    //printf("\033[1;31m协议类型无效！\033[0m\n");
    return false;
}

bool isValidInterfaceType(const char *interfaceType) {
    if (strcmp(interfaceType, "$") == 0) {
        return true;  
    }
    const char *valid_interfaces[] = {"eth0", "eth1", "wlan0","ens33", "$"};
    for (int i = 0; valid_interfaces[i]; i++) {
        if (strcmp(interfaceType, valid_interfaces[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool isValidIP(const char* ip) {
    if (strcmp(ip, "$") == 0) {
        return true;
    }

    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
    if (result == 0) {
        //printf("\033[1;31mIP地址无效！\033[0m\n");
    }
    return result != 0;
}

bool isValidPort(const char* port) {
    if (strcmp(port, "$") == 0) {
        return true;
    }

    char* endPtr;
    long portNum = strtol(port, &endPtr, 10);

    // 检查是否解析到字符串末尾并且字符串末尾没有多余的非数字字符
    while (*endPtr) {
        if (!isspace((unsigned char)*endPtr)) {
            //printf("\033[1;31m端口号无效，必须是数字且在0到65535之间！\033[0m\n");
            return false;
        }
        endPtr++;
    }

    if (portNum < 0 || portNum > 65535) {
        //printf("\033[1;31m端口号无效，必须在0到65535之间！\033[0m\n");
        return false;
    }
    return true;
}

bool isValidTime(const char* time) {
    if (strcmp(time, "$") == 0) {
        return true;
    }

    // 临时存储解析后的时间各部分
    int year, month, day, hour, minute, second;
    char extra;
    
    // 检查格式是否完全符合 YYYY-MM-DD HH:MM:SS
    if (sscanf(time, "%4d-%2d-%2d %2d:%2d:%2d%c", &year, &month, &day, &hour, &minute, &second, &extra) != 6) {
        printf("\033[1;31m日期时间格式无效！应为YYYY-MM-DD HH:MM:SS\033[0m\n");
        return false;
    }

    // 检查年、月、日、时、分、秒是否在有效范围内
    if (year < 0 || month < 1 || month > 12 || day < 1 || hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        printf("\033[1;31m日期或时间值超出有效范围！\033[0m\n");
        return false;
    }

    // 检查月份的天数是否合法
    int maxDays = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        maxDays = 30;
    } else if (month == 2) {
        bool isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        maxDays = isLeapYear ? 29 : 28;
    }

    if (day > maxDays) {
        printf("\033[1;31m无效的日期，%d月没有%d天！\033[0m\n", month, day);
        return false;
    }

    return true;
}


bool isBeginTimeBeforeEndTime(const char* beginTime, const char* endTime) {
    if (strcmp(beginTime, "$") == 0 || strcmp(endTime, "$") == 0) {
        return true;  // 如果任一时间为空，假定无需比较
    }

    struct tm tmStart, tmEnd;
    memset(&tmStart, 0, sizeof(struct tm));
    memset(&tmEnd, 0, sizeof(struct tm));

    if (!strptime(beginTime, "%Y-%m-%d %H:%M:%S", &tmStart) ||
        !strptime(endTime, "%Y-%m-%d %H:%M:%S", &tmEnd)) {
        printf("\033[1;31m日期时间格式错误\033[0m\n");
        return false;
    }

    double diff = difftime(mktime(&tmEnd), mktime(&tmStart));
    if (diff <= 0) {
        printf("\033[1;31m结束时间必须晚于开始时间！\033[0m\n");
        return false;
    }
    return true;
}
