<html>
    <head>
        <title>CIS 322 REST-api: Brevet list</title>
    </head>

    <body>
      <h1> Consuming ... </h1>
        <h2>List All - JSON</h2>
        <ul>
            <?php
            $json = file_get_contents('http://web:5000/listAll');
            echo $json;
            ?>
        </ul>
        <h2> List Open Only - JSON </h2>
        <ul>
            <?php
            $json = file_get_contents('http://web:5000/listOpenOnly');
            echo $json;
            ?>
        </ul>
        <h2> List Close Only - JSON </h2>
        <ul>
            <?php
            $json = file_get_contents('http://web:5000/listCloseOnly');
            echo $json;
            ?>
        </ul>
        <h2> First 3 Brevets Only - JSON </h2>
        <ul>
            <?php
            $json = file_get_contents('http://web:5000/listAll/json?top=3');
            echo $json;
            ?>
        </ul>
        <h2>List All - CSV</h2>
        <ul>
            <?php
            $csv = file_get_contents('http://web:5000/listAll/csv');
            echo $csv;
            ?>
        </ul>
        <h2> List Open Only - CSV </h2>
        <ul>
            <?php
            $csv = file_get_contents('http://web:5000/listOpenOnly/csv');
            echo $csv;
            ?>
        </ul>
        <h2> List Close Only - CSV </h2>
        <ul>
            <?php
            $csv = file_get_contents('http://web:5000/listCloseOnly/csv');
            echo $csv;
            ?>
        </ul>
        <h2> First 3 Brevets Only - CSV </h2>
        <ul>
            <?php
            $csv = file_get_contents('http://web:5000/listAll/csv?top=3');
            echo $csv;
            ?>
        </ul>




    </body>
</html>
