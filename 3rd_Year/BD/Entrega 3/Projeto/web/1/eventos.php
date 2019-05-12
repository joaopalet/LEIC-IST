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
            <h3>Inserir Evento de Emergência</h3>
            <form action='eventos.php' method='post'>
                <h6>Telefone: <input type='text' name='tlfn' required='required'/></h6>
                <h6>Instante de Chamada: <input type='datetime-local' step='1' name='instante' required='required'/></h6>
                <h6>Nome da Pessoa: <input type='text' name='nome' required='required'/></h6>
                <h6>Local: <input type='text' name='morada' required='required'/></h6>
                <h6>Nº Processo: <input type='number' name='n' min='0' required='required'/></h6>
                <h6><input class="btn btn-success" type="submit" value="Submit"></h6>
            </form>
        </div>

        <?php 

        if(isset($_REQUEST['tlfn']) and isset($_REQUEST['instante']) and isset($_REQUEST['nome']) and isset($_REQUEST['morada']) and isset($_REQUEST['n'])){
        
            $tlfn     = $_REQUEST['tlfn']; 
            $instante = str_replace("T"," ",$_REQUEST['instante']);
            $nome     = $_REQUEST['nome'];
            $morada   = $_REQUEST['morada'];
            $num      = $_REQUEST['n'];

            $host = "db.ist.utl.pt";
            $user ="ist186512";
            $password = "fico6299";
            $dbname = $user;
        
            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            $sql = "SELECT * FROM local WHERE moradalocal = :morada;";
        
            $result = $db->prepare($sql);
            $result->execute([':morada'=> $morada]);

            $process = $result->fetchAll();
        
            $db = null;

            if(sizeOf($process)==0){
                echo("<div class='centered'><h6>ERRO: Local não existe</h6></div>");
            }else{
                $host = "db.ist.utl.pt";
                $user ="ist186512";
                $password = "fico6299";
                $dbname = $user;
            
                $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
                $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    
                $sql = "SELECT * FROM processosocorro WHERE numprocessosocorro = :n;";
            
                $result = $db->prepare($sql);
                $result->execute([':n'=> $num]);
    
                $process = $result->fetchAll();
            
                $db = null;
    
                if(sizeOf($process)==0){
                    $host = "db.ist.utl.pt";
                    $user ="ist186512";
                    $password = "fico6299";
                    $dbname = $user;
    
                    $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
                    $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    
                    $sql = "INSERT INTO processosocorro (numprocessosocorro) VALUES (:n);";
    
                    $result = $db->prepare($sql);
                    $result->execute([':n'=> $num]);
    
                    $db = null;
                }

                try{
                $host = "db.ist.utl.pt";
                $user ="ist186512";
                $password = "fico6299";
                $dbname = $user;
    
                $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
                $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    
                $sql = "INSERT INTO eventoemergencia (numtelefone, instantechamada, nomepessoa, moradalocal, numprocessosocorro) VALUES (:tlfn,:inst,:nome,:morada,:n);";
    
                $result = $db->prepare($sql);
                $result->execute([':tlfn' => $tlfn, ':inst' => $instante, ':nome' => $nome, ':morada' => $morada, ':n' => $num]);
    
                $db = null;
    
                header("Refresh:0");
                }
                catch (PDOException $e)
                {
                    echo("<div class='centered'><h6>ERRO: Evento já existe</h6></div>");
                }
            }
        }

        if(isset($_REQUEST['remNumTel'])){
          
            $apagarNumTel    = $_REQUEST['remNumTel'];
            $apagarInstante  = $_REQUEST['remInstante'];

            $host = "db.ist.utl.pt";
            $user ="ist186512";
            $password = "fico6299";
            $dbname = $user;
        
            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        
            $sql = "DELETE FROM eventoemergencia WHERE numtelefone = :apagarNumTel AND instantechamada = :apagarInstante;";

            $result = $db->prepare($sql);
            $result->execute([':apagarNumTel' => $apagarNumTel, 'apagarInstante' => $apagarInstante]);
        
            $db = null;

            $newURL = 'eventos.php';
            header('Location: '.$newURL);

        }

        ?>


        <div class="container">
            <table class="table col-md-12">
                <thead class="thead-dark">
                <tr>
                    <th style='text-align:center' scope="col">Telefone</th>
                    <th style='text-align:center' scope="col">Instante</th>
                    <th style='text-align:center' scope="col">Nome</th>
                    <th style='text-align:center' scope="col">Morada</th>
                    <th style='text-align:center' scope="col">Nº Processo</th>
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
            
                $sql = "SELECT numtelefone, instantechamada, nomepessoa, moradalocal, numprocessosocorro FROM eventoemergencia ORDER BY numprocessosocorro ASC;";
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
                    echo("<td style='text-align:center'>");
                    echo("<a href='eventos.php?remNumTel={$row['numtelefone']}&remInstante={$row['instantechamada']}'><img width='20' src='https://goo.gl/uJnJJD'>");
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
