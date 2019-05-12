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
            <h3>Inserir Processo</h3>
            <form action='processos.php' method='post'>
                <h6>Número: <input type='number' name='n' min='0' required='required'/></h6>
                <h6><input class="btn btn-success" type="submit" value="Submit"/></h6>
            </form>
        </div>

        <?php 

        if(isset($_REQUEST['n'])){
            try
            {       
                $novo_n = $_REQUEST['n'];    

                $host = "db.ist.utl.pt";
                $user ="ist186512";
                $password = "fico6299";
                $dbname = $user;

                $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
                $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

                $sql = "INSERT INTO processosocorro (numprocessosocorro) VALUES (:novo_n);";

                $result = $db->prepare($sql);
                $result->execute([':novo_n'=> $novo_n]);

                $db = null;

                header("Refresh:0");
            }
            catch (PDOException $e)
            {
                echo("<div class='centered'><h6>ERRO: Processo já existe</h6></div>");
            }
        }

        if(isset($_REQUEST['rem'])){
            $apagar = $_REQUEST['rem'];

            $host = "db.ist.utl.pt";
            $user ="ist186512";
            $password = "fico6299";
            $dbname = $user;

            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            $sql = "DELETE FROM processosocorro WHERE numprocessosocorro = :apagar;";

            $result = $db->prepare($sql);
            $result->execute([':apagar'=> $apagar]);

            echo('entrei');

            $db = null;

            $newURL = 'processos.php';
            header('Location: '.$newURL);

        }
        ?>

        <div class="container">
            <table class="table col-md-6">
                <thead class="thead-dark">
                <tr>
                    <th style='text-align:center' scope="col">Número</th>
                    <th style='text-align:center' scope="col">Remover</th>
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
            
                $sql = "SELECT numprocessosocorro FROM processosocorro ORDER BY numprocessosocorro ASC;";
                $result = $db->prepare($sql);
                $result->execute();

                foreach($result as $row)
                {
                    echo("<tr>");
                    echo("<td style='text-align:center'>");
                    echo($row['numprocessosocorro']);
                    echo("</td>");
                    echo("<td style='text-align:center'>");
                    echo("<a href='processos.php?rem={$row['numprocessosocorro']}'><img width='20' src='https://goo.gl/uJnJJD'>");
                    echo("</td>");
                    echo("<tr>");
                }
        
                $db = null;
                ?>
                </tbody>
                
            </table>
        </div>
            
    </body>
</html>
