<?php
    $dbhost = '127.0.0.1'; #MySQL IP 
    $dbuser = 'root'; #帳號 
    $dbpass = ''; 
    $dbname = 'buying_db'; #資料庫名稱
    $conn = mysqli_connect($dbhost, $dbuser, $dbpass) or die('Error with MySQL connection'); 
    mysqli_query($conn, "SET NAMES 'utf8'"); 
    // mysqli_select_db($conn, $dbname); 
    mysqli_query($conn, "CREATE DATABASE buying_db ");
    mysqli_select_db($conn, $dbname);
    mysqli_query($conn, "
    CREATE TABLE buying_items  (
        id int NOT NULL AUTO_INCREMENT,
        item varchar(50),
        pay int,
        cost int,
        PRIMARY KEY (id)
      );
    ");
    $fptr = fopen("input_large.txt", "r");
    $count=0;
    while (!feof($fptr)) {
        $buffer = trim(fgets($fptr));
        $array=explode("\t",$buffer);
        $item=addslashes($array[0]);
        $pay=(int)$array[1];
        $cost=(int)$array[2];
        // echo $item."\t".$pay."\t".$cost."\n";
        mysqli_query($conn, "INSERT INTO buying_items (item, pay, cost)
        VALUES ('$item','$pay', '$cost');");
        $count++;
    }
    // echo $count."\n";
    fclose($fptr);
?>