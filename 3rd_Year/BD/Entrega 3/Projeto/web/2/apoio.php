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
            <h3>Inserir Meio de Apoio</h3>
            <form action='apoio.php' method='post'>
                <h6>Nº: <input type='number' name='n' min='0' required='required'/></h6>
                <h6>Nome: <input type='text' name='nome' required='required'/></h6>
                <h6>Entidade: <input type='text' name='entidade' required='required'/></h6>
                <h6><input class="btn btn-success" type="submit" value="Submit"/></h6>
            </form>
        </div>

        <?php 

        if(isset($_REQUEST['n'])){
            try
            {           
                $num   = $_REQUEST['n'];
                $ent   = $_REQUEST['entidade'];

                $host = "db.ist.utl.pt";
                $user ="ist186512";
                $password = "fico6299";
                $dbname = $user;
            
                $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
                $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
            
                $sql = "INSERT INTO meioapoio (nummeio,nomeentidade) VALUES (:num,:entidade);";
            
                $result = $db->prepare($sql);
                $result->execute([':num'=>$num, ':entidade'=>$ent]);
            
                $db = null;

                header("Refresh:0");
            }
            catch (PDOException $e)
            {
                $type = $e->getCode();
                if($type == 23505) echo("<div class='centered'><h6>ERRO: Meio existe e já é de apoio</h6></div>");
                else if($type == 23503){
                    try{
                        $name  = $_REQUEST['nome'];

                        $host = "db.ist.utl.pt";
                        $user ="ist186512";
                        $password = "fico6299";
                        $dbname = $user;
                    
                        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
                        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
                    
                        $sql = "INSERT INTO meio (nummeio,nomemeio,nomeentidade) VALUES (:num,:nome,:entidade);";
                    
                        $result = $db->prepare($sql);
                        $result->execute([':num'=>$num, ':nome'=>$name, ':entidade'=>$ent]);
                    
                        $db = null;

                        $host = "db.ist.utl.pt";
                        $user ="ist186512";
                        $password = "fico6299";
                        $dbname = $user;
                    
                        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
                        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
                    
                        $sql = "INSERT INTO meioapoio (nummeio,nomeentidade) VALUES (:num,:entidade);";
                    
                        $result = $db->prepare($sql);
                        $result->execute([':num'=>$num, ':entidade'=>$ent]);
                    
                        $db = null;

                        header("Refresh:0");
                    }
                    catch (PDOException $e)
                    {
                        $type = $e->getCode();
                        if($type == 23505) echo("<div class='centered'><h6>ERRO: Meio já existe</h6></div>");
                        else if($type == 23503) echo("<div class='centered'><h6>ERRO: Entidade não existe</h6></div>");
                    }
                }
            }
        }
        

        if(isset($_REQUEST['remN'])){
          
            $apagarN    = $_REQUEST['remN'];
            $apagarEnt  = $_REQUEST['remEnt'];    

            echo($apagarN);
            echo($apagarEnt);

            $host = "db.ist.utl.pt";
            $user ="ist186512";
            $password = "fico6299";
            $dbname = $user;
        
            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        
            $sql = "DELETE FROM meioapoio WHERE nummeio = :apagarN AND nomeentidade = :apagarEntidade;";

            $result = $db->prepare($sql);
            $result->execute([':apagarN' => $apagarN, 'apagarEntidade' => $apagarEnt]);
        
            $db = null;

            $newURL = 'apoio.php';
            header('Location: '.$newURL);

        }

        ?>


        <div class="container">
            <table class="table col-md-8">
                <thead class="thead-dark">
                <tr>
                    <th style='text-align:center' scope="col">Nº</th>
                    <th style='text-align:center' scope="col">Nome</th>
                    <th style='text-align:center' scope="col">Entidade</th>
                    <th style='text-align:center' scope="col">Editar</th>
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
            
                $sql = "SELECT nummeio, nomeentidade, nomemeio FROM meioapoio NATURAL JOIN meio ORDER BY nomeentidade, nummeio ASC;";
                $result = $db->prepare($sql);
                $result->execute();

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
                    echo("<td style='text-align:center'>");
                    echo("<a href='editApoio.php?num={$row['nummeio']}&nome={$row['nomemeio']}&entidade={$row['nomeentidade']}'><img width='20' src='https://goo.gl/9tGAh8'></a>");
                    echo("</td>");
                    echo("</td>");
                    echo("<td style='text-align:center'>");
                    echo("<a href='apoio.php?remN={$row['nummeio']}&remEnt={$row['nomeentidade']}'><img width='20' src='https://goo.gl/uJnJJD'></a>");
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
