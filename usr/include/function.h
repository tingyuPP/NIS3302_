#ifndef FUNCTION_H
#define FUNCTION_H

#include "rule.h"

// 添加规则
bool addRule(const Rule *rule);

// 删除规则
bool deleteRule(int ruleID);

// 修改规则
bool modifyRule(int ruleID, char *field, char *value);

// 显示当前规则
void displayRules();

// 保存规则到文件
void saveRulesToFile(const char *filename);

// 从文件中读取规则
void readRulesFromFile(const char *filename);

//将规则写入设备文件
bool writeRulesToDevice();

#endif