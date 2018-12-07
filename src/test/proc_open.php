<?php

$process = proc_open(
    'ssh arena',
    [
        0 => ["pipe", "r"],  // stdin は、子プロセスが読み込むパイプです。
        1 => ["pipe", "w"],  // stdout は、子プロセスが書き込むパイプです。
        2 => ["file", "/tmp/error-output.txt", "a"], // はファイルで、そこに書き込みます。
    ],
    $pipes, //  入出力パイプを入れる変数
    '/',    //  初期ディレクトリ
    [       //  追加の環境変数
         // 'some_option' => 'aeiou',
    ]
);

if (is_resource($process)) {
    // $pipes はこの時点で次のような形を取っています。
    // 0 => 子プロセスの stdin に繋がれた書き込み可能なハンドル
    // 1 => 子プロセスの stdout に繋がれた読み込み可能なハンドル
    // すべてのエラー出力は /tmp/error-output.txt に書き込みされます。

    fwrite($pipes[0], 'ls -a');
    fclose($pipes[0]);

    echo stream_get_contents($pipes[1]);
    fclose($pipes[1]);

    // デッドロックを避けるため、proc_close を呼ぶ前に
    // すべてのパイプを閉じることが重要です。
    $return_value = proc_close($process);

    echo "command returned $return_value\n";
}
?>
