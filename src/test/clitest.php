<?php
system("stty -icanon");
stream_set_blocking(STDIN, false);

$line = '';

$time = microtime(true);

$prompt = '> ';

echo $prompt;

while (true)
{
  if (microtime(true) - $time > 5)
  {
    echo "\nTick...\n$prompt$line";
    $time = microtime(true);
  }

  $c = fgetc(STDIN);
  if ($c !== false)
  {
    if ($c != "\n") {
        echo "You Entered $c (".ord($c).") \n";
      $line .= $c;
    }
    else{
      if ($line == 'exit' || $line == 'quit')
        break;
      else if ($line == 'help')
        echo "Type exit\n";
      else
        echo "Unrecognized command.\n";

      echo $prompt;
      $line = '';
    }
  }
}