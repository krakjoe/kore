--TEST--
func_get_named_args format(int, string, variadic)
--FILE--
<?php
function test(int $first, string $second, int ... $third) {
	var_dump(func_get_named_args(function($name){
		return sprintf(":%s", $name);
	}));
}

test(1, "2", 3, 4, 5);
?>
--EXPECT--
array(3) {
  [":first"]=>
  int(1)
  [":second"]=>
  string(1) "2"
  [":third"]=>
  array(3) {
    [0]=>
    int(3)
    [1]=>
    int(4)
    [2]=>
    int(5)
  }
}





