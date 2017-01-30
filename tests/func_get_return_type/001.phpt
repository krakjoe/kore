--TEST--
func_get_return_type none
--FILE--
<?php
function test() {
	var_dump(func_get_return_type());
}

test();
?>
--EXPECT--
NULL


