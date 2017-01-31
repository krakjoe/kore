--TEST--
func_get_named_args no prototype
--FILE--
<?php
function test() {
	var_dump(func_get_named_args());
}

test(1,2,3);
?>
--EXPECT--
array(0) {
}






