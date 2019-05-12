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
            <a href='index.html'><button type="button" class="btn btn-primary">Back</button></a>
        </div>
        
        <div class="centered">
            <h3>Inserir Evento e Processo de Socorro a associar</h3>
            <form action='eventos.php' method='post'>
                <h6>Telefone: <input type='text' name='tele' required='required'/></h6>
                <h6>Instante de Chamada: <input type='datetime-local' step='1' name='instante' required='required'/></h6>
                <h6>Nº Processo: <input type='number' name='processo' min='0' required='required'/></h6>
                <h6><input class="btn btn-success" type="submit" value="Submit"></h6>
            </form>
        </div>

        <?php 

        if(isset($_REQUEST['tele'])){
            $tele     = $_REQUEST['tele'];
            $instante = str_replace("T"," ",$_REQUEST['instante']);
            $processo = $_REQUEST['processo'];

            $host = "db.ist.utl.pt";
            $user ="ist186512";
            $password = "fico6299";
            $dbname = $user;
        
            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            $sql = "SELECT * FROM processosocorro WHERE numprocessosocorro = :processo;";
        
            $result = $db->prepare($sql);
            $result->execute([':processo'=> $processo]);

            $process = $result->fetchAll();
        
            $db = null;

            if(sizeOf($process)==0){
                echo("<div class='centered'><h6>ERRO: Processo não existe</h6></div>");
            }else{
                $host = "db.ist.utl.pt";
                $user ="ist186512";
                $password = "fico6299";
                $dbname = $user;
            
                $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
                $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

                $sql = "SELECT * FROM eventoemergencia WHERE numtelefone = :tele AND instantechamada = :instante;";

                $result = $db->prepare($sql);
                $result->execute([':tele'=> $tele, ':instante'=> $instante]);
    
                $process = $result->fetchAll();
            
                $db = null;
                if(sizeOf($process)==0){
                    echo("<div class='centered'><h6>ERRO: Evento não existe</h6></div>");
                }else{
                    $host = "db.ist.utl.pt";
                    $user ="ist186512";
                    $password = "fico6299";
                    $dbname = $user;
                
                    $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
                    $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
                
                    $sql = "UPDATE eventoemergencia SET numprocessosocorro = :processo WHERE instantechamada = :instante AND numtelefone = :tele;";
                
                    $result = $db->prepare($sql);
                    $result->execute([':processo'=> $processo, ':instante' => $instante, ':tele' => $tele]);
                
                    $db = null;
    
                    header("Refresh:0");
                }
            }
        }    

        ?>

        <div class="container">
            
            <table class="table col-md-8">
                <thead class="thead-dark">
                <tr>
                <th style='text-align:center' scope="col">Telefone</th>
                    <th style='text-align:center' scope="col">Instante</th>
                    <th style='text-align:center' scope="col">Nome</th>
                    <th style='text-align:center' scope="col">Morada</th>
                    <th style='text-align:center' scope="col">Nº Processo</th>
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
            
                $sql = "SELECT * FROM eventoemergencia ORDER BY instantechamada ASC;";

                $result = $db->prepare($sql);
                $result->execute();

                foreach($result as $row)
                {
                    echo("<tr>");
                    echo("<td style='text-align:center'>");
                    echo($row['numtelefone']);
                    echo("</td>");
                    echo("<td style='text-align:center'>");
                    echo($row['instantechamada']);
                    echo("</td>");
                    echo("<td style='text-align:center'>");
                    echo($row['nomepessoa']);
                    echo("</td>");
                    echo("<td style='text-align:center'>");
                    echo($row['moradalocal']);
                    echo("</td>");
                    echo("<td style='text-align:center'>");
                    echo($row['numprocessosocorro']);
                    echo("</td>");
                }
        
                $db = null;
                ?>
                </tbody>
                
            </table>
        </div>
            
    </body>
</html>
