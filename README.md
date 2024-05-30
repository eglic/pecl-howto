# 好土

一个用于展示、记录 PHP 扩展开发过程的扩展。

## 文件说明
| 文件/目录              | 重要文件 | 功能说明                                    |
| :--------------------- | :------- | :------------------------------------------ |
| [dist](dist/README.md) |          | 存放各种脚本，主要用于一键调试              |
| [base](base/README.md) |          | 存放一些 C 语言底层的工具和封装类           |
|                        | base.h   | 原生C常用类型和工具                         |
| [life](life/README.md) |          | 扩展生命周期管理的函数拆开放这里            |
|                        | base.h   | 全局变量                                    |
| [type](type/README.md) |          | 各种提供给PHP的类型、枚举、接口、函数放这里 |
|                        | base.h   | 各种好用的宏                                |
|                        | class.h  | 实现类常用的宏                              |
|                        | enum.h   | 实现枚举常用的宏                            |
| [test](test/README.md) |          | 按自己喜好来准备用例                        |
| config.m4              |          | 每个扩展都必须的入口                        |
| project.h              |          | 项目配置宏定义                              |
| main.c                 |          | 扩展的主程序                                |

## 怎么用

``` 
howto use howto
```
## 开发构建
```bash
phpize && ./configure;
make && sudo make install;
php test/all.php;
```

## 发布构建
本地得有 docker
```bash
./dist/
```

## 小问题

 Q: 构建完了目录下文件好乱怎么办
```bash
make clean && phpize --clean
```
