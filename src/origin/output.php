<?php
    require_once __DIR__.'/../connect_database.php';

//    echo "Connecting to database...\n";
    $pdo = connect_database();
//    echo "Database connection established.\n";

    $ost = $pdo->query('SELECT * FROM outputs');
    $data = $ost->fetchAll();
    var_dump($data);

?>