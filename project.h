#pragma once
/*
	项目配置文件
	Q: 为啥不叫 config.h
	A: 因为:
			1. configure 自己会生成一个 config.h 文件
			2. phpize --clean 后还会剩下一堆 config 开头的没用文件
			3. 想清理干净的话，在工程目录中 rm -f config* ，舒服。

	Q: 下面的 clang-format off 干啥的
	A: 禁止 clang-format 格式化后面的代码，这样就能保持队形了

*/

// clang-format off
#define PROJECT_NAME 				HowTo
#define PROJECT_NAME_STR 			"HowTo"
#define PROJECT_VERSION_MAJOR 		1
#define PROJECT_VERSION_MINOR 		2
#define PROJECT_VERSION_BUILD 		3
#define PROJECT_VERSION_STR 		"1.2.3-240601"
#define PROJECT_ROOT_NAMESPACE		HowTo
#define PROJECT_ROOT_NAMESPACE_STR	"HowTo"

#define PROJECT_INI_PREFIX 			"howto"
