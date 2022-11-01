<pre>
<?php
    $dbhost = '127.0.0.1'; #MySQL IP 
    $dbuser = 'root'; #帳號 
    $dbpass = ''; 
    $dbname = 'buying_db'; #資料庫名稱
    $conn = mysqli_connect($dbhost, $dbuser, $dbpass) or die('Error with MySQL connection'); 
    mysqli_select_db($conn, $dbname);
    print_r($_POST);
    $item=$_POST["item"];
    $pay=$_POST["pay"];
    $cost=$_POST["cost"];
    if($pay>=$cost&&$pay!=NULL&&$cost!=NULL){
        echo    "Pass!";
        mysqli_query($conn, "INSERT INTO buying_items (item, pay, cost)
        VALUES ('$item','$pay', '$cost');");
    }
    else{
        die("Error:Pay<Cost!");
    }
?>
</pre>
