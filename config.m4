
PHP_ARG_WITH(HowTo, whether to enable HowTo support,
[  --with-HowTo           Include HowTo support])
if test "$PHP_HOWTO" != "no"; then
	PHP_SUBST(HOWTO_SHARED_LIBADD)

dnl +---------------------------------------------------------------------+
dnl | 如果需要使用其他库，引入包含目录，抄这个                               |
dnl | 相当于 gcc -I/opt/lnmp/include                                       |
dnl +---------------------------------------------------------------------+
	PHP_ADD_INCLUDE(/opt/lnmp/include)
	PHP_ADD_INCLUDE(/usr/include/libxml2)
	PHP_ADD_INCLUDE(/usr/include/curl)
	PHP_ADD_INCLUDE(/usr/include/freetype2)

dnl +---------------------------------------------------------------------+
dnl | 如果需要使用其他库，引入链接搜索目录，抄这个                           |
dnl | 相当于 gcc -L/opt/lnmp/lib                                           |
dnl +---------------------------------------------------------------------+
	PHP_ADD_LIBPATH(/opt/lnmp/lib)

dnl +---------------------------------------------------------------------+
dnl | 链接三方库                                                           |
dnl | 相当于 gcc -lpthread -lpcre 等等                                     |
dnl +---------------------------------------------------------------------+
	PHP_ADD_LIBRARY(pthread)
	PHP_ADD_LIBRARY(freetype)
	PHP_ADD_LIBRARY(onig)
	PHP_ADD_LIBRARY_WITH_PATH(sqlite3,/opt/lnmp/lib,HOWTO_SHARED_LIBADD)


dnl +---------------------------------------------------------------------+
dnl | 定义对其它扩展的依赖                                                 |
dnl | 在 main.c 里面也有定义，我也不知道为啥要写两遍，照抄就是               |
dnl +---------------------------------------------------------------------+
	PHP_ADD_EXTENSION_DEP(ziima_dep,standard)
	PHP_ADD_EXTENSION_DEP(ziima_dep,spl)
	PHP_ADD_EXTENSION_DEP(ziima_dep,date)
	PHP_ADD_EXTENSION_DEP(ziima_dep,hash)
	PHP_ADD_EXTENSION_DEP(ziima_dep,json)
	PHP_ADD_EXTENSION_DEP(ziima_dep,curl)
	PHP_ADD_EXTENSION_DEP(ziima_dep,gd)

	PHP_NEW_EXTENSION(HowTo, \
	\
dnl +---------------+
dnl | 底层代码      |
dnl +--------------+
	base/string.c base/debug.c \
	\
dnl +---------------+
dnl | 函数定义       |
dnl +---------------+
	type/ht_empty.c type/ht_ztype.c  \
	\
dnl +---------------+
dnl | 类型定义       |
dnl +---------------+
	type/Loader.c \
	\
dnl +---------------+
dnl | 生命周期       |
dnl +---------------+
	life/global_ctor.c life/global_dtor.c life/pecl_ctor.c life/pecl_dtor.c life/script_ctor.c life/script_dtor.c life/pecl_info.c \
	\
dnl +---------------+
dnl | 主文件         |
dnl +---------------+
	main.c \
	, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi