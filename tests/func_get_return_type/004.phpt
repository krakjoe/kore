--TEST--
func_get_return_type void
--SKIPIF--
<?php if (PHP_VERSION_ID < 70100) die("skip 7.1+ only"); ?>
--FILE--
<?php
class User {}
function test() : void {
	var_dump(func_get_return_type());
}

test();
?>
--EXPECT--
string(4) "void"



