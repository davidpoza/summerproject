<?php
include 'config.php';

function jsonResponse($data, $msg)
{
  header('Content-Type: application/json');
  echo json_encode(['data:', $data]);
}

function isAuth($user, $password)
{
    $user = $_GET['user'];
    $password = $_GET['password'];

    $query = $connection->prepare("SELECT * FROM users WHERE user=:user");
    $query->bindParam("user", $user, PDO::PARAM_STR);
    $query->execute();
    $result = $query->fetch(PDO::FETCH_ASSOC);

}

?>
