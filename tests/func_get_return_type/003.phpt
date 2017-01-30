--TEST--
func_get_return_type class
--FILE--
<?php
class User {}
function test() : User {
	var_dump(func_get_return_type());

	return new User;
}

test();
?>
--EXPECT--
string(4) "User"


