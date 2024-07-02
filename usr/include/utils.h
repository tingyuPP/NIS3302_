#ifndef UTILS_H
#define UTILS_H

#include "rule.h"

// 根据id来查找规则
Rule getRuleById(int ruleID);

// 判断规则是否存在
bool isRuleExist(const Rule *rule);

// 判断规则是否为空
bool isRuleEmpty(const Rule *rule);

// 判断协议类型是否有效
bool isValidProtocolType(const char *protocolType);

// 判断网络接口类型是否有效
bool isValidInterfaceType(const char *interfaceType);

// 判断IP地址是否有效
bool isValidIP(const char *ip);

// 判断端口号是否有效
bool isValidPort(const char *port);

// 判断时间是否有效
bool isValidTime(const char *time);

// 判断开始时间是否早于结束时间
bool isBeginTimeBeforeEndTime(const char *beginTime, const char *endTime);

#endif







