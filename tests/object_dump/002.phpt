--TEST--
object_dump std with properties
--FILE--
<?php
$object = new stdClass;
$object->one = 1;
var_dump(object_dump($object));
?>
--EXPECT--
array(1) {
  ["one"]=>
  int(1)
}

