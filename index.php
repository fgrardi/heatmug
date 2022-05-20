<?php

?><!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <?php include_once('stylesheets.php'); ?>
    <link rel="stylesheet" href="custom.css">
    <title>Heatmug</title>
</head>
<body class="d-flex h-100 text-center text-white bg-dark vh-100">
    <div class="cover-container d-flex w-100 h-100 p-3 mx-auto flex-column">
        <header class="mb-auto py-2">
            <h3 class="justify-content-center mb-0">heatmug.</h3>
        </header>

        <main class="px-3">
            <h1 class="">Hot</h1>
            <p class="badge bg-success">Drink Monitor Activated</p>
            
            <p class="lead">
                <span class="">Your drink is getting cold!</span> 
                The current temperature of your mug is now
                <span class="">75°</span>. The temperature was last updated on 
                <span class="">11:45:33</span>
            </p>
            <div class="form-check form-switch d-flex justify-content-center">
                <input class="form-check-input" type="checkbox" role="switch" id="flexSwitchCheckChecked" checked>
                <span class="material-symbols-outlined ps-2">notifications</span>
            </div>
        </main>

        <footer class="mt-auto text-white-50">
            <p>Smart Technology project by <a href="https://getbootstrap.com/" class="text-white text-decoration-none">Ellen de Veth</a>, and <a
                    href="https://twitter.com/mdo" class="text-white text-decoration-none">Fien Gérardi</a>.</p>
        </footer>
    </div>

    <?php include_once('bootstrapJS.php'); ?>
</body>
</html>