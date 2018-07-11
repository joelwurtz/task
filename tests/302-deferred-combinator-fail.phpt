--TEST--
Deferred combinator ending with failure.
--SKIPIF--
<?php
if (!extension_loaded('task')) echo 'Test requires the task extension to be loaded';
?>
--FILE--
<?php

namespace Concurrent;

$scheduler = new TaskScheduler();

try {
    $scheduler->run(function () {
        var_dump('A');

        return Task::await(Deferred::combine([
            Deferred::error(new \Error('Fail!'))
        ], function (Deferred $defer, bool $last, int $index, ?\Throwable $e, $v) {
            var_dump('B');
            var_dump($last);
            var_dump($v);
            $defer->fail($e);
            var_dump('C');
        }));
    });
} catch (\Throwable $e) {
    var_dump($e->getMessage());
}

?>
--EXPECT--
string(1) "A"
string(1) "B"
bool(true)
NULL
string(1) "C"
string(5) "Fail!"
