
<?php
if(!$connection = ssh2_connect('localhost', 22)) die('Connection failed.');
if(!ssh2_auth_password($connection, 'test', 'hogehoge')) die('Authentication Failed...');
if(!$stdio = ssh2_shell($connection, 'xterm')) die('Shell access failed.');


$command = "/bin/ls /tmp\n";

fwrite($stdio,$command);
sleep(1);

while($line = fgets($stdio)) {
  flush();
  echo $line."\n";
}



$command = "/bin/ls /usr\n";

fwrite($stdio,$command);
sleep(1);

while($line = fgets($stdio)) {
  flush();
  echo $line."\n";
}

fclose($stdio);



?>
