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
// #include <stdbool.h>
// #include <stdlib.h>
#include <linux/skbuff.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/timekeeping.h>
#include <linux/inet.h>
#include <linux/file.h>
#include <asm/uaccess.h>
#include <linux/fcntl.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>

#define MAX_RULES_NUM 30                 // 最大规则数
#define MAX_INPUT_STR_LEN 9999           // 最大输入字符串长度
#define LOG_FILE "/var/log/firewall.log" // 日志文件路径
#define DEV_FILE "controlinfo"           // 设备文件名

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
static char device[MAX_INPUT_STR_LEN];   // 设备缓冲区
// static struct semaphore sem;             // 信号量
static DECLARE_WAIT_QUEUE_HEAD(wq); // 等待队列
struct file *filep = NULL;          // 文件指针
loff_t pos = 0;                     // 文件偏移量
// struct rw_semaphore file_lock;           // 文件锁
char buf_log[256];                      // 日志缓冲区
static struct nf_hook_ops *nfho = NULL; // 网络过滤钩子

int atoi(char *pstr)
{
    int Ret_Integer = 0;
    int Integer_sign = 1;

    /*
     * 跳过前面的空格字符
     */
    while (isspace(*pstr) == 0)
    {
        pstr++;
    }

    /*
     * 判断正负号
     * 如果是正号，指针指向下一个字符
     * 如果是符号，把符号标记为Integer_sign置-1，然后再把指针指向下一个字符
     */
    if (*pstr == '-')
    {
        Integer_sign = -1;
    }
    if (*pstr == '-' || *pstr == '+')
    {
        pstr++;
    }

    /*
     * 把数字字符串逐个转换成整数，并把最后转换好的整数赋给Ret_Integer
     */
    while (*pstr >= '0' && *pstr <= '9')
    {
        Ret_Integer = Ret_Integer * 10 + *pstr - '0';
        pstr++;
    }
    Ret_Integer = Integer_sign * Ret_Integer;

    return Ret_Integer;
}
// 检查源ip和目的ip是否在规则范围内
bool check_ip(const struct sk_buff *skb, const struct Rule *rule)
{
    // 打印rule的全部信息
    // printk(KERN_INFO "rule_id=%d\n", rule->id);
    // printk(KERN_INFO "rule_protocol_type=%s\n", rule->protocol_type);
    // printk(KERN_INFO "rule_interface_type=%s\n", rule->interface_type);
    // printk(KERN_INFO "rule_src_ip=%s\n", rule->src_ip);
    // printk(KERN_INFO "rule_src_port=%s\n", rule->src_port);
    // printk(KERN_INFO "rule_dst_ip=%s\n", rule->dst_ip);
    // printk(KERN_INFO "rule_dst_port=%s\n", rule->dst_port);
    // printk(KERN_INFO "rule_begin_time=%s\n", rule->begin_time);
    // printk(KERN_INFO "rule_end_time=%s\n", rule->end_time);

    // printk(KERN_INFO "start check_ip\n");
    struct iphdr *iph = ip_hdr(skb); // 获取ip头部
    char src_ip_str[16];
    char dst_ip_str[16];
    sprintf(src_ip_str, "%pI4", &iph->saddr);
    sprintf(dst_ip_str, "%pI4", &iph->daddr);

    // printk(KERN_INFO "src_ip=%s, dst_ip=%s\n", src_ip_str, dst_ip_str);
    // printk(KERN_INFO "rule_src_ip=%s, rule_dst_ip=%s\n", rule->src_ip, rule->dst_ip);

    //  检查源ip
    if (strcmp(rule->src_ip, "$") != 0 && strcmp(rule->src_ip, dst_ip_str) != 0)
    {
        return false;
    }
    // 检查目的ip
    if (strcmp(rule->dst_ip, "$") != 0 && strcmp(rule->dst_ip, src_ip_str) != 0)
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
    // 输出端口号和规则端口号
    // printk(KERN_INFO "src_port=%d, dst_port=%d\n", ntohs(tcph->source), ntohs(tcph->dest));
    // printk(KERN_INFO "rule_src_port=%s, rule_dst_port=%s\n", rule->src_port, rule->dst_port);
    // 检查源端口
    if (strcmp(rule->src_port, "$") != 0)
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
    if (strcmp(rule->dst_port, "$") != 0)
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
    // 输出网络接口和规则网络接口
    // printk(KERN_INFO "interface_type=%s\n", in->name);
    // printk(KERN_INFO "rule_interface_type=%s\n", rule->interface_type);
    // 检查网络接口
    if (strcmp(rule->interface_type, "$") != 0 && strcmp(rule->interface_type, in->name) != 0)
    {
        return false;
    }
    return true;
}

// 比较两个时间字符串的大小，字符串格式为"YYYY-MM-DD HH:MM:SS"，返回值为1表示time1大于time2，返回值为0表示time1等于time2，返回值为-1表示time1小于time2
int istimebig(const char *time1, const char *time2)
{
    int year1, month1, day1, hour1, minute1, second1;
    int year2, month2, day2, hour2, minute2, second2;
    sscanf(time1, "%d-%d-%d %d:%d:%d", &year1, &month1, &day1, &hour1, &minute1, &second1);
    sscanf(time2, "%d-%d-%d %d:%d:%d", &year2, &month2, &day2, &hour2, &minute2, &second2);
    if (year1 > year2)
    {
        return 1;
    }
    else if (year1 < year2)
    {
        return -1;
    }
    else
    {
        if (month1 > month2)
        {
            return 1;
        }
        else if (month1 < month2)
        {
            return -1;
        }
        else
        {
            if (day1 > day2)
            {
                return 1;
            }
            else if (day1 < day2)
            {
                return -1;
            }
            else
            {
                if (hour1 > hour2)
                {
                    return 1;
                }
                else if (hour1 < hour2)
                {
                    return -1;
                }
                else
                {
                    if (minute1 > minute2)
                    {
                        return 1;
                    }
                    else if (minute1 < minute2)
                    {
                        return -1;
                    }
                    else
                    {
                        if (second1 > second2)
                        {
                            return 1;
                        }
                        else if (second1 < second2)
                        {
                            return -1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }
}

// 检查时间是否在规则时间范围内，字符串格式为"YYYY-MM-DD HH:MM:SS"
bool check_time(const char *cur_time, const char *begin_time, const char *end_time)
{
    // 如果对时间无要求
    if (strcmp(begin_time, "$") == 0 && strcmp(end_time, "$") == 0)
    {
        return true;
    }
    // 如果对begin_time无要求
    if (strcmp(begin_time, "$") == 0)
    {
        if (istimebig(cur_time, end_time) <= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    // 如果对end_time无要求
    if (strcmp(end_time, "$") == 0)
    {
        if (istimebig(cur_time, begin_time) >= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    // 如果对时间有要求
    if (istimebig(cur_time, begin_time) >= 0 && istimebig(cur_time, end_time) <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }

    return false;
}

// 检查规则是否匹配
bool check_rule(const struct sk_buff *skb, const struct Rule *rule)
{
    // 检查协议类型
    int protocol = ip_hdr(skb)->protocol;
    if (strcmp(rule->protocol_type, "$") != 0)
    {
        if (strcmp(rule->protocol_type, "tcp") == 0 && protocol != IPPROTO_TCP)
        {
            return false;
        }
        if (strcmp(rule->protocol_type, "udp") == 0 && protocol != IPPROTO_UDP)
        {
            return false;
        }
        if (strcmp(rule->protocol_type, "icmp") == 0 && protocol != IPPROTO_ICMP)
        {
            return false;
        }
    }
    // printk(KERN_INFO "protocol_type: %s\n", protocol == IPPROTO_TCP ? "tcp" : protocol == IPPROTO_UDP ? "udp"
    //                                                                                                  : "icmp");
    // printk(KERN_INFO "protocol_type: %s\n", rule->src_ip);

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
    struct timespec64 tv;
    struct tm tm;
    char cur_time[20];
    ktime_get_real_ts64(&tv);
    time64_to_tm(tv.tv_sec, 0, &tm);
    int shiqu = (tm.tm_hour + 8) / 24;

    sprintf(cur_time, "%04ld-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday + shiqu, (tm.tm_hour + 8) % 24, tm.tm_min, tm.tm_sec);

    // printk(KERN_INFO "cur_time=%s\n", cur_time);
    // printk(KERN_INFO "begin_time=%s, end_time=%s\n", rule->begin_time, rule->end_time);
    // printk(KERN_INFO "check_time=%d\n", istimebig(rule->begin_time, rule->end_time));

    if (!check_time(cur_time, rule->begin_time, rule->end_time))
    {
        return false;
    }

    return true;
}

// 打印日志文件，日志文件指的是firewall.log，其内容为阻止的数据包信息
static int print_log(const char *log)
{
    ssize_t len = snprintf(buf_log, sizeof(buf_log), "%s", log);

    if (len < 0)
    {
        pr_err("Failed to format data\n");
        return -1;
    }

    // 获取文件锁
    // down_write(&file_lock);

    kernel_write(filep, buf_log, len, &pos);

    // 释放文件锁
    // up_write(&file_lock);

    return 0;
}

// 将读取到的字符串解析并保存在结构体数组中，即分割字符串，将每个规则保存在一个结构体中
// 每个规则之间用分号分割，规则的每个字段之间用逗号分割
// 不需要错误处理，其在用户态已经处理过
static void parse_rules(void)
{
    char *p = device;
    char *q = device;
    while (*p != '\0')
    {
        if (*p == ';')
        {
            *p = '\0';
            char *rule = q;
            q = p + 1;
            char *protocol_type = strsep(&rule, ",");
            char *interface_type = strsep(&rule, ",");
            char *src_ip = strsep(&rule, ",");
            char *src_port = strsep(&rule, ",");
            char *dst_ip = strsep(&rule, ",");
            char *dst_port = strsep(&rule, ",");
            char *begin_time = strsep(&rule, ",");
            char *end_time = strsep(&rule, ",");
            // 保存规则
            rules[rules_num].id = rules_num;
            rules[rules_num].protocol_type = kstrdup(protocol_type, GFP_KERNEL);
            rules[rules_num].interface_type = kstrdup(interface_type, GFP_KERNEL);
            rules[rules_num].src_ip = kstrdup(src_ip, GFP_KERNEL);
            rules[rules_num].src_port = kstrdup(src_port, GFP_KERNEL);
            rules[rules_num].dst_ip = kstrdup(dst_ip, GFP_KERNEL);
            rules[rules_num].dst_port = kstrdup(dst_port, GFP_KERNEL);
            rules[rules_num].begin_time = kstrdup(begin_time, GFP_KERNEL);
            rules[rules_num].end_time = kstrdup(end_time, GFP_KERNEL);
            rules_num++;

            // test
            // printk(KERN_INFO "protocol_type: %s\n", rules[rules_num - 1].protocol_type);
            // printk(KERN_INFO "interface_type: %s\n", rules[rules_num - 1].interface_type);
            // printk(KERN_INFO "src_ip: %s\n", rules[rules_num - 1].src_ip);
            // printk(KERN_INFO "src_port: %s\n", rules[rules_num - 1].src_port);
            // printk(KERN_INFO "dst_ip: %s\n", rules[rules_num - 1].dst_ip);
            // printk(KERN_INFO "dst_port: %s\n", rules[rules_num - 1].dst_port);
            // printk(KERN_INFO "begin_time: %s\n", rules[rules_num - 1].begin_time);
            // printk(KERN_INFO "end_time: %s\n", rules[rules_num - 1].end_time);
        }
        p++;
    }
}

// 读取设备文件，把规则字符串读取到缓冲区中
// 参数分别表示文件指针，用户缓冲区，读取的字节数，偏移量
static ssize_t read_control(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    int temp = simple_read_from_buffer(buf, count, ppos, device, MAX_INPUT_STR_LEN); // 从device中读取数据到buf中,返回值为读取的字节数
    return temp;
}

// 写入设备文件，把规则解析并保存在结构体数组中，参数同上
static ssize_t write_control(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    int temp = simple_write_to_buffer(device, MAX_INPUT_STR_LEN, ppos, buf, count); // 从buf中写入数据到device中,返回值为写入的字节数
    if (temp > 0)
    {
        // 清空规则
        rules_num = 0;
        memset(rules, 0, sizeof(rules));
        // 解析规则,注意信号量
        // if (down_interruptible(&sem))
        // { // 获取信号量
        //     // 进入临界区
        //     wait_event_interruptible(wq, (sem.count > 0));
        // }
        parse_rules();
        // up(&sem); // 释放信号量
    }
    // 初始化设备缓冲区
    memset(device, 0, MAX_INPUT_STR_LEN);
    return temp;
}

// 数据包处理函数,处理数据包的函数，当数据包匹配规则时，将数据包信息写入日志文件
// 参数分别表示私有数据，数据包，钩子状态
// 注意信号量的使用
static unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    // 打印skb内容中的源ip和目的ip
    // printk(KERN_INFO "src_ip=%pI4, dst_ip=%pI4\n", &ip_hdr(skb)->saddr, &ip_hdr(skb)->daddr);

    // if (down_interruptible(&sem))
    // {
    //     wait_event_interruptible(wq, (sem.count > 0));
    // }
    int i;
    for (i = 0; i < rules_num; i++)
    {
        printk(KERN_INFO "check rule %d\n", i);
        if (check_rule(skb, &rules[i]))
        {
            printk(KERN_INFO "rule %d matched\n", i);
            // 打印日志
            char log[200];
            // struct iphdr *iph = ip_hdr(skb);
            // struct tcphdr *tcph = tcp_hdr(skb);
            // struct udphdr *udph = udp_hdr(skb);
            // struct icmphdr *icmph = icmp_hdr(skb);
            // char src_ip[16];
            // char dst_ip[16];
            // sprintf(src_ip, "%pI4", &iph->saddr);
            // sprintf(dst_ip, "%pI4", &iph->daddr);
            // if (tcph != NULL)
            // {
            //     sprintf(log, "Blocked TCP packet: src_ip=%s, src_port=%d, dst_ip=%s, dst_port=%d\n", src_ip, ntohs(tcph->source), dst_ip, ntohs(tcph->dest));
            // }
            // else if (udph != NULL)
            // {
            //     sprintf(log, "Blocked UDP packet: src_ip=%s, src_port=%d, dst_ip=%s, dst_port=%d\n", src_ip, ntohs(udph->source), dst_ip, ntohs(udph->dest));
            // }
            // else if (icmph != NULL)
            // {
            //     sprintf(log, "Blocked ICMP packet: src_ip=%s, dst_ip=%s\n", src_ip, dst_ip);
            // }
            // 输出格式为[时间] 协议类型 源IP:源端口 -> 目的IP:目的端口,时间格式为"YYYY-MM-DD HH:MM:SS"
            struct timespec64 tv;
            struct tm tm;
            char cur_time[20];
            ktime_get_real_ts64(&tv);
            time64_to_tm(tv.tv_sec, 0, &tm);
            int shiqu = (tm.tm_hour + 8) / 24;

            sprintf(cur_time, "%04ld-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday + shiqu, (tm.tm_hour + 8) % 24, tm.tm_min, tm.tm_sec);

            char src_ip[16];
            char dst_ip[16];
            sprintf(src_ip, "%pI4", &ip_hdr(skb)->saddr);
            sprintf(dst_ip, "%pI4", &ip_hdr(skb)->daddr);

            // 获取skb的协议类型
            char *protocol_type;
            if (ip_hdr(skb)->protocol == IPPROTO_TCP)
            {
                protocol_type = "TCP";
            }
            else if (ip_hdr(skb)->protocol == IPPROTO_UDP)
            {
                protocol_type = "UDP";
            }
            else if (ip_hdr(skb)->protocol == IPPROTO_ICMP)
            {
                protocol_type = "ICMP";
            }
            else
            {
                protocol_type = "UNKNOWN";
            }

            sprintf(log, "Blocked [%s] %s %s:%d -> %s:%d\n", cur_time, protocol_type, src_ip, atoi(rules[i].src_port), dst_ip, atoi(rules[i].dst_port));

            print_log(log);
            return NF_DROP;
        }
    }
    // up(&sem);
    return NF_ACCEPT;
}

// 字符设备注册
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = read_control,
    .write = write_control,
};
static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEV_FILE,
    .fops = &fops,
};

// 初始化模块,功能包括初始化设备文件，初始化文件锁，注册设备，注册网络过滤钩子，初始化设备缓冲区
static int __init firewall_init(void)
{

    printk(KERN_INFO "Firewall module loaded\n");
    // 初始化设备文件
    struct file *file = filp_open(LOG_FILE, O_RDWR | O_CREAT | O_APPEND, 0644);
    if (IS_ERR(file))
    {
        pr_err("Failed to open file\n");
        return -1;
    }
    filep = file;
    // 初始化文件锁
    // init_rwsem(&file_lock);

    misc_register(&misc); // 注册设备

    // 注册网络过滤钩子

    nfho = (struct nf_hook_ops *)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);
    if (nfho == NULL)
    {
        pr_err("Failed to allocate memory\n");
        return -1;
    }
    nfho->hook = hook_func;              // 数据包处理函数
    nfho->hooknum = NF_INET_PRE_ROUTING; // 钩子位置
    nfho->pf = NFPROTO_IPV4;             // 协议族
    nfho->priority = NF_IP_PRI_FIRST;    // 优先级

    nf_register_net_hook(&init_net, nfho);
    // 初始化设备缓冲区
    memset(device, 0, MAX_INPUT_STR_LEN);
    // 初始化信号量
    // sema_init(&sem, 1);
    // init_waitqueue_head(&wq);
    return 0;
}

// 模块注销,功能包括注销设备，注销网络过滤钩子，释放内存，关闭文件，清空规则数组
static void __exit firewall_exit(void)
{
    // 注销设备
    misc_deregister(&misc);
    // 注销网络过滤钩子
    nf_unregister_net_hook(&init_net, nfho);
    // 释放内存
    int i;
    for (i = 0; i < rules_num; i++)
    {
        kfree(rules[i].protocol_type);
        kfree(rules[i].interface_type);
        kfree(rules[i].src_ip);
        kfree(rules[i].src_port);
        kfree(rules[i].dst_ip);
        kfree(rules[i].dst_port);
        kfree(rules[i].begin_time);
        kfree(rules[i].end_time);
    }

    // 删除LOG_FILE中的内容
    filp_close(filep, NULL);

        // 清空规则数组
    rules_num = 0;
    memset(rules, 0, sizeof(rules));
}

module_init(firewall_init);
module_exit(firewall_exit);

MODULE_LICENSE("GPL");
