<?php
    require_once __DIR__.'/connect_database.php';

    $pdo = connect_database();
    $ist = $pdo->prepare("INSERT INTO inputs VALUES(null, :command)");
    $ost = $pdo->prepare("SELECT * FROM outputs");
    $dst = $pdo->prepare("DELETE FROM outputs WHERE id = :id");

    $is_input = true;

    while(true){
        if($is_input) {
            echo '>';
            $stdin = trim(fgets(STDIN));
            if ($stdin === '') return;
            $ist->execute(['command' => $stdin]);
            $is_input = false;
        }
        else{
            $ost->execute();
            $outputs = $ost->fetchAll();
            if(!count($outputs)) {
                usleep(500000);   // sleep 0.5sec
            }
            else {
                foreach($outputs as $row) {
                    echo $row['content']."\n";
                    $dst->execute(['id' => $row['id']]);
                }
                $is_input = true;
            }
        }
    }
?>