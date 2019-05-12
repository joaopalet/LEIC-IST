<html>
    <head>
        <meta charset="UTF-8">
        <link rel="icon" type="image/png" href= "http://www.pngall.com/wp-content/uploads/2016/04/Database-PNG.png">
        <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.5.0/css/all.css" integrity="sha384-B4dIYHKNBt8Bc12p+WXckhzcICo0wtJAoU8YZTY5qE0Id1GSseTk6S+L3BlXeVIU" crossorigin="anonymous">
        <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css" integrity="sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO" crossorigin="anonymous">
        <title>Base de Dados</title>
        <style>
            body{
                background-color: #474747;
            }
            h3{
                color: white;
                margin-top: 30;
                text-align: center;
            }
            h6{
                color: white;
                text-align: center;
                margin-left: 130;
                margin-right: 130;
            }
            img{
                cursor: pointer;
            }
            a{
                margin: 1;
            }
            table{
                color: white;
                margin: 0 auto;
            }
            .centered{
                margin: 20 auto;
            }
            .leftie{
                margin: 10 10;
            }
        </style>
    </head>
    <body>

        <div class="leftie">
            <a href='.'><button type="button" class="btn btn-primary">Back</button></a>
        </div>
        
        <div class="centered">
            <h3>Meios</h3>
        </div>

        <div class="container">
            <table class="table col-md-8">
                <thead class="thead-dark">
                <tr>
                    <th style='text-align:center' scope="col">Nº</th>
                    <th style='text-align:center' scope="col">Nome</th>
                    <th style='text-align:center' scope="col">Entidade</th>
                </tr>
                </thead>
                <tbody>
                <?php

                $host = "db.ist.utl.pt";
                $user ="ist186512";
                $password = "fico6299";
                $dbname = $user;
            
                $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
                $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
            
                $sql = "SELECT nummeio, nomemeio, nomeentidade FROM meio;";
                $result = $db->prepare($sql);
                $result->execute();

                $count = 0;
                foreach($result as $row)
                {
                    echo("<tr>");
                    echo("<td style='text-align:center'>");
                    echo($row['nummeio']);
                    echo("</td>");
                    echo("<td style='text-align:center'>");
                    echo($row['nomemeio']);
                    echo("</td>");
                    echo("<td style='text-align:center'>");
                    echo($row['nomeentidade']);
                    echo("</td>");
                    echo("<tr>");
                    $count = $count + 1;
                }
        
                $db = null;
                ?>
                </tbody>
                
            </table>
        </div>
            
    </body>
</html>
