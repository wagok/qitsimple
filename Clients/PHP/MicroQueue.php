<?php

/**
 * Created by IntelliJ IDEA.
 * User: wlad
 * Date: 7/16/15
 * Time: 9:50 PM
 */
class MicroQueue
{
    public $socket;

    /**
     * Connect to server
     * @param $host Hostname
     * @param $port Port
     * @return bool
     */
    public function connect($host, $port)
    {
        $service_port = $port;
        $address = gethostbyname($host);
        $this->socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
        if ($this->socket === false) {
            return false;
        } else
            $result = socket_connect($this->socket, $address, $service_port);
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
        $str = 'p' . chr(strlen($name)) . $name . chr($priority_high) . chr($priority_low) . $str;
        $res = socket_write($this->socket, $str, strlen($str));
        $out = "";
        $result = "";
        while (true) {
            $bytes = socket_recv($this->socket, $out, 2048, MSG_DONTWAIT);
            if ($bytes === false) {
                $err = socket_strerror(socket_last_error($this->socket));
            }
            if (!empty($out)) {
                $result .= $out;
            }
            if (substr($out, -1) === chr(0)) {
                break;
            }
        }
        $result = str_replace(chr(0), "", $result);
        if ($result === "ok\n") {
            return true;
        } else {
            false;
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
        $str = 't' . chr(strlen($name)) . $name . chr($time_high) . chr($time_low) . $str;
        $res = socket_write($this->socket, $str, strlen($str));
        $out = "";
        $result = "";
        while (true) {
            $bytes = socket_recv($this->socket, $out, 2048, MSG_DONTWAIT);
            if ($bytes === false) {
                $err = socket_strerror(socket_last_error($this->socket));
            }
            if (!empty($out)) {
                $result .= $out;
            }
            if (substr($out, -1) === chr(0)) {
                break;
            }
        }
        $result = str_replace(chr(0), "", $result);
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
        $result = "";
        $out = "";
        $str = 'g' . chr(strlen($name)) . $name;
        socket_write($this->socket, $str, strlen($str));
        while (true) {
            $bytes = socket_recv($this->socket, $out, 2048, MSG_DONTWAIT);

            if (!empty($out)) {
                $result .= $out;
            }
            if (substr($out, -1) === chr(0)) {
                break;
            }
        }
        return str_replace(chr(0), "", $result);
    }

    /**
     * @return array Not empty queues list
     */
    public function getQuequeList()
    {
        $result = "";
        $out = "";
        $str = 'q  ';
        socket_write($this->socket, $str, strlen($str));
        while (true) {
            $bytes = socket_recv($this->socket, $out, 2048, MSG_DONTWAIT);
            if (!empty($out)) {
                $result .= $out;
            }
            if (substr($out, -1) === chr(0)) {
                break;
            }
        }
        $result = trim(str_replace(chr(0), "", $result));
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
        $result = "";
        $out = "";
        $str = 'l' . chr(strlen($name)) . $name;
        socket_write($this->socket, $str, strlen($str));
        while (true) {
            $bytes = socket_recv($this->socket, $out, 2048, MSG_DONTWAIT);
            if (!empty($out)) {
                $result .= $out;
            }
            if (substr($out, -1) === chr(0)) {
                break;
            }
        }
        return str_replace(chr(0), "", $result);
    }

    /**
     * Disconnect from server
     */
    public function disconnect()
    {
        socket_close($this->socket);
        $this->socket = null;
    }

}