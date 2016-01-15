<?php
if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
	$data = json_decode(file_get_contents("php://input"),true);
	print_r($data);

	echo $data["country"];

	try{
		$bdd = new PDO('mysql:host=localhost;dbname=MPI;charset=utf8', 'root', 'notsecure');
		// On ajoute une entrée dans la table jeux_video
		echo 'ROW ' + $bdd->exec('INSERT INTO TestMpi_tbl(TestMpi_x,TestMpi_y) VALUES(\''. $data["x"].'\',\''.$data["y"].'\')');

		echo 'INSERT INTO TestMpi_tbl(TestMpi_x,TestMpi_y) VALUES(\''+$data["x"]+'\',\''+$data["y"]+'\')';
	}
	catch (Exception $e)
	{
		die('Erreur : ' . $e->getMessage());
	}

	
	
	
	


  
}
  
  
else
{
	try{
		$bdd = new PDO('mysql:host=localhost;dbname=MPI;charset=utf8', 'root', 'notsecure');
		// On ajoute une entrée dans la table jeux_video
		echo 'ROW ' + $bdd->exec('INSERT INTO TestMpi_tbl( TestMpi_x , TestMpi_y ) VALUES(\'0\',\'0\')');
	}
	catch (Exception $e)
	{
		die('Erreur : ' . $e->getMessage());
	}
} 
  
  
/*city: "Luhansk"
city2: "Lynnwood"
country: "UA"
country2: "US"
countrycode: "UA"
countrycode2: "US"
cx: 793.3146236364234
cy: 81.91969361179596
datetime: "2016-01-13&ensp;09:32:03.53"
dport: 3391
id: 4
latitude: 48.58
latitude2: 47.8
longitude: 39.33
longitude2: -122.28
md5: "46.161.40.90"
org: "Net For Ankas"
service: "savant"
svc: "3391"
targetX: 340.0233364286332
targetY: 85.2014732036134
type: "ipviking.honey"
x: 793.3146236364234
y: 81.91969361179596
*/
  
?>



