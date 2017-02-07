--TEST--
object_dump class with protected property
--FILE--
<?php
class Test {
	protected $one = 1;
}
var_dump(object_dump(new Test));
?>
--EXPECT--
array(1) {
  ["one"]=>
  int(1)
}

