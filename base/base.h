#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// [1] 全局宏函数
#define NSTR_IS_ASCII_SPACE( c ) ( c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\b' || c == '\v' )

// [2] 枚举
typedef enum {
    LL_NONE  = 0,
    LL_DEBUG = 1,
    LL_INFO  = 2,
    LL_DONE  = 3,
    LL_WARN  = 4,
    LL_ERROR = 5,
    LL_FATAL = 6,
    LL_FULL  = 0xFF
} log_level_t;

typedef enum {

    RS_NOOP = 0x00, // 未实现       501
    RS_OK   = 0x01, // 成功且继续   200
    RS_GOTO = 0x02, // 成功且调整   302
    RS_STOP = 0x03, // 成功且停止   405
    RS_LOST = 0x04, // 失败丢失     404

    RS_ERROR  = 0x0100,          // 500
    RS_PROTO  = RS_ERROR | 0x01, // 协议错误 , 400
    RS_PARAM  = RS_ERROR | 0x02, // 参数错误 , 400
    RS_LIMIT  = RS_ERROR | 0x03, // 超出限制 , 400
    RS_AUTH   = RS_ERROR | 0x04, // 未登录 , 401
    RS_DENY   = RS_ERROR | 0x05, // 无逻辑权限 403
    RS_PERM   = RS_ERROR | 0x06, // 无底层权限 403
    RS_STATUS = RS_ERROR | 0x07, // 状态错误 403
    RS_TARGET = RS_ERROR | 0x08, // 目标错误

    RS_FATAL = 0x0200,          // 500
    RS_ARCH  = RS_FATAL | 0x01, // 系统架构错误
    RS_CONF  = RS_FATAL | 0x02, // 系统配置错误
    RS_PROG  = RS_FATAL | 0x03, // 系统编程错误
    RS_BUSY  = RS_FATAL | 0x04, // 系统运行时错误 503

    RS_WTF = RS_ERROR | RS_FATAL | 0xFF, // 失败 500
} result_code_t;

// [3] 数据结构
#pragma pack( push, 1 )

// 给上层数据结构附魔的
typedef union {
    void         *ptr;
    int64_t       val;
    unsigned char bin[8];
} z_tag;

struct __us_node;
// 单双环链表/二叉树 , 带简易记录管理 , 本体长 40 B
typedef struct __us_node {
    union {
        struct __us_node *parent; // 父节点 , 与 owner 共体
        void             *owner;  // 归属对象 , 与  parent 共体
    };
    union {
        struct __us_node *next;  // 下一个 , 与 right 共体
        struct __us_node *right; // 右节点 , 与 next 共体
    };
    union {
        struct __us_node *prev; // 前一个 , 与 left 共体
        struct __us_node *left; // 左节点 ， 与 prev 共体
    };
    z_tag    tag; // 节点附加值
    uint32_t cap; // 内存容量
    struct {
        uint16_t len;     // 扩展内存单元长度
        uint16_t cnt;     // 扩展内存单元数量
    } unit;               // 扩展内存单元
    unsigned char bin[0]; // 扩展内存
} z_node;
#pragma pack( pop )

// [4] 基础函数定义

// [4.1] 对象池 / 超轻量级不智能化的内存分配器
z_node *object_pool_create( uint16_t unitSize, uint16_t unitCount );
void    object_pool_destroy( z_node *pool );
void   *object_pool_borrow( z_node *pool, uint16_t unitSize );
bool    object_pool_return( z_node *pool, void *ptr );

// 日志相关
void nlog_console_echo( const char *func, log_level_t level, const char *fmt, ... );

// clang-format off
#ifdef NODEBUG
	#define kdebug(...)
	#define kinfo(...)
	#define kdone(...)
	#define kwarn(...)
	#define kerror(...)
	#define kfatal(...)
#else
	#define kdebug(...) nlog_console_echo(__FUNCTION__, LL_DEBUG, __VA_ARGS__)
	#define kinfo(...)  nlog_console_echo(__FUNCTION__, LL_INFO, __VA_ARGS__)
	#define kdone(...)  nlog_console_echo(__FUNCTION__, LL_DONE, __VA_ARGS__)
	#define kwarn(...)  nlog_console_echo(__FUNCTION__, LL_WARN, __VA_ARGS__)
	#define kerror(...) nlog_console_echo(__FUNCTION__, LL_ERROR, __VA_ARGS__)
	#define kfatal(...) nlog_console_echo(__FUNCTION__, LL_FATAL, __VA_ARGS__)
#endif
// clang-format on
