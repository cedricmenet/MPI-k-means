<?php 
// on se connecte à MySQL 
$db = mysql_connect('localhost', 'root', 'notsecure'); 

// on sélectionne la base 
mysql_select_db('MPI',$db); 

// on crée la requête SQL 
$sql = 'SELECT * from MpiResult_tbl'; 

// on envoie la requête 
$req = mysql_query($sql) or die('Erreur SQL !<br>'.$sql.'<br>'.mysql_error()); 


$json = array();

// on fait une boucle qui va faire un tour pour chaque enregistrement
while($data = mysql_fetch_assoc($req))
	array_push($json, $data);
	
echo json_encode($json);

// on ferme la connexion à mysql 
mysql_close(); 
?> 