
// LIBRERIAS WIFI
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiMulti.h>

// OBJETO CONEXIÓN WIFI
WiFiMulti WiFiMulti;

// CLIENTE HTTP
HTTPClient client;
// SERVIDOR Y PUERTO A DONDE SE VA A CONECTAR EL DISPOSITIVO
const uint16_t port = 80;
const char server[]= "http://lerdbiomedica.pythonanywhere.com/APIusuario/1/";


// OBJETO JSON PARA ALMACENAR INFORMACIÓN RECIBIDA Y ENVIADA A SERVIDOR
DynamicJsonDocument doc(10000);
bool boleano;

int PLETISMOGRAFIA[]={130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 130000, 122879, 122825, 122757, 122632, 122513, 122424, 122387, 122340, 122263, 122187, 122148, 122111, 122074, 122042, 122052, 122094, 122112, 122072, 122052, 122071, 122101, 122101, 122108, 122196, 122274, 122288, 122287, 122323, 122326, 122320, 122319, 122330, 122405, 122450, 122433, 122441, 122440, 122451, 122457, 122438, 122490, 122572, 122603, 122628, 122612, 122634, 122653, 122634, 122648, 122713, 122792, 122829, 122819, 122823, 122876, 122876, 122869, 122890, 122949, 123009, 122989, 122935, 122880, 122789, 122684, 122570, 122505, 122473, 122440, 122372, 122315, 122287, 122278, 122228, 122211, 122212, 122270, 122281, 122270, 122258, 122285, 122309, 122319, 122306, 122377, 122455, 122463, 122450, 122457, 122482, 122482, 122465, 122502, 122528, 122576, 122600, 122567, 122573, 122592, 122601, 122597, 122638, 122700, 122759, 122745, 122743, 122758, 122781, 122784, 122772, 122825, 122878, 122921, 122935, 122951, 122979, 122981, 122991, 122966, 122996, 123025, 123009, 122908, 122781, 122714, 122613, 122514, 122425, 122420, 122449, 122401, 122346, 122325, 122303, 122302, 122258, 122270, 122308, 122345, 122352, 122343, 122361, 122385, 122387, 122373, 122424, 122506, 122535, 122531, 122501, 122502, 122511, 122495, 122485, 122525, 122591, 122587, 122573, 122583, 122619, 122611, 122605, 122664, 122723, 122777, 122784, 122787, 122801, 122822, 122835, 122824, 122860, 122931, 122987, 122995, 122979, 123030, 123036, 123034, 122992, 122993, 123010, 122945, 122852, 122730, 122636, 122554, 122473, 122407, 122398, 122407, 122382, 122337, 122320, 122296, 122272, 122242, 122263, 122297, 122336, 122353, 122351, 122361, 122402, 122396, 122408, 122434, 122510, 122540, 122532, 122506, 122518, 122526, 122517, 122483, 122531, 122613, 122647, 122649, 122637, 122650, 122670, 122685, 122650, 122724, 122795, 122846, 122856, 122844, 122875, 122889, 122871, 122888, 122935, 123009, 123038, 123044, 123050, 123064, 123055, 123014, 122999, 122985, 122919, 122813, 122703, 122622, 122541, 122470, 122401, 122377, 122415, 122392, 122364, 122320, 122308};
void enviarDatosServidor(int SENAL_ENVIAR[256], int frecuencia_cardiaca_enviar, int oximetria_enviar, int numero_picos_enviar, int BalanceIRRDC_enviar);

void setup()
{
    Serial.begin(115200);
    delay(10);

    // PONER AQUÌ CLAVE Y CONTRASEÑA WIFI
    WiFiMulti.addAP("ERAZO", "sofia108516#");
    Serial.print("Esperano conexión WIFI");
    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
        


}



void loop()
{
    boleano=false;
    
    // variables auxiliares
    int i;  // puntero de valor de señal  enviar
    int tiempo;

    Serial.print("Conectando a: ");
    Serial.println(server);

    //REALIZAR LA CONEXION    
    if (!client.begin(server)) {
        // conexión fallida
        Serial.println("Connection failed.");
        Serial.println("Waiting 5 seconds before retrying...");
        delay(5000);
        return;
    }
    else{
        Serial.println("conexión exitosa");
        while (true){    
            enviarDatosServidor(PLETISMOGRAFIA,75+int(boleano),98-int(boleano),0,1);                               
        } 
    }
}

void enviarDatosServidor(int SENAL_ENVIAR[256], int frecuencia_cardiaca_enviar, int oximetria_enviar, int numero_picos_enviar, int BalanceIRRDC_enviar){
    Serial.print("enviando datos");
    
    int tiempo=micros();
    

    int httpCode=client.GET();
    if (httpCode > 0) {
        // --------- LEER DATOS DE BASE DE DATOS
        // Get the request response payload
        String payload = client.getString(); 
        //Serial.println(payload);          
        DeserializationError error= deserializeJson(doc, payload); // deserializa y revisa errores
        if (error) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
        }
        
        int frecuencia_cardiaca = doc["frecuenciaCardiaca"].as<int>();
        Serial.print("la frecuencia cardíaca es");
        Serial.println(frecuencia_cardiaca);
        

        // -------- SOBREESCRIBIR DATOS EN BASE DE DATOS
        
        boleano=not(boleano);
        client.addHeader("Content-Type", "application/json");
        
        for (int j=0;j<256;j++){doc["pletismografia"]["senal"][j]=SENAL_ENVIAR[j]+ 10000*int(boleano);}
        doc["frecuenciaCardiaca"] = frecuencia_cardiaca_enviar;
        doc["oximetria"] = oximetria_enviar;
        doc["NumeroPicos"]=numero_picos_enviar;
        doc["BalanceIRRDC"]=BalanceIRRDC_enviar;

        String message = "";
        serializeJson(doc, message);
        //Serial.print(message);
        httpCode = client.PUT(message);
    }  
    Serial.print("El tiempo transcurrido es:");
    Serial.print(micros()-tiempo);

}