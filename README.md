# Qitsimple (Queue It Simple)
Qitsimple named message queue. 
It runs as a daemon (service). Client applications can connect to qitsimple and push messages (tasks, jobs) to the queue and receive messages from the queue.
Connection is via the tcp socket so client applications can reside on the same or on another server to which there is network access.
Qitsimple it will be useful for the implementation of such micro-services architecture.
It implements prioritize messages when the messages with a higher priority "float upwards" in the queue.
Qitsimple implements posponed mesages when you can set the time period in seconds after which the only message will appear at the output queue.

How to use:

Client:
```php
<?php
require_once 'MicroQueue.php';
$client = new MicroQueue();
$client->connect("localhost", 5555);

if (!$client->put("queue_one", 5, "queue one - priority 5")) {
  echo "error";
}

$client->put("queue_one", 6, "queue one - priority 6");
$client->put("queue_one", 1000, "queue one - priority 1000");

$client->put("queue_two", 5, "queue two - priority 5");
$client->put("queue_two", 6, "queue two - priority 6");
$client->put("queue_two", 7, "queue two - priority 7");

$client->put_postponed("queue_two", 60, "queue two - 60 seconds later");

$client->put_postponed("queue_two", 5, "queue two - 5 seconds later");

$client->disconnect();
```
Worker:
```php
<?php
require_once 'MicroQueue.php';
$worker = new MicroQueue();
$worker->connect("localhost", 5555);
echo "Queue 1 length: " . $worker->length("queue_one")."\n";
echo "Queue 2 length: " . $worker->length("queue_two")."\n";
while(true) {
    $result = $worker->get("queue_two");
    if (strpos($result, "::no tasks::") !== 0) {
        echo $result."\n";
    }
    usleep(50000);
}
$client->disconnect();
```
Get status:
```php
require_once 'MicroQueue.php';

$obj = new MicroQueue();
$obj->connect("localhost", 5555);
$list = $obj->getQuequeList();
foreach ($list as $queue) {
    echo "Queue:" . $queue . " " . $obj->length($queue) . "\n";
}
$obj->disconnect();
```

How to compile:

Download project to some directory.
In directory:
```bash
cmake -DCMAKE_BUILD_TYPE=RELEASE
make
```
Now run daemon from root:

```
sudo qitsimple
```

