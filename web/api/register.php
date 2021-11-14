<?php

error_reporting(E_ALL);
ini_set("display_errors", 1);

include('config.php');

if (isset($_POST['register'])) {

    $user = $_POST['user'];
    //$id = $_POST['id'];
    $password = $_POST['password'];
    $password_hash = password_hash($password, PASSWORD_BCRYPT);

    $query = $connection->prepare("SELECT * FROM users WHERE USER=:user");
    $query->bindParam("user", $user, PDO::PARAM_STR);
    $query->execute();

    if ($query->rowCount() > 0) {
        echo '<p class="error">This user is already registered</p>';
    }

    if ($query->rowCount() == 0) {
        $query = $connection->prepare("INSERT INTO users(USER,PASSWORD) VALUES (:user,:password_hash)");
        $query->bindParam("user", $user, PDO::PARAM_STR);
        $query->bindParam("password_hash", $password_hash, PDO::PARAM_STR);
        //$query->bindParam("id", $id, PDO::PARAM_STR);
        $result = $query->execute();

        if ($result) {
            echo '<p class="success">Your registration was successful!</p>';
        } else {
            echo '<p> Error </p>';
        }
    }
}
?>
<h1> -- Añadir Cuenta -- </h1>
<form method="post" action="" name="signup-form">
    <div class="form-element">
        <label>Username</label>
        <input type="text" name="user" pattern="[a-zA-Z0-9]+" required />
    </div>
    </div>
    <div class="form-element">
        <label>Password</label>
        <input type="password" name="password" required />
    </div>
    <button type="submit" name="register" value="register">Register</button>
</form>
