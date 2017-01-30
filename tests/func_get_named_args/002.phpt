--TEST--
func_get_named_args int
--FILE--
<?php
function test(int $first) {
	var_dump(func_get_named_args());
}

test(1);
?>
--EXPECT--
array(1) {
  ["first"]=>
  int(1)
}



