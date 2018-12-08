
<?php
    echo "username: "; $username = trim(fgets(STDIN));
    echo "password: "; $password = trim(`stty -echo;read PASSWORD;echo \$PASSWORD`);

    echo "\n";
    echo "username: >$username<\n";
    echo "password: >$password<\n";

    `stty -icanon`;

    while (true){
        // $c = fgetc(STDIN);
        // if($c !== false) echo $c;
        $line = fgets(STDIN);
        if($line !== false) echo $line."\n";
        usleep(10 * 1000);
    }

    // `stty icanon`;
    // `stty echo;`
?>