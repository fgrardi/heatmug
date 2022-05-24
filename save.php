<?php
    $time = Date("H:i:s");
    // $temp = $_POST['temp'];
    // $btnState = $_POST['isPressed'];
    // $data = (object) array('time' => $time, 'temp' => $temp, 'btnState' => $btnState);
    $temp = $_POST['temp'] . " ";
    
    $data = array("time" => $time, "temp" => $temp);

    file_put_contents('save.html', implode(' ', $data)."\n");
