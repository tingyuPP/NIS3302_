#ifndef CONFIGURE_H
#define CONFIGURE_H

#include "rule.h"

//使用说明
void displayUsage();


//解析规则参数
Rule parseRule(int argc, char *argv[]);

//处理时间格式中为避免断句而加上的引号
char* removeQuotes(char* str);
