
<?php

    if(!$con_local = ssh2_connect(
        'localhost', 22, [], [
            'ignore' => function(){ echo "Ignored.\n"; },
            'debug' => function(){ echo "debug.\n"; },
            'macerror' => function(){ echo "macerror.\n"; },
            'disconnect' => function ($reason, $message, $language) {
                echo "Disconnected.\n";
                echo "Reason：$reason\n";
                echo "Message：$message\n";
                echo "Language：$language\n";
            }
        ]
        
    )){
        die('SSH Connection failed.');
    }

    echo "username: "; $username = trim(fgets(STDIN));
    echo "password: "; $password = trim(`stty -echo;read PASSWORD;echo \$PASSWORD`);

    // $username = 'kato';
    // $password = '************';
    // `stty -echo;`;

    if(!ssh2_auth_password($con_local, $username, $password)){
        die('Authentication Failed...');
    }

    if(!$stdio = ssh2_shell($con_local, 'xterm')){
        die('Shell access failed.');
    }

    `stty -icanon`;


    while (true){

    //    if(!is_resource($con_local)) break;
        echo stream_get_contents($stdio);
    //     $out = stream_get_contents($stdio);
    // //    echo $out;
    //     var_dump($out);

    //     echo "--begin--\n";
    //     foreach(str_split($out) as $c){
    //         echo "$c (".ord($c).")\n";
    //     }
    //     echo "--end--\n";

        $input = fgetc(STDIN);
        if($input !== false){
            fwrite($stdio, $input);
        }
        usleep(10 * 1000);
    }
    echo "CloudSSH closed.\n";
    `stty echo`;
    `stty icanon`;
    exit;
?>
