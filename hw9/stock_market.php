<?php 
$link = mysql_connect('localhost', 'root', '', 'stock_market');

echo "   ";
echo "<table>";
$cols = mysql_num_fields($rows);
echo "<tr>"
for ($col=0; $col < $cols; $col++){
	echo "<th>";
	echo mysql_field_name($row, $col);
	echo "</th";
}
echo "</tr>";

while ($row = mysql_fetch_array($rows)){
	echo "<tr>";
	for ($col=0; $col < $cols; $col++){
		echo "<td>";
		echo $row[$col];
		echo "</td>";
	}
	echo "</tr>";
}
echo "</table>";
mysql_close($link);
?>

<html>
 <head>
  <title>PHP Test</title>
 </head>
 <body>
 </body>
</html>