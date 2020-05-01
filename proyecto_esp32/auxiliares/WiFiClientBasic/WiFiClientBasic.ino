#include <ArduinoJson.h>

#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP("ERAZO", "sofia108516#");

    Serial.print("Waiting for WiFi connection... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(5000);
}


void loop()
{
    bool boleano=false;
    const uint16_t port = 80;
    const char server[]= "http://proyecto123.pythonanywhere.com/revisar/examinarusuario/1/";
    Serial.print("Connecting to ");
    Serial.println(server);
    
    HTTPClient client;
       int i;
   int vector[] = {85,
86,
87,
88,
89,
90,
91,
92,
91,
90,
89,
89,
90,
89,
88,
87,
86,
85,
84,
83};

    
    if (!client.begin(server)) {
      Serial.println("Connection failed.");
      Serial.println("Waiting 5 seconds before retrying...");
      delay(5000);
      return;
    }
    else{
      while (true){                                   
      for (i=0; i<=19; i++){
        Serial.println("conexión exitosa");
        
        // ---------LEER DATOS
        int httpCode=client.GET();

        DynamicJsonDocument doc(1024);
        if (httpCode > 0) {
          // Get the request response payload
          String payload = client.getString(); 
          Serial.println(payload);
          
          // obtener los datos de cadena de texto json
          
          deserializeJson(doc, payload);
          JsonObject obj = doc.as<JsonObject>();
          long frecuencia_estimulacion = obj[String("frecuencia_estimulacion")];
          Serial.println("la frecuencia de estimulación es");
          Serial.println(frecuencia_estimulacion);
        }

        
        // -------- SOBREESCRIBIR datos en api del servidor
        boleano=not(boleano);
        client.addHeader("Content-Type", "application/json");
        delay(450);
        String message = "";
        StaticJsonDocument<300> jsonDoc;
        //doc[String("nombre")] = "hola";
        doc[String("frecuencia_estimulacion")] = vector[i];
        //doc[String("frecuencia_estimulacion")] = 87+7*int(boleano);
        doc[String("edad")] =60+ 3*int(boleano);
        
        serializeJson(doc, message);
        httpCode = client.PUT(message);
        delay(450);
        

        }
        }
        
    }
    
    /*

    //const uint16_t port = 80;

    //const char server[]= "www.google.com";
    ///const char server[]= "http://proyecto123.pythonanywhere.com/revisar/examinarusuario/1/";
    //const char * host = "proyecto123.pythonanywhere.com/revisar/examinarusuario/1/";

 
    // Use WiFiClient class to create TCP connections
    
    //WiFiClient client;

    if (!client.begin(server)) {
    //if (!client.connect(server, port)) {
        Serial.println("Connection failed.");
        Serial.println("Waiting 5 seconds before retrying...");
        delay(5000);
        return;
    }
  else{                                   
      Serial.println("conexión exitosa");
      //http://proyecto123.pythonanywhere.com/revisar/examinar?format=json
      int httpCode=client.GET();

      if (httpCode > 0) {
      // Get the request response payload
      String payload = client.getString(); 
      Serial.print(payload);
      // TODO: Parsing
      }

     
      //client.print("GET /revisar/examinarusuario/1 HTTP/1.1");
      //client.println("GET /search?q=arduino HTTP/1.1");
      //client.println("Host: www.google.com");
      //client.println("Connection: close");
      //client.println();
    }

    

    // This will send a request to the server
    //uncomment this line to send an arbitrary string to the server
    //client.print("Send this data to the server");
    //uncomment this line to send a basic document request to the server
    //client.print("GET http://proyecto123.pythonanywhere.com/revisar/examinarusuario/1/");
    
    //client.print("GET /index.html HTTP/1.1\n\n");

  int maxloops = 0;

  //wait for the server's reply to become available
//  while (!client.available() && maxloops < 1000)
//  {
//    maxloops++;
//    delay(100); //delay 1 msec
//  }
//  while (client.available() > 0)
//  {
//    //read back one line from the server
//    char c = client.read();
//    Serial.write(c);
//    //String line = client.readStringUntil('\r');
//    //Serial.println(line);
//  }

  
  //{
  //  Serial.println("client.available() timed out ");
  //}

  //  Serial.println("Closing connection.");
  //  client.stop();

  //  Serial.println("Waiting 5 seconds before restarting...");
 */
}
