<?php

use TeaOpenPTY\TeaOpenPTY;

$app  = "/usr/bin/bashx";
$argv = ["/usr/bin/bash", "-i"];

$teaOpenPTY = new TeaOpenPTY($app);

echo "Starting TeaOpenPTY...\n";

$ret = $teaOpenPTY->exec(...$argv);

if ($ret === -1) {
  echo "Error: ", $teaOpenPTY->error(), "\n";
}

echo "TeaOpenPTY terminated!\n";
