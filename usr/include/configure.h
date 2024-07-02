#ifndef CONFIGURE_H
#define CONFIGURE_H

#include "rule.h"

//使用说明
void displayUsage();

//解析规则参数
Rule parseRule(int argc, char *argv[]);