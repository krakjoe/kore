--TEST--
func_get_return_type string
--FILE--
<?php
function test() : string {
	var_dump(func_get_return_type());

	return "";
}

test();
?>
--EXPECT--
string(6) "string"


