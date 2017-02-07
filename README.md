# kore

[![Build Status](https://travis-ci.org/krakjoe/kore.svg?branch=master)](https://travis-ci.org/krakjoe/kore)

```php
/**
* Shall return array of arguments passed to calling function
*
* @param <callable> shall be called to format the name of each argument
*/
function func_get_named_args([callable format]) : array;

/**
* Shall return the return type of calling function
*/
function func_get_return_type() : ?string;

/**
* Shall return get_properties output, unmangled, something like pre debugInfo
*/
function object_dump(object $object) : array;
```
