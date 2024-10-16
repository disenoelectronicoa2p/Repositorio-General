<?php

        // Conexión a la base de datos
        $servername = "fdb1023.awardspace.net";   
        $username = "";           
        $password = "";                 
        $dbname = "";           
           

        //Se crea la conexion
        $conn = mysqli_connect($servername, $username, $password, $dbname);
        if (!$conn) {
            ResponseHandler::error('Database connection failed: ' . mysqli_connect_error());
        }

        //Se obtiene el cuerpo de la peticion 
        $json = file_get_contents('php://input');
        $data = json_decode($json, true);
        $humedad =$data['humidity'];
        $temperatura =$data['temperature'];

        //Se obtiene la fecha y hora actual, con zona horaria "America/Bogota"
        $date = new DateTime("now", new DateTimeZone('America/Bogota'));
        $current = $date->format('Y-m-d H:i:s');


        //Se crea la sql para insertar los datos en la tabla plant_humidity_data
        $sql = "INSERT INTO plant_humidity_data (humedad,temperatura,date) VALUES ($humedad, $temperatura, '$current')";

        //Respuestra estructurada en formato json
        $responseOk = [ 'code' => 200, 'description' => 'Data saved successfully' ];
        $responseError = [ 'code' => 301, 'description' => 'Error'];

        //Se ejecuta la consulta sql
        header('Content-type: application/json');
        if (mysqli_query($conn, $sql)) { 
           echo json_encode( $responseOk);
        } else {
           echo json_encode( $responseError);
        }

        mysqli_close($conn);

        
        /**************************************/
        // Clase para manejar las respuestas
        class ResponseHandler {
            public static function send($code, $description, $errors = null) {
                header('Content-Type: application/json');
                http_response_code($code); // Establecer el código de respuesta HTTP

                $response = [
                    'code' => $code,
                    'description' => $description,
                ];

                if ($errors) {
                    $response['errors'] = $errors;
                }

                echo json_encode($response);
                exit();  // Asegura que se detenga la ejecución después de enviar la respuesta
            }

            public static function ok() {
                self::send(200, 'Data saved successfully.');
            }

            public static function badRequest($message = 'Invalid input data', $errors = []) {
                self::send(400, $message, $errors);
            }

            public static function error($message = 'Server error') {
                self::send(500, $message);
            }
        }

?>
