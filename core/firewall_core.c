#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/if_ether.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/tcp.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/udp.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <linux/skbuff.h>

#define MAX_RULES_NUM 30                 // 最大规则数
#define LOG_FILE "/var/log/firewall.log" // 日志文件路径

struct Rule
{
    int id;               // 规则编号
    char *protocol_type;  // 协议类型
    char *interface_type; // 网络接口类型
    char *src_ip;         // 源IP地址
    char *src_port;       // 源端口号
    char *dst_ip;         // 目的IP地址
    char *dst_port;       // 目的端口号
    char *begin_time;     // 开始时间
    char *end_time;       // 结束时间
};

static struct Rule rules[MAX_RULES_NUM]; // 规则数组
static int rules_num = 0;                // 规则数量

// 检查时间是否在规则时间范围内，字符串格式为"YYYY-MM-DD HH:MM:SS"
bool check_time(const char *cur_time, const char *begin_time, const char *end_time)
{
    // 如果对begin_time无要求
    if (strcmp(begin_time, "*") == 0)
    {
        if (strcmp(cur_time, end_time) <= 0)
        {
            return true;
        }
        return false;
    }
    // 如果对end_time无要求
    if (strcmp(end_time, "*") == 0)
    {
        if (strcmp(cur_time, begin_time) >= 0)
        {
            return true;
        }
        return false;
    }

    if (strcmp(cur_time, begin_time) >= 0 && strcmp(cur_time, end_time) <= 0)
    {
        return true;
    }
    return false;
}

// 检查源ip和目的ip是否在规则范围内
bool check_ip(const struct sk_buff *skb, const struct Rule *rule)
{
    struct iphdr *iph = ip_hdr(skb); // 获取ip头部
    // 转换为主机字节序, 并获取源ip和目的ip
    __be32 src_ip = ntohl(iph->saddr);
    __be32 dst_ip = ntohl(iph->daddr);
    // 转换为字符串
    char src_ip_str[16]; // 16位是因为最大的IP地址长度是15位
    char dst_ip_str[16];
    sprintf(src_ip_str, "%u.%u.%u.%u", src_ip & 0xff, (src_ip >> 8) & 0xff, (src_ip >> 16) & 0xff, (src_ip >> 24) & 0xff); // 将IP地址转换为字符串
    sprintf(dst_ip_str, "%u.%u.%u.%u", dst_ip & 0xff, (dst_ip >> 8) & 0xff, (dst_ip >> 16) & 0xff, (dst_ip >> 24) & 0xff);
    // 检查源ip
    if (strcmp(rule->src_ip, "*") != 0 && strcmp(rule->src_ip, src_ip_str) != 0)
    {
        return false;
    }
    // 检查目的ip
    if (strcmp(rule->dst_ip, "*") != 0 && strcmp(rule->dst_ip, dst_ip_str) != 0)
    {
        return false;
    }
    return true;
}

// 检查源端口和目的端口是否在规则范围内
bool check_port(const struct sk_buff *skb, const struct Rule *rule)
{
    struct tcphdr *tcph = tcp_hdr(skb); // 获取tcp头部
    struct udphdr *udph = udp_hdr(skb); // 获取udp头部
    // 检查源端口
    if (strcmp(rule->src_port, "*") != 0)
    {
        if (tcph != NULL && ntohs(tcph->source) != atoi(rule->src_port))
        {
            return false;
        }
        if (udph != NULL && ntohs(udph->source) != atoi(rule->src_port))
        {
            return false;
        }
    }
    // 检查目的端口
    if (strcmp(rule->dst_port, "*") != 0)
    {
        if (tcph != NULL && ntohs(tcph->dest) != atoi(rule->dst_port))
        {
            return false;
        }
        if (udph != NULL && ntohs(udph->dest) != atoi(rule->dst_port))
        {
            return false;
        }
    }
    return true;
}

// 检查网络接口是否在规则范围内
bool check_interface(const struct sk_buff *skb, const struct Rule *rule)
{
    struct net_device *in = skb->dev; // 获取网络接口
    // 检查网络接口
    if (strcmp(rule->interface_type, "*") != 0 && strcmp(rule->interface_type, in->name) != 0)
    {
        return false;
    }
    return true;
}

// 检查规则是否匹配
bool check_rule(const struct sk_buff *skb, const struct Rule *rule)
{
    // 检查协议类型
    if (strcmp(rule->protocol_type, "TCP") != 0 && strcmp(rule->protocol_type, "UDP") != 0 && strcmp(rule->protocol_type, "ICMP") != 0)
    {
        return false;
    }

    // 检查ip
    if (!check_ip(skb, rule))
    {
        return false;
    }
    // 检查端口
    if (!check_port(skb, rule))
    {
        return false;
    }
    // 检查网络接口
    if (!check_interface(skb, rule))
    {
        return false;
    }
    // 检查时间
    struct timeval tv;
    struct tm *tm;
    do_gettimeofday(&tv);
    tm = (struct tm *)kmalloc(sizeof(struct tm), GFP_KERNEL);
    time_to_tm(tv.tv_sec, 0, tm);
    char cur_time[20];
    sprintf(cur_time, "%d-%02d-%02d %02d:%02d:%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    if (!check_time(cur_time, rule->begin_time, rule->end_time))
    {
        return false;
    }

    return true;
}
