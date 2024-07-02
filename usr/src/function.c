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
    asprintf(&id, "%d", ruleNumber + 1);

    // 写入规则
    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", id, rule->protocol_type, rule->interface_type, rule->src_ip, rule->src_port, rule->dst_ip, rule->dst_port, rule->begin_time, rule->end_time, action);
    
    printf("\033[1;32m添加规则成功！\033[0m\n");

    fclose(fp);
}