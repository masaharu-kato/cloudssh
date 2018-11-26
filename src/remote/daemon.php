<?php
    require_once __DIR__.'/connect_database.php';

//    echo "Connecting to database...\n";
    $pdo = connect_database();
//    echo "Database connection established.\n";

    $ist = $pdo->prepare('SELECT * FROM inputs');
    
    $ost = $pdo->prepare("INSERT INTO outputs VALUES(null, :input_id, :content)");
        
    $dst = $pdo->prepare("DELETE FROM inputs WHERE id = :id");

    while(true){

        $ist->execute();

        while($row = $ist->fetch()){
            $input_id = $row['id'];
            $out = [];
            exec($row['command'], $out);
            $ost->execute(['input_id' => $input_id, 'content' => implode("\n", $out)]);
            $dst->execute(['id' => $input_id]);
            echo '#';
        }

        sleep(1);
        echo '.';
    }

    echo "Done.\n";
?>