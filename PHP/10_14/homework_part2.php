<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<pre>
<?php


$buffer = "ABC;123;xyz";
$a = explode(";", $buffer);
print_r($a);


echo "品項\tPAY\tCOST\t10\t5\t1\n";;
$fptr = fopen("input_small.txt", "r");
while (!feof($fptr)) {
    $buffer = fgets($fptr);
    $array=explode("\t",$buffer);
    $product=$array[0];
    $pay=(int)$array[1];
    $cost=(int)$array[2];
    $money=$pay-$cost;
    $ten=floor($money/10);
    $money=$money%10;
    $five=floor($money/5);
    $money=$money%5;
    $one=$money;
    echo $product."\t",$pay,"\t",$cost,"\t",$ten."\t".$five."\t".$one."\n";
}
fclose($fptr);

?>
</pre>
