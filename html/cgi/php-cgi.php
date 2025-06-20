#!/usr/bin/php-cgi
<?php
echo "Content-Type: text/html; charset=UTF-8" . "\r\n";
echo "\r\n";
date_default_timezone_set('Asia/Tokyo');
echo "現在時刻: " . date("Y-m-d H:i:s") . "<br>";
?>
