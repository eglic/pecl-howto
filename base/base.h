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

#define NSTR_IS_ASCII_SPACE(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\b' || c == '\v')

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

void nlog_console_echo(const char *func, log_level_t level, const char *fmt, ...);

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
