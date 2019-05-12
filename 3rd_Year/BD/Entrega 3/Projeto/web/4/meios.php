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
            <h3>Inserir Meio e Processo de Socorro a associar</h3>
            <form action='meios.php' method='post'>
                <h6>Nº Meio: <input type='number' name='nMeio' min='0' required='required'/></h6>
                <h6>Entidade: <input type='text' name='entidade' required='required'/></h6>
                <h6>Nº Processo: <input type='number' name='nProcesso' min='0' required='required'/></h6>
                <h6><input class="btn btn-success" type="submit" value="Submit"></h6>
            </form>
        </div>

        <?php 

        if(isset($_REQUEST['nMeio'])){
            $nMeio     = $_REQUEST['nMeio'];
            $entidade  = $_REQUEST['entidade'];
            $nProcesso = $_REQUEST['nProcesso'];

            $host = "db.ist.utl.pt";
            $user ="ist186512";
            $password = "fico6299";
            $dbname = $user;
        
            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            $sql = "SELECT * FROM processosocorro WHERE numprocessosocorro = :nP;";
        
            $result = $db->prepare($sql);
            $result->execute([':nP'=> $nProcesso]);

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
    
                $sql = "SELECT * FROM meio WHERE nummeio = :nM AND nomeentidade = :nEntidade;";
            
                $result = $db->prepare($sql);
                $result->execute([':nM'=> $nMeio, ':nEntidade'=>$entidade,]);
    
                $process = $result->fetchAll();
            
                $db = null;

                if(sizeOf($process)==0){
                    echo("<div class='centered'><h6>ERRO: Meio não existe</h6></div>");
                }else{
                    try
                    {
                        $host = "db.ist.utl.pt";
                        $user ="ist186512";
                        $password = "fico6299";
                        $dbname = $user;
                    
                        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
                        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        
        
                    
                        $sql = "INSERT INTO acciona (nummeio, nomeentidade, numprocessosocorro) VALUES (:nM, :nEntidade, :nP);";
                    
                        $result = $db->prepare($sql);
                        $result->execute([':nM'=> $nMeio, ':nEntidade'=>$entidade, ':nP'=> $nProcesso]);
                    
                        $db = null;
        
                        header("Refresh:0");
                    }
                    catch (PDOException $e)
                    {
                        echo("<div class='centered'><h6>ERRO: O Processo já está associado ao Meio</h6></div>");
                    }
                }
            }
        }

        ?>

        <div class="container">
            
            <table class="table col-md-7">
                <thead class="thead-dark">
                <tr>
                    <th style='text-align:center'  scope="col">Nº Meio</th>
                    <th style='text-align:center'  scope="col">Entidade</th>
                    <th style='text-align:center'  scope="col">Nº Processo Socorro</th>
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
            
                $sql = "SELECT * FROM acciona ORDER BY numprocessosocorro, nummeio, nomeentidade ASC;";

                $result = $db->prepare($sql);
                $result->execute();

                foreach($result as $row)
                {
                    echo("<tr>");
                    echo("<td style='text-align:center' >");
                    echo($row['nummeio']);
                    echo("</td>");
                    echo("<td style='text-align:center' >");
                    echo($row['nomeentidade']);
                    echo("</td>");
                    echo("<td style='text-align:center' >");
                    echo($row['numprocessosocorro']);
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
