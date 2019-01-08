<?php

namespace Concurrent;

$domain = (\DIRECTORY_SEPARATOR == '\\') ? \STREAM_PF_INET : \STREAM_PF_UNIX;

list ($a, $b) = stream_socket_pair($domain, \STREAM_SOCK_STREAM, \STREAM_IPPROTO_IP);

foreach ([$a, $b] as $r) {
    stream_set_blocking($r, false);
    stream_set_read_buffer($r, 0);
    stream_set_write_buffer($r, 0);
}

$watcher = new StreamWatcher($b);

Task::async(function () use ($b, $watcher) {
    $watcher->awaitReadable();
    var_dump('GET CONTENT...');
    var_dump(stream_get_contents($b, 0xFFFF));
});

Task::background(function () use ($a, $b, $watcher) {
    try {
        while (true) {
            $watcher->awaitReadable();
        }
    } catch (\Throwable $e) {
        echo $e;
    }
});

Task::async(function () use ($a) {
    (new Timer(250))->awaitTimeout();
    var_dump('TIMER TRIGGERED');
    
    fwrite($a, 'Hello!');
});
