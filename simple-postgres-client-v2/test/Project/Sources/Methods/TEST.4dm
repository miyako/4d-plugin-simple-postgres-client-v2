//%attributes = {}
  //most easy
$connect:="user=miyako dbname=postgres"
$SQL:="SELECT * FROM users"

$status:=PQ EXECUTE ($connect;$SQL)

