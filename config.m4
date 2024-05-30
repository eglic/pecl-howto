PHP_ARG_WITH(HowTo, whether to enable HowTo support,
[  --with-HowTo           Include HowTo support])
if test "$PHP_HOWTO" != "no"; then
	PHP_SUBST(HOWTO_SHARED_LIBADD)

	PHP_ADD_INCLUDE(/opt/lnmp/include)
	PHP_ADD_INCLUDE(/usr/include/libxml2)
	PHP_ADD_INCLUDE(/usr/include/curl)
	PHP_ADD_INCLUDE(/usr/include/freetype2)

	PHP_ADD_LIBPATH(/opt/lnmp/lib)

	PHP_ADD_LIBRARY(pthread)
	PHP_ADD_LIBRARY(freetype)
	PHP_ADD_LIBRARY(onig)
	PHP_ADD_LIBRARY_WITH_PATH(sqlite3,/opt/lnmp/lib,HOWTO_SHARED_LIBADD)

	PHP_ADD_EXTENSION_DEP(ziima_dep,standard)
	PHP_ADD_EXTENSION_DEP(ziima_dep,spl)
	PHP_ADD_EXTENSION_DEP(ziima_dep,date)
	PHP_ADD_EXTENSION_DEP(ziima_dep,hash)
	PHP_ADD_EXTENSION_DEP(ziima_dep,json)
	PHP_ADD_EXTENSION_DEP(ziima_dep,curl)
	PHP_ADD_EXTENSION_DEP(ziima_dep,gd)

	PHP_NEW_EXTENSION(HowTo,
	core/string.c \
	type/Loader.c \
	main.c \
	, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi