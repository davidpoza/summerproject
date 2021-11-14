<?php
define('USER', '***');
define('PASSWORD', '*******');
define('HOST', '**********');
define('DATABASE', 'wimc');

try {
    $connection = new PDO("mysql:host=".HOST.";dbname=".DATABASE, USER, PASSWORD);
} catch (PDOException $e) {
    //echo '<p> no conecta </p>';
    exit("Error: " . $e->getMessage());
}
?>
