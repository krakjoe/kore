--TEST--
func_get_named_args format(int, string)
--FILE--
<?php
function test(int $first, string $second) {
	var_dump(func_get_named_args(function($name) {
		return sprintf(":%s", $name);
	}));
}

test(1, "2");
?>
--EXPECT--
array(2) {
  [":first"]=>
  int(1)
  [":second"]=>
  string(1) "2"
}




