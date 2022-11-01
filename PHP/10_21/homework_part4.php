<pre>
<?php
    $dbhost = '127.0.0.1'; #MySQL IP 
    $dbuser = 'root'; #帳號 
    $dbpass = ''; 
    $dbname = 'buying_db'; #資料庫名稱
    $conn = mysqli_connect($dbhost, $dbuser, $dbpass) or die('Error with MySQL connection'); 
    mysqli_query($conn, "SET NAMES 'utf8'"); 
    // mysqli_select_db($conn, $dbname); 
    mysqli_select_db($conn, $dbname);
    $result=mysqli_query($conn, "SELECT item,pay,cost FROM buying_items") or die('MySQL query error'); 
    echo "Type\t10\t5\t1\n";
    while($row = mysqli_fetch_array($result)){ 
        $item=$row['item']; 
        $pay=$row['pay']; 
        $cost=$row['cost'];
        $money=$pay-$cost;
        $ten=floor($money/10);
        $money=$money%10;
        $five=floor($money/5);
        $money=$money%5;
        $one=$money;
        #=============================================
        echo $item."\t".$ten."\t".$five."\t".$one."\n";
    }
?>
</pre>