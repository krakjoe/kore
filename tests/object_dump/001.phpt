--TEST--
object_dump std
--FILE--
<?php
var_dump(object_dump(new stdClass));
?>
--EXPECT--
array(0) {
}
