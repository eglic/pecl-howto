<?php

/**
 * 注意： 最好在 php.ini 中配置
 *        用 dl 加载的，莫名其妙段错误，很难搞
 * 		  可能跟 ZTS 有关，懒得管了
 */
if (!extension_loaded('HowTo')) {
	dl('HowTo.so');
}
