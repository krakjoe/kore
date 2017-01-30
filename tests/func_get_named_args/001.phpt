--TEST--
func_get_named_args
--FILE--
<?php
function test() {
	var_dump(func_get_named_args());
}

test();
?>
--EXPECT--
array(0) {
}


