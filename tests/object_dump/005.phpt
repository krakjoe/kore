--TEST--
object_dump class with private property
--FILE--
<?php
class Test {
	private $one = 1;
}
var_dump(object_dump(new Test));
?>
--EXPECT--
array(1) {
  ["Test::one"]=>
  int(1)
}

