--TEST--
func_get_named_args int, string
--FILE--
<?php
function test(int $first, string $second) {
	var_dump(func_get_named_args());
}

test(1, "2");
?>
--EXPECT--
array(2) {
  ["first"]=>
  int(1)
  ["second"]=>
  string(1) "2"
}




