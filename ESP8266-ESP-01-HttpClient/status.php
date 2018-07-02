<?php

if (isset($_GET["gpio2"])) {
    $file=fopen("esp8266_gpio2_status.txt", "w") or die("Unable to open file!");
    $gpio2_state = $_GET["gpio2"];
    fwrite($file, $gpio2_state);
    fclose($file);
    echo ($gpio2_state);
} else {
    $file=fopen("esp8266_gpio2_status.txt", "r") or die("Unable to open file!");
    echo fgets($file);
    fclose($file);	
}

?>
