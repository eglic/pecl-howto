<?php

include_once "base.php";

enum Human: int {
	case Baby = 0;
	case Man = 1;
	case Woman = 2;
}

function inner_display_val(mixed $val): string {
	if ($val === null) return 'null';
	if ($val === false) return 'false';
	if ($val === true) return 'true';
	if (is_int($val)) return "$val";
	if (is_float($val)) return sprintf("%g", $val);
	if (is_string($val)) return '"' . str_replace(
		["\r", "\n", "\t"],
		['\r', '\n', '\t'],
		$val
	) . '"';
	if (is_array($val)) {
		if (empty($val)) return '[]';
		else return '[...]';
	}
	if (is_object($val)) {
		if ($val instanceof stdClass)
			return 'stdClass()';
		if ($val instanceof Human)
			return $val->name;
	}
	return "$val";
}
foreach ([null, false, true, 0, 1, 0.0, 1.1, 1.0e5, 1.1e-10, 2.3e-12, '', ' ', "  \n\r\t  ", [], [1], [1, 2, 3], new stdClass(), Human::Baby, Human::Man] as $v) {
	printf(
		"ht_empty( %s , %s) = %s\n",
		ht_ztype($v),
		inner_display_val($v),
		(ht_empty($v) ? 'yes' : 'no')
	);
}

echo "----------------------------------------------------------------------------\n";
printf("   global constant __HT_VERSION__ = %s\n",__HT_VERSION__);
printf("namespace constant HT\\__VERSION__ = %s\n",HT\__VERSION__);
