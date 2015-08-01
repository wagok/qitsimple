<?php

/**
 * Created by IntelliJ IDEA.
 * User: wlad
 * Date: 7/16/15
 * Time: 9:50 PM
 */

define ('MAX_DATA_SIZE', 524288); // 512K
define ("END_OF_STREAM", chr(0));

class Qitsimple
{
    public $socket;
    protected $timeout;

    /**
     * Connect to server
     * @param $host Hostname
     * @param $port Port
     * @param $timeout Timeout in ms
     * @return bool
     */
    public function connect($host, $port, $timeout = 100)
    {
        $this->timeout = $timeout;
        $service_port = $port;
        $address = gethostbyname($host);
        $this->socket = @socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
        if ($this->socket === false) {
            return false;
        } else
            $result = @socket_connect($this->socket, $address, $service_port);
        if ($result === false) {
            return false;
        }
    }

    /**
     * Put message to queue
     * @param $name Queue name
     * @param $priority Priority
     * @param string $str Data
     * @return bool
     */
    public function put($name, $priority, $str = "")
    {
        if (strlen($name) > 255) {
            $name = substr($name, 0, 255);
        }
        if ($priority < 1) {
            $priority = 1;
        }
        if ($priority > 256 * 256) {
            $priority = 256 * 256;
        }
        $priority_high = $priority >> 8;
        $priority_low = $priority % 256;
        $str = 'p' . chr(strlen($name)) . $name . chr($priority_high) . chr($priority_low) . $str . END_OF_STREAM;
        $result = $this->execute($str);
        if ($result === "ok\n") {
            return true;
        } else {
            return false;
        }
    }

    /**
     * Clean queue
     * @param $name queue name
     * @return bool
     */
    public function clean($name)
    {
        if (strlen($name) > 255) {
            $name = substr($name, 0, 255);
        }
        $str = 'e' . chr(strlen($name)) . $name . END_OF_STREAM;
        $result = $this->execute($str);
        if ($result === "ok\n") {
            return true;
        } else {
            return false;
        }
    }
    /**
     * Put postponed message to queue
     * @param $name Queue name
     * @param $time Time (delay) im seconds
     * @param string $str Data
     * @return bool
     */
    public function put_postponed($name, $time, $str = "")
    {
        if (strlen($name) > 255) {
            $name = substr($name, 0, 255);
        }
        if ($time > 256 * 256) {
            $time = 256 * 256;
        }
        $time_high = $time >> 8;
        $time_low = $time % 256;
        $str = 't' . chr(strlen($name)) . $name . chr($time_high) . chr($time_low) . $str . END_OF_STREAM;
        $result = $this->execute($str);
        if ($result === "ok\n") {
            return true;
        } else {
            false;
        }
    }

    /**
     * Get message from queue
     * @param $name Queue name
     * @return string Message
     */
    public function get($name)
    {
        if (strlen($name) > 255) {
            $name = substr($name, 0, 255);
        }

        $str = 'g' . chr(strlen($name)) . $name . END_OF_STREAM;
        $result = $this->execute($str);
        return $result;
    }
    /**
     * Get counter value
     * @param $name Counter name
     * @return integer Value
     */
    public function getCounter($name)
    {
        if (strlen($name) > 255) {
            $name = substr($name, 0, 255);
        }

        $str = 'G' . chr(strlen($name)) . $name . END_OF_STREAM;
        $result = $this->execute($str);
        return $result;
    }
    /**
     * Increment counter value
     * @param $name Counter name
     * @return integer Value
     */
    public function incCounter($name)
    {
        if (strlen($name) > 255) {
            $name = substr($name, 0, 255);
        }

        $str = 'I' . chr(strlen($name)) . $name . END_OF_STREAM;
        $result = $this->execute($str);
        return $result;
    }
    /**
     * Decrement counter value
     * @param $name Counter name
     * @return integer Value
     */
    public function decCounter($name)
    {
        if (strlen($name) > 255) {
            $name = substr($name, 0, 255);
        }

        $str = 'D' . chr(strlen($name)) . $name . END_OF_STREAM;
        $result = $this->execute($str);
        return $result;
    }
   /**
     * Clear (reset) counter
     * @param $name Counter name
     * @return integer Value
     */
    public function clearCounter($name)
    {
        if (strlen($name) > 255) {
            $name = substr($name, 0, 255);
        }

        $str = 'E' . chr(strlen($name)) . $name . END_OF_STREAM;
        $result = $this->execute($str);
        return $result;
    }

    /**
     * @return array Not empty queues list
     */
    public function getQuequeList()
    {
        $str = 'q  ' . END_OF_STREAM;
        $result = $this->execute($str);
        $result = trim($result);
        if (empty($result)) {
            return array();
        }
        return explode("\n", $result);

    }
    /**
     * @return array Counters list
     */
    public function getCountersList()
    {
        $str = 'Q  ' . END_OF_STREAM;
        $result = $this->execute($str);
        $result = trim($result);

        if (empty($result)) {
            return array();
        }
        return explode("\n", $result);

    }

    /**
     * Get length of queue (messages count)
     * @param $name Queue name
     * @return string Length of queue
     */
    public function length($name)
    {

        $str = 'l' . chr(strlen($name)) . $name . END_OF_STREAM;
        return $this->execute($str);
    }

    /**
     * Disconnect from server
     */
    public function disconnect()
    {
        socket_close($this->socket);
        $this->socket = null;
    }

    /**
     * @param $str
     * @return string
     */
    protected function execute($str)
    {
        @socket_write($this->socket, $str, strlen($str));
        $endtime = microtime(true) + $this->timeout / 1000;
        $result = "";
        $out = "";
        while ($endtime > microtime(true)) {
            $bytes = @socket_recv($this->socket, $out, MAX_DATA_SIZE, MSG_DONTWAIT);

            if (!empty($out)) {
                $result .= $out;
            }
            if (substr($out, -1) === END_OF_STREAM) {
                break;
            }
        }
        return str_replace(END_OF_STREAM, "", $result);
    }




}