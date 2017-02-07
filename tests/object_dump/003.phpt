--TEST--
object_dump class with ignored debugInfo
--FILE--
<?php
class Test {
	public function __debugInfo() {
		return array("one");
	}

	public $one = 1;
	public $two = 2;
}
var_dump(object_dump(new Test));
?>
--EXPECT--
array(2) {
  ["one"]=>
  int(1)
  ["two"]=>
  int(2)
}

