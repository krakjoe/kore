--TEST--
func_get_named_args format(int)
--FILE--
<?php
function test(int $first) {
	var_dump(func_get_named_args(function($name) {
		return sprintf(":%s", $name);
	}));
}

test(1);
?>
--EXPECT--
array(1) {
  [":first"]=>
  int(1)
}



