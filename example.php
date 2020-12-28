<?php

use TeaOpenPTY\TeaOpenPTY;

$app  = "/usr/bin/bash";
$argv = [$app, "-i"];

$teaOpenPTY = new TeaOpenPTY($app);
$teaOpenPTY->exec(...$argv);
var_dump($teaOpenPTY);