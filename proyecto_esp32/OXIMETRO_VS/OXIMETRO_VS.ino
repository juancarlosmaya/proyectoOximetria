bool tomarDatos=false;                          // bandera para iniciar lectura de datos en sensor e impresión en OLED

// LIBRERIAS WIFI
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiMulti.h>
WiFiMulti WiFiMulti;                            // objeto conexion wifi
HTTPClient client;                              // cliente http
const uint16_t port = 80;                       // servidor y puerto a donde se va a conectar el dispositivo
//const char server[]="http://127.0.0.1:8000/APIusuario/1/";
const char server[]= "http://lerdbiomedica.pythonanywhere.com/APIusuario/1/";
DynamicJsonDocument doc(10000);                 // objeto json para almacenar información recibida y enviada a servidor      
TaskHandle_t manejadorTareaServidor= NULL;      // manejador de tarea envío datos a servidor


// Pantalla OLED
#include <U8g2lib.h>
#include <U8x8lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Fourier
#include <arduinoFFT.h>
#include <defs.h>
#include <types.h>
arduinoFFT FFT = arduinoFFT(); 

#include "Arduino.h"
#include "Esp.h"

// Max30102
#include <Wire.h>
#include "MAX30105.h"
MAX30105 particleSensor;
TaskHandle_t manejadorTareaSensorPantalla= NULL;

// SEÑALES LEIDAS DE SENSOR MAX30102
float senalAnalizarIR[512];
float senalAnalizarRojo[512];
  

// VARIABLES DE FILTROS Y  PSD
float resultadoPasaBajos[512];                   // DE FILTROS
float resultadoPasaAltos[512];
float resultadoPasaAltos2[512];
float resultadoPasaBajos2[512];
float  resultadoPasaBajos3[512];
float imprimirPasaBajos[512];
float imprimirPasaAltos[512];
float imprimir[256];                 
const uint16_t samples = 512;                   // DE PSD 
double senal5seg[samples];
const double samplingFrequency = 100;
const uint8_t amplitude = 100;
double senalPSD[samples/2];
double vImag2[samples];
const uint16_t ventana= 128;                    // DE MUESTRAS PARA PSD
double vImag[ventana];
double muestra[ventana];
double psd[ventana/2];
double psdPromedio[ventana/2];
float imprimirOLED[256];                        // copia de imprimir a mostrar en la OLED

float senal[512];
float senalEliminandoPicos[512];
int numeroPicosEncontrados=0;          

// variables Rojo
float valorDCRojo;
float frecuenciaCardiacaRojo;
float valorACRojo;
float frecuenciaCardiacaPSDRojo;

// variables IR
float valorDCIR;
float frecuenciaCardiacaIR;
float valorACIR;
float frecuenciaCardiacaPSDIR;

float hamming[512]={0,0,0,0,0,0,0.08, 0.08003647, 0.08014586, 0.08032815, 0.08058333, 0.08091135, 0.08131215, 0.08178567, 0.08233184, 0.08295058, 0.08364177, 0.08440532, 0.0852411 , 0.08614898, 0.08712882, 0.08818046, 0.08930373, 0.09049846, 0.09176445, 0.09310151, 0.09450942, 0.09598796, 0.0975369 , 0.09915599, 0.10084497, 0.10260358, 0.10443154, 0.10632855, 0.10829432, 0.11032854, 0.11243087, 0.114601  , 0.11683857, 0.11914323, 0.12151461, 0.12395235, 0.12645605, 0.12902531, 0.13165973, 0.13435889, 0.13712236, 0.13994971, 0.14284048, 0.14579422, 0.14881046, 0.15188872, 0.15502852, 0.15822935, 0.16149071, 0.16481208, 0.16819293, 0.17163273, 0.17513093, 0.17868699, 0.18230032, 0.18597037, 0.18969655, 0.19347827, 0.19731492, 0.20120591, 0.20515061, 0.2091484 , 0.21319865, 0.2173007 , 0.22145392, 0.22565765, 0.22991121, 0.23421393, 0.23856514, 0.24296414, 0.24741023, 0.2519027 , 0.25644086, 0.26102397, 0.26565131, 0.27032215, 0.27503575, 0.27979135, 0.28458821, 0.28942556, 0.29430265, 0.29921868, 0.30417289, 0.30916449, 0.31419268, 0.31925668, 0.32435568, 0.32948886, 0.33465542, 0.33985454, 0.34508538, 0.35034713, 0.35563895, 0.36096   , 0.36630944, 0.37168641, 0.37709007, 0.38251955, 0.38797401, 0.39345257, 0.39895436, 0.40447851, 0.41002415, 0.4155904 , 0.42117637, 0.42678118, 0.43240394, 0.43804376, 0.44369975, 0.449371  , 0.45505662, 0.46075571, 0.46646736, 0.47219067, 0.47792473, 0.48366863, 0.48942147, 0.49518232, 0.50095028, 0.50672443, 0.51250386, 0.51828764, 0.52407487, 0.52986462, 0.53565598, 0.54144803, 0.54723984, 0.55303051, 0.55881912, 0.56460474, 0.57038646, 0.57616336, 0.58193453, 0.58769905, 0.59345601, 0.59920449, 0.60494359, 0.61067239, 0.61638998, 0.62209546, 0.62778793, 0.63346648, 0.63913021, 0.64477823, 0.65040963, 0.65602352, 0.66161903, 0.66719525, 0.6727513 , 0.67828631, 0.68379939, 0.68928967, 0.69475629, 0.70019836, 0.70561504, 0.71100547, 0.71636878, 0.72170412, 0.72701066, 0.73228755, 0.73753396, 0.74274904, 0.74793198, 0.75308196, 0.75819815, 0.76327974, 0.76832594, 0.77333594, 0.77830894, 0.78324416, 0.78814081, 0.79299813, 0.79781533, 0.80259165, 0.80732635, 0.81201866, 0.81666784, 0.82127316, 0.82583388, 0.83034929, 0.83481866, 0.83924129, 0.84361648, 0.84794353, 0.85222176, 0.85645049, 0.86062904, 0.86475676, 0.86883299, 0.87285709, 0.87682842, 0.88074634, 0.88461024, 0.8884195 , 0.89217352, 0.89587171, 0.89951347, 0.90309824, 0.90662543, 0.9100945 , 0.9135049 , 0.91685608, 0.92014751, 0.92337866, 0.92654904, 0.92965813, 0.93270544, 0.93569049, 0.9386128 , 0.94147192, 0.94426739, 0.94699876, 0.9496656 , 0.95226749, 0.95480402, 0.95727479, 0.9596794 , 0.96201747, 0.96428863, 0.96649252, 0.96862879, 0.97069711, 0.97269714, 0.97462857, 0.97649109, 0.97828441, 0.98000824, 0.98166231, 0.98324635, 0.98476012, 0.98620338, 0.98757589, 0.98887744, 0.99010783, 0.99126685, 0.99235433, 0.99337008, 0.99431396, 0.99518581, 0.99598549, 0.99671288, 0.99736786, 0.99795032, 0.99846018, 0.99889735, 0.99926176, 0.99955337, 0.99977211, 0.99991795, 0.99999088, 0.99999088, 0.99991795, 0.99977211, 0.99955337, 0.99926176, 0.99889735, 0.99846018, 0.99795032, 0.99736786, 0.99671288, 0.99598549, 0.99518581, 0.99431396, 0.99337008, 0.99235433, 0.99126685, 0.99010783, 0.98887744, 0.98757589, 0.98620338, 0.98476012, 0.98324635, 0.98166231, 0.98000824, 0.97828441, 0.97649109, 0.97462857, 0.97269714, 0.97069711, 0.96862879, 0.96649252, 0.96428863, 0.96201747, 0.9596794 , 0.95727479, 0.95480402, 0.95226749, 0.9496656 , 0.94699876, 0.94426739, 0.94147192, 0.9386128 , 0.93569049, 0.93270544, 0.92965813, 0.92654904, 0.92337866, 0.92014751, 0.91685608, 0.9135049 , 0.9100945 , 0.90662543, 0.90309824, 0.89951347, 0.89587171, 0.89217352, 0.8884195 , 0.88461024, 0.88074634, 0.87682842, 0.87285709, 0.86883299, 0.86475676, 0.86062904, 0.85645049, 0.85222176, 0.84794353, 0.84361648, 0.83924129, 0.83481866, 0.83034929, 0.82583388, 0.82127316, 0.81666784, 0.81201866, 0.80732635, 0.80259165, 0.79781533, 0.79299813, 0.78814081, 0.78324416, 0.77830894, 0.77333594, 0.76832594, 0.76327974, 0.75819815, 0.75308196, 0.74793198, 0.74274904, 0.73753396, 0.73228755, 0.72701066, 0.72170412, 0.71636878, 0.71100547, 0.70561504, 0.70019836, 0.69475629, 0.68928967, 0.68379939, 0.67828631, 0.6727513 , 0.66719525, 0.66161903, 0.65602352, 0.65040963, 0.64477823, 0.63913021, 0.63346648, 0.62778793, 0.62209546, 0.61638998, 0.61067239, 0.60494359, 0.59920449, 0.59345601, 0.58769905, 0.58193453, 0.57616336, 0.57038646, 0.56460474, 0.55881912, 0.55303051, 0.54723984, 0.54144803, 0.53565598, 0.52986462, 0.52407487, 0.51828764, 0.51250386, 0.50672443, 0.50095028, 0.49518232, 0.48942147, 0.48366863, 0.47792473, 0.47219067, 0.46646736, 0.46075571, 0.45505662, 0.449371  , 0.44369975, 0.43804376, 0.43240394, 0.42678118, 0.42117637, 0.4155904 , 0.41002415, 0.40447851, 0.39895436, 0.39345257, 0.38797401, 0.38251955, 0.37709007, 0.37168641, 0.36630944, 0.36096   , 0.35563895, 0.35034713, 0.34508538, 0.33985454, 0.33465542, 0.32948886, 0.32435568, 0.31925668, 0.31419268, 0.30916449, 0.30417289, 0.29921868, 0.29430265, 0.28942556, 0.28458821, 0.27979135, 0.27503575, 0.27032215, 0.26565131, 0.26102397, 0.25644086, 0.2519027 , 0.24741023, 0.24296414, 0.23856514, 0.23421393, 0.22991121, 0.22565765, 0.22145392, 0.2173007 , 0.21319865, 0.2091484 , 0.20515061, 0.20120591, 0.19731492, 0.19347827, 0.18969655, 0.18597037, 0.18230032, 0.17868699, 0.17513093, 0.17163273, 0.16819293, 0.16481208, 0.16149071, 0.15822935, 0.15502852, 0.15188872, 0.14881046, 0.14579422, 0.14284048, 0.13994971, 0.13712236, 0.13435889, 0.13165973, 0.12902531, 0.12645605, 0.12395235, 0.12151461, 0.11914323, 0.11683857, 0.114601  , 0.11243087, 0.11032854, 0.10829432, 0.10632855, 0.10443154, 0.10260358, 0.10084497, 0.09915599, 0.0975369 , 0.09598796, 0.09450942, 0.09310151, 0.09176445, 0.09049846, 0.08930373, 0.08818046, 0.08712882, 0.08614898, 0.0852411 , 0.08440532, 0.08364177, 0.08295058, 0.08233184, 0.08178567, 0.08131215, 0.08091135, 0.08058333, 0.08032815, 0.08014586, 0.08003647, 0.08, 0,0,0,0,0,0};

int led = 2;

float calculoFrecuenciaCardiaca(float entrada[512]);
void obtenerValorAcDc(float miSenal[512], float *direccion_valorDC, float *direccion_frecuenciaCardiaca, float *direccion_valorAC, float *direccion_frecuenciaCardiacaPSD);
void eliminarPicos(float senalPicos[512]);
void enviarDatosServidor(float SENAL_ENVIAR[256], float frecuencia_cardiaca_enviar, float oximetria_enviar, int numero_picos_enviar, float BalanceIRRDC_enviar);
void lecturaDatosMAX30102();
void envioDatosPantalla();


float resultadoFrecuenciaCardiaca;
float resultadoSpo2;


// VARIABLES MAX30102
byte ledBrightnessIR = 0xC8;
int muestrasRepresadas=0;
int punteroValorSensado;                    // número de muestra leida y almacenada fs=100 Hz
bool aumentarIntensidadIR=false;            // banderas para hacer ajuste de intensidad en led infrarojo
bool disminuirIntensidadIR=false;




void envioDatosServidor(void * parameter){
  for(;;){ 
    //    Serial.print("la tarea del servidor se esta ejecutando en: ");
    //    Serial.println(xPortGetCoreID());

    //    Serial.println("resultadoSpO2=");
    //    Serial.println(resultadoSpo2);

        //enviarDatosServidor(imprimir,resultadoFrecuenciaCardiaca,resultadoSpo2,numeroPicosEncontrados,(valorDCIR-valorDCRojo)*1000/valorDCIR);                               
        enviarDatosServidor(imprimir,resultadoFrecuenciaCardiaca,resultadoSpo2,numeroPicosEncontrados,2);                                    
  }
}


void lecturaSensorImpresionPantalla(void * parameter){
    for(;;){
        lecturaDatosMAX30102(); 
        envioDatosPantalla();     
    }
}

void setup() {
    tomarDatos=false;
    
    Serial.begin(115200);
    pinMode(led, OUTPUT);
    
    // CONFIGURACIÓN E INICIALIZACIÓN PANTALLA OLED
    u8g2.setBusClock(400000);
    u8g2.begin();
    u8g2.setBusClock(400000);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
    u8g2.drawStr(0,10,"Fc= ");	// write something to the internal memory
    u8g2.setCursor(20,10);
    u8g2.print(u8x8_u8toa(89, 3));
    u8g2.drawStr(40,10,"bpm");
    u8g2.drawStr(70,10,"SpO2= ");  // write something to the internal memory
    u8g2.setCursor(105,10);
    u8g2.print(u8x8_u8toa(94, 2));
    u8g2.drawStr(120,10,"%");
    u8g2.sendBuffer();
    

    // CONFIGURACIÓN DE SENSOR MAX30102
    if (particleSensor.begin(Wire, I2C_SPEED_STANDARD) == false){
    //if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false){
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
        while (1);
    }
    byte ledBrightness = 0xFF; //Options: 0=Off to 255=50mA
    byte sampleAverage = 2; //Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    int sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 411; //Options: 69, 118, 215, 411
    int adcRange = 16384; //Options: 2048, 4096, 8192, 16384
    particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
    particleSensor.setPulseAmplitudeIR(ledBrightnessIR);


    // CONFIGURACIÓN ENVÍO DE DATOS A SERVIDOR
    WiFiMulti.addAP("ERAZO", "tommy1505"); // poner aquí clave y contraseña wifi
    Serial.print("Esperano conexión WIFI");
    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // CONFIGURACIÓN DE TAREA ENVÍO DE DATOS A INTERNET
    xTaskCreatePinnedToCore(
        envioDatosServidor,             // nombre de la funcion
        "envio datos servidor",         // nombre de funcion para depuración
        10000,                          // tamaño de stack (bytes)
        NULL,                           // parametros pasados
        1,                              // prioridad de la tarea
        &manejadorTareaServidor,        // manejador de esta tarea
        1                              // nucleo en le que se ejecuta esta tarea
    );

    // CONFIGURACIÓN DE TAREA LECTURA DE SENSOR E IMPRESIÓN EN PANTALLA
    xTaskCreatePinnedToCore(
        lecturaSensorImpresionPantalla,                         // nombre de la funcion
        "lectura datos sensor e impresion pantalla",            // nombre de funcion para depuración
        10000,                                                  // tamaño de stack (bytes)
        NULL,                                                   // parametros pasados
        7,                                                      // prioridad de la tarea
        &manejadorTareaSensorPantalla,                          // manejador de esta tarea
        0                                                       // nucleo en el que se ejecuta esta tarea
    );  
}


void loop() {

    tomarDatos=false;

 

    // resultados
    float R;
    float spo2;
    int i;

    Serial.print("Conectando a: ");
    Serial.println(server);

    //REALIZAR LA CONEXION    
    if (!client.begin(server)) {
        // conexión fallida
        Serial.println("Connection failed.");
        while (1){}
    }

    // inicialización lectura de senal rojo e Infrarojo 
    float valorInicialIR=0;
    float valorInicialRojo=0;
    delay(100);
    particleSensor.check();
    while (particleSensor.available()){
        //for (i=0;i<512;i++){senalAnalizarIR[i]=senalIR[0];}
        valorInicialIR=float(particleSensor.getFIFOIR());
        for (i=0;i<512;i++){senalAnalizarIR[i]=valorInicialIR;}
        Serial.println(valorInicialIR);
        //for (i=0;i<512;i++){senalAnalizarRojo[i]=senalRojo[0];}
        valorInicialRojo=float(particleSensor.getFIFORed());
        for (i=0;i<512;i++){senalAnalizarRojo[i]=valorInicialRojo;}
        particleSensor.nextSample();
    }

    int tiempoTranscurrido;
    
    punteroValorSensado=0;
    tiempoTranscurrido=millis();
   
    tomarDatos=true; // habilita lectura de sensor e impresión en OLED
    Serial.println("Se inicia lectura e impresión en OLED");
    while(1){
        
        /*
        // LECTURA MAX30102
        particleSensor.check();
        while (particleSensor.available()) //do we have new data?
        {
            punteroValorSensado=punteroValorSensado+1;
            for (i=0;i<511;i++){senalAnalizarIR[i]=senalAnalizarIR[i+1];}
            //senalAnalizarIR[511]=senalIR[punteroValorSensado];
            senalAnalizarIR[511]=float(particleSensor.getFIFOIR());
            for (i=0;i<511;i++){senalAnalizarRojo[i]=senalAnalizarRojo[i+1];}
            //senalAnalizarRojo[511]=senalRojo[punteroValorSensado];
            senalAnalizarRojo[511]=float(particleSensor.getFIFORed());
            particleSensor.nextSample();
            muestrasRepresadas++;    
        }
        Serial.print("el número de muestras represadas fué: ");
        Serial.println(muestrasRepresadas);
        muestrasRepresadas=0;
        */
        //delay(10);
        /*
        Serial.println("la señal IR a analizar es: ");
        for (i=0;i<512;i++){
            Serial.print(i);
            Serial.print(" ");
            Serial.println(senalAnalizarIR[i]);
        }
        Serial.println("la señal Roja a analizar fue:");
        for (i=0;i<512;i++){
            Serial.print(i);
            Serial.print(" ");
            Serial.println(senalAnalizarRojo[i]);
        }
        */ 
        //Serial.print("el tiempo transcurrido es: ");
        //Serial.println((millis()- tiempoTranscurrido)/1000);
        //Serial.println("");
        //tiempoTranscurrido=millis();
        
        // ANÁLISIS SEÑAL IR
        obtenerValorAcDc(senalAnalizarIR, &valorDCIR, &frecuenciaCardiacaIR, &valorACIR, &frecuenciaCardiacaPSDIR);
        /*
        Serial.print("MI VALOR DC IR ES: ");
        Serial.println(valorDCIR);
        Serial.print("MI FRECUENCIA CARDIACA IR ES: ");
        Serial.println(frecuenciaCardiacaIR); 
        Serial.print("MI VALOR AC IR ES: ");
        Serial.println(valorACIR);
        Serial.print("MI FRECUENCIA CARDIACA PSD IR ES: ");
        Serial.println(frecuenciaCardiacaPSDIR);
        Serial.println("");
        */
        

        //Serial.print("FcIR: ");
        //Serial.print(frecuenciaCardiacaIR); 
        //Serial.print(" FcIRPSD: ");
        //Serial.println(frecuenciaCardiacaPSDIR); 
        /*
        // FINALIZACIÓN DE 51 SEGUNDOS DE FUNCIONAMIENTO 
        if (punteroValorSensado>5110){
            Serial.println("IR: señal procesada:");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(senalAnalizarIR[i]);
            } 
            Serial.println("IR: señal sin nivel DC es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(senal[i]);
            }
            Serial.println("IR: señal con filtro pasa altos es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaAltos[i]);
            }
            Serial.println("IR: señal con filtro pasa altos 2 es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaAltos2[i]);
            }
            Serial.println("IR: señal con filtro pasa bajos 2 es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaBajos2[i]);
            }
            Serial.println("IR: señal con filtro pasa bajos 3 es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaBajos3[i]);
            }
        }
        */

        for (i=0;i<256;i++){
            imprimirOLED[i]=imprimir[i];
        }
        

        // ANÁLISIS SEÑAL ROJO
        obtenerValorAcDc(senalAnalizarRojo, &valorDCRojo, &frecuenciaCardiacaRojo, &valorACRojo, &frecuenciaCardiacaPSDRojo);
        
        Serial.print("MI VALOR DC ROJO ES: ");
        Serial.println(valorDCRojo);
        Serial.print("MI FRECUENCIA CARDIACA ROJO ES: ");
        Serial.println(frecuenciaCardiacaRojo); 
        Serial.print("MI VALOR AC ROJO ES: ");
        Serial.println(valorACRojo);
        Serial.print("MI FRECUENCIA CARDIACA PSD ROJO ES: ");
        Serial.println(frecuenciaCardiacaPSDRojo);
        Serial.println("");
        

            

        //Serial.print("FcR_: ");
        //Serial.print(frecuenciaCardiacaRojo); 
        //Serial.print(" FcR_PSD: ");
        //Serial.println(frecuenciaCardiacaPSDRojo);
        
        R=(valorACRojo/valorDCRojo)/(valorACIR/valorDCIR);
        R=R*2.5;
        spo2 =110-25*R;
        
        // VALIDACIÓN RESULTADOS DE SpO2 y FC
        //if ((abs(frecuenciaCardiacaIR-frecuenciaCardiacaRojo)>6) || (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaIR)>25) || (abs(frecuenciaCardiacaPSDRojo-frecuenciaCardiacaRojo)>25) ||  (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaPSDRojo)>25)  || (frecuenciaCardiacaIR>240) || (frecuenciaCardiacaRojo>240) or (frecuenciaCardiacaIR<30) || (frecuenciaCardiacaRojo<30) || (spo2<50) || (spo2>99) || (valorACRojo<500) || (valorACIR<500) ){
        //if ( (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaIR)>25) || (abs(frecuenciaCardiacaPSDRojo-frecuenciaCardiacaRojo)>25) ||  (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaPSDRojo)>25)  || (frecuenciaCardiacaIR>240) || (frecuenciaCardiacaRojo>240) or (frecuenciaCardiacaIR<30) || (frecuenciaCardiacaRojo<30) || (spo2<50) || (spo2>99) ){
        //if ( (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaIR)>25) || (frecuenciaCardiacaIR>240) || (frecuenciaCardiacaIR<30) || (spo2<50) || (spo2>100) ){
        if  (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaPSDRojo)>15){ 
            // datos incorrectos
            resultadoSpo2=0;
            resultadoFrecuenciaCardiaca=0;
        }
        else{
            // datos correctos
            resultadoSpo2=spo2;
            if (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaIR)>15){
                resultadoFrecuenciaCardiaca=0;     
            }
            else{
                resultadoFrecuenciaCardiaca=frecuenciaCardiacaIR;     
            }

              
        }
        
        //Serial.println(resultadoSpo2);



        // ACTUALIZACIÓN DE INTENSIDAD DE LEDS
        int estabilidad;
        //if ((punteroValorSensado % 64==0) && (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaPSDRojo)<25)){
        if (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaPSDRojo)<20){
            estabilidad++;
            if (estabilidad>10){
                Serial.println("DCIR/DCRR=");
                Serial.println((100*(valorDCIR-valorDCRojo)/valorDCIR));
                if ((100*(valorDCIR-valorDCRojo)/valorDCIR)>1 && ledBrightnessIR>0x01){
                    disminuirIntensidadIR=true;
                    //ledBrightnessIR=ledBrightnessIR-0x01;
                    //particleSensor.setPulseAmplitudeIR(ledBrightnessIR);
                }
                if ((100*(valorDCRojo-valorDCIR)/valorDCIR)>1 && ledBrightnessIR<0xFF){
                    aumentarIntensidadIR=true;
                    //ledBrightnessIR=ledBrightnessIR+0x01;
                    //particleSensor.setPulseAmplitudeIR(ledBrightnessIR);
                }
                estabilidad=0;
            }
        }
        else{
            estabilidad=0;
        }

        
        
        /*
        // VISUALIZACIÓN EN OLED
        if (punteroValorSensado % 1==0){
            u8g2.clearBuffer();
            int OLEDvalorActual;
            int OLEDvalorAnterior=37;
            for (i=128;i<256;i++){
                OLEDvalorActual=int(map(imprimir[i], -1200, 1000, 0, 40));
                OLEDvalorActual=55-OLEDvalorActual;
                u8g2.drawLine(i-128,OLEDvalorAnterior,i+1-128,OLEDvalorActual);
                OLEDvalorAnterior=OLEDvalorActual;
            }
            u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
            u8g2.drawStr(0,10,"Fc= ");	// write something to the internal memory
            u8g2.setCursor(20,10);
            if (resultadoFrecuenciaCardiaca>30){
                u8g2.print(u8x8_u8toa(resultadoFrecuenciaCardiaca, 3));    
            }
            else{
                u8g2.print(" --");   
            }
            u8g2.drawStr(40,10,"bpm");
            u8g2.drawStr(70,10,"SpO2= ");  // write something to the internal memory
            
            u8g2.setCursor(105,10);
            if (resultadoSpo2>60){
                u8g2.print(u8x8_u8toa(resultadoSpo2, 2));
            }
            else {
                u8g2.print("---");
            }
            u8g2.drawStr(120,10,"%");
            
            
            u8g2.setCursor(0,60);
            u8g2.print(u8x8_u8toa(numeroPicosEncontrados, 2));
            u8g2.sendBuffer();
            
            u8g2.setCursor(80,60);
            u8g2.print(u8x8_u8toa((valorDCIR-valorDCRojo)*1000/valorDCIR, 3));
            u8g2.setCursor(110,60);
            u8g2.print(u8x8_u8toa(int(ledBrightnessIR),3));
            u8g2.sendBuffer();  

        }
        */

    //    Serial.print("la tarea principal se esta ejecutando en: ");
    //    Serial.println(xPortGetCoreID());

        /*
        // FINALIZACIÓN DE 51 SEGUNDOS DE FUNCIONAMIENTO 
        if (punteroValorSensado>5110){
            Serial.println("ROJO: señal procesada:");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(senalAnalizarRojo[i]);
            } 
            Serial.println("ROJO: señal sin nivel DC:");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(senal[i]);
            }
            Serial.println("ROJO: señal con filtro pasa altos es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaAltos[i]);
            }
            Serial.println("ROJO: señal con filtro pasa altos 2 es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaAltos2[i]);
            }
            Serial.println("ROJO: señal con filtro pasa bajos 2 es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaBajos2[i]);
            }
            Serial.println("ROJO: señal con filtro pasa bajos 3 es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaBajos3[i]);
            }
            while(1){};
        }
        */
        
        
    }
}

float calculoFrecuenciaCardiaca(float entrada[500]){
    int cruces[512];
    int ubicacionCruces[512];
    int i;
    for (i=0;i<512;i++){
        cruces[i]=0;
    }
    for (i=0;i<512;i++){
        ubicacionCruces[i]=0;
    }
    int j=0;
    for (i=0;i<512;i++){
        if (resultadoPasaBajos2[i]<0 && resultadoPasaBajos2[i+1]>0){
            cruces[i]=100;
            ubicacionCruces[j]=i;
            j=j+1;
        }
    }
    for (i=0;i<512;i++){
        //printf( "%d\n", ubicacionCruces[i]);
    }
    float frecuencias[100];
    for (i=0;i<100;i++){
        frecuencias[i]=0;
    }
    i=0;
    while ((ubicacionCruces[i]>0) && (ubicacionCruces[i+1]>0)){
        frecuencias[i]=(float)100/(ubicacionCruces[i+1]-ubicacionCruces[i]);
        i=i+1;    
    }
    for (i=0;i<100;i++){
        //printf( "%f\n", frecuencias[i]);
    }
    float frecuenciaPromedio;
    i=0;
    frecuenciaPromedio=0;
    while (frecuencias[i]>0){
        frecuenciaPromedio=frecuenciaPromedio+frecuencias[i];
        i=i+1;
    }
    frecuenciaPromedio=frecuenciaPromedio/i;
    frecuenciaPromedio=60*frecuenciaPromedio;
    
    return frecuenciaPromedio;

}

void  obtenerValorAcDc(float miSenal[512], float *direccion_valorDC, float *direccion_frecuenciaCardiaca, float *direccion_valorAC, float *direccion_frecuenciaCardiacaPSD){

    
    float valorDC;
    int i;

    for ( i = 0; i < 512; i++ ) {
      senal[i]=miSenal[i];
      //Serial.print(i);
      //Serial.print(" ");
      //Serial.println(senal[i]);
    }
    
    ///---------------CALCULAR Y ELIMINAR EL VALOR DC---------------
    
    // Filtro pasa bajos a 1 Hz
    float b[4]= {2.91464945e-05, 8.74394834e-05, 8.74394834e-05, 2.91464945e-05};
    float a[4]= {1, -2.87435689,  2.7564832, -0.88189313};
    resultadoPasaBajos[0]=senal[0];
    resultadoPasaBajos[1]=senal[0];
    resultadoPasaBajos[2]=senal[0];
    senal[1]=senal[0];
    senal[2]=senal[0];
    senal[3]=senal[0];
    for ( i = 3; i < 512; i++ ) {
        resultadoPasaBajos[i] =senal[i]*b[0]+senal[i-1]*b[1]+senal[i-2]*b[2]+senal[i-3]*b[3]-(resultadoPasaBajos[i-1]*a[1]+resultadoPasaBajos[i-2]*a[2]+resultadoPasaBajos[i-3]*a[3]);
        //Serial.println(resultadoPasaBajos[i]);
    }

    valorDC=0;
    for ( i = 200; i < 512; i++ ) {
        valorDC=valorDC+resultadoPasaBajos[i];
    }
    valorDC=valorDC/312;

    for ( i = 0; i < 512; i++ ) {
        senal[i]=senal[i]-valorDC;
        //Serial.println( senal[i]);
    }


    ///-------------------------------------------------------------

    eliminarPicos(senal);

    ///---------------CALCULAR EL VALOR AC--------------------------    
    
    // Filtro pasa altos a 0.5 Hz
    b[0]=0.96907117; b[1]=-2.90721352;  b[2]=2.90721352; b[3]=-0.96907117;
    a[0]=1; a[1]=-2.93717073; a[2]=2.87629972; a[3]=-0.93909894;
    senal[0]=senal[0]/2;
    resultadoPasaAltos[0]=senal[0];
    resultadoPasaAltos[1]=senal[0];
    resultadoPasaAltos[2]=senal[0];
    senal[1]=senal[0];
    senal[2]=senal[0];
    senal[3]=senal[0];
    for ( i = 3; i < 512; i++ ) {
        resultadoPasaAltos[i] =senal[i]*b[0]+senal[i-1]*b[1]+senal[i-2]*b[2]+senal[i-3]*b[3]-(resultadoPasaAltos[i-1]*a[1]+resultadoPasaAltos[i-2]*a[2]+resultadoPasaAltos[i-3]*a[3]);
        //Serial.print( "%f\n", resultadoPasaAltos[i]);
    }


    

    // INICIO IMPRIMIR
    // Filtro pasa altos a 0.5 Hz
    b[0]=0.96907117; b[1]=-2.90721352;  b[2]=2.90721352; b[3]=-0.96907117;
    a[0]=1; a[1]=-2.93717073; a[2]=2.87629972; a[3]=-0.93909894;
    resultadoPasaAltos[0]=resultadoPasaAltos[0]/2;
    imprimirPasaAltos[0]=resultadoPasaAltos[0];
    imprimirPasaAltos[1]=resultadoPasaAltos[0];
    imprimirPasaAltos[2]=resultadoPasaAltos[0];
    resultadoPasaAltos[1]=resultadoPasaAltos[0];
    resultadoPasaAltos[2]=resultadoPasaAltos[0];
    resultadoPasaAltos[3]=resultadoPasaAltos[0];
    for ( i = 3; i < 512; i++ ) {
        imprimirPasaAltos[i] =resultadoPasaAltos[i]*b[0]+resultadoPasaAltos[i-1]*b[1]+resultadoPasaAltos[i-2]*b[2]+resultadoPasaAltos[i-3]*b[3]-(imprimirPasaAltos[i-1]*a[1]+imprimirPasaAltos[i-2]*a[2]+imprimirPasaAltos[i-3]*a[3]);
        //Serial.print( "%f\n", imprimirPasaAltos[i]);
    }
    // filtro pasa bajos a 5 Hz.
    b[0]=0.00289819; b[1]=0.00869458;  b[2]=0.00869458; b[3]=0.00289819;
    a[0]=1; a[1]=-2.37409474; a[2]=1.92935567; a[3]=-0.53207537;
    imprimirPasaAltos[0]=imprimirPasaAltos[0]/2;
    imprimirPasaBajos[0]=imprimirPasaAltos[0];
    imprimirPasaBajos[1]=imprimirPasaAltos[0];
    imprimirPasaBajos[2]=imprimirPasaAltos[0];
    imprimirPasaAltos[1]=imprimirPasaAltos[0];
    imprimirPasaAltos[2]=imprimirPasaAltos[0];
    imprimirPasaAltos[3]=imprimirPasaAltos[0];
    for ( i = 3; i < 512; i++ ) {
        imprimirPasaBajos[i] =imprimirPasaAltos[i]*b[0]+imprimirPasaAltos[i-1]*b[1]+imprimirPasaAltos[i-2]*b[2]+imprimirPasaAltos[i-3]*b[3]-(imprimirPasaBajos[i-1]*a[1]+imprimirPasaBajos[i-2]*a[2]+imprimirPasaBajos[i-3]*a[3]);
        //Serial.println(imprimirPasaBajos[i]);
    }
    int j=0;
    for ( i = 0; i < 512; i=i+2 ) {
        imprimir[j] = imprimirPasaBajos[i];
        j++;
    }



    // FIN IMPRIMIR
    


    // ventana hamming 
    for ( i = 0; i < 512; i++ ) {
        resultadoPasaAltos[i]=resultadoPasaAltos[i]*hamming[i];
        //Serial.println(resultadoPasaAltos[i]);
    }



    // Filtro pasa altos a 0.5 Hz
    b[0]=0.96907117; b[1]=-2.90721352;  b[2]=2.90721352; b[3]=-0.96907117;
    a[0]=1; a[1]=-2.93717073; a[2]=2.87629972; a[3]=-0.93909894;
    resultadoPasaAltos[0]=resultadoPasaAltos[0]/2;
    resultadoPasaAltos2[0]=resultadoPasaAltos[0];
    resultadoPasaAltos2[1]=resultadoPasaAltos[0];
    resultadoPasaAltos2[2]=resultadoPasaAltos[0];
    resultadoPasaAltos[1]=resultadoPasaAltos[0];
    resultadoPasaAltos[2]=resultadoPasaAltos[0];
    resultadoPasaAltos[3]=resultadoPasaAltos[0];
    for ( i = 3; i < 512; i++ ) {
        resultadoPasaAltos2[i] =resultadoPasaAltos[i]*b[0]+resultadoPasaAltos[i-1]*b[1]+resultadoPasaAltos[i-2]*b[2]+resultadoPasaAltos[i-3]*b[3]-(resultadoPasaAltos2[i-1]*a[1]+resultadoPasaAltos2[i-2]*a[2]+resultadoPasaAltos2[i-3]*a[3]);
        //Serial.print( "%f\n", resultadoPasaAltos2[i]);
    }

    // filtro pasa bajos a 5 Hz.
    b[0]=0.00289819; b[1]=0.00869458;  b[2]=0.00869458; b[3]=0.00289819;
    a[0]=1; a[1]=-2.37409474; a[2]=1.92935567; a[3]=-0.53207537;
    resultadoPasaAltos2[0]=resultadoPasaAltos2[0]/2;
    resultadoPasaBajos2[0]=resultadoPasaAltos2[0];
    resultadoPasaBajos2[1]=resultadoPasaAltos2[0];
    resultadoPasaBajos2[2]=resultadoPasaAltos2[0];
    resultadoPasaAltos2[1]=resultadoPasaAltos2[0];
    resultadoPasaAltos2[2]=resultadoPasaAltos2[0];
    resultadoPasaAltos2[3]=resultadoPasaAltos2[0];
    
    for ( i = 3; i < 512; i++ ) {
        resultadoPasaBajos2[i] =resultadoPasaAltos2[i]*b[0]+resultadoPasaAltos2[i-1]*b[1]+resultadoPasaAltos2[i-2]*b[2]+resultadoPasaAltos2[i-3]*b[3]-(resultadoPasaBajos2[i-1]*a[1]+resultadoPasaBajos2[i-2]*a[2]+resultadoPasaBajos2[i-3]*a[3]);
    }


    // mi ultimo filtro
    // filtro pasa bajos a 3 Hz.

    // # 3HZ
    b[0]=0.00069935; b[1]=0.00209805;  b[2]=0.00209805; b[3]=0.00069935;
    a[0]=1; a[1]=-2.62355181; a[2]=2.31468258; a[3]=-0.68553598;
    //2 Hz
    //b[0]=0.00021961; b[1]=0.00065882;  b[2]=0.00065882; b[3]=0.00021961;
    //a[0]=1; a[1]=-2.74883581; a[2]=2.52823122; a[3]=-0.77763856;
    resultadoPasaBajos2[0]=resultadoPasaBajos2[0]/2;
    resultadoPasaBajos3[0]=resultadoPasaBajos2[0];
    resultadoPasaBajos3[1]=resultadoPasaBajos2[0];
    resultadoPasaBajos3[2]=resultadoPasaBajos2[0];
    resultadoPasaBajos2[1]=resultadoPasaBajos2[0];
    resultadoPasaBajos2[2]=resultadoPasaBajos2[0];
    resultadoPasaBajos2[3]=resultadoPasaBajos2[0];
    for ( i = 3; i < 512; i++ ) {
        resultadoPasaBajos3[i] =resultadoPasaBajos2[i]*b[0]+resultadoPasaBajos2[i-1]*b[1]+resultadoPasaBajos2[i-2]*b[2]+resultadoPasaBajos2[i-3]*b[3]-(resultadoPasaBajos3[i-1]*a[1]+resultadoPasaBajos3[i-2]*a[2]+resultadoPasaBajos3[i-3]*a[3]);
    }





    // fin de mi último filtro 











    //for ( i = -1; i < 512; i++ ) {
        //Serial.println(resultadoPasaBajos2[i]);
    //}

    float frecuenciaCardiaca=calculoFrecuenciaCardiaca(resultadoPasaBajos3);
    //Serial.print("El cálculo de la frecuencia cardíaca es de: ");
    //Serial.println(frecuenciaCardiaca);



     /* Print the results of the simulated sampling according to time */
    for (i=0;i<samples;i++){
      senal5seg[i]=resultadoPasaBajos3[i];
    }
  
    // submuestreo 1/2
    j=0;
    for (i=0;i<samples;i=i+2){
      senalPSD[j]=senal5seg[i];
      j++;
    }
    
    // Hamming
    // FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  
    
    int overlap= int(ventana*0.7);
    int saltoNuevaVentana= ventana-overlap;
    int numeroVentanaActual=0;
    int puntero;
    for (i=0;i<ventana/2;i++) {psdPromedio[i]=0;}
    for (puntero=0;puntero<((samples/2)-ventana);puntero=(puntero+saltoNuevaVentana)){
      numeroVentanaActual=numeroVentanaActual+1;
  
      // Extraer muestra
      for (i=0;i<=ventana;i++){
        muestra[i]=senalPSD[puntero+i];
      }  
  
      // Hamming
      FFT.Windowing(muestra, ventana, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  
      // Fourier
      for (i=0;i<ventana;i++){vImag[i]=0;}
      FFT.Compute(muestra, vImag, ventana, FFT_FORWARD); /* Compute FFT */
      FFT.ComplexToMagnitude(muestra, vImag, ventana); /* Compute magnitudes */
      //Serial.println("psd muestra");
      for (i=0;i<ventana/2;i++){
        psd[i]=2*muestra[i]*muestra[i]/(ventana*50.48);
        psdPromedio[i]=psdPromedio[i]+psd[i];
        //Serial.println(psd[i]);
      }
      
      
  
    }
    //Serial.println("PSD promedio");
    for (i=0;i<ventana/2;i++){
      psdPromedio[i]=psdPromedio[i]/numeroVentanaActual;
      //Serial.println(psdPromedio[i]);
    }
  
    //  Encontrar el PSD[j] máximo
    double maxPSD_v=0;
    double maxPSD_i=0; 
    for ( int i = 0; i < ventana/2; i++ )
    {
      if ( psdPromedio[i] > maxPSD_v )
      {
         maxPSD_v = psdPromedio[i];
         maxPSD_i = i;
      }
    }
  
    double valorAC=maxPSD_v;
    double frecuenciaMaxima= maxPSD_i*(((samplingFrequency/2)/2)/(ventana/2));
    //Serial.print("El valor AC es: ");
    //Serial.println(valorAC);
    
    //Serial.print("la frecuencia cardíaca es: ");
    //Serial.println(frecuenciaMaxima*60);
      


    *direccion_valorDC=valorDC;
    *direccion_frecuenciaCardiaca=frecuenciaCardiaca;
    *direccion_valorAC=valorAC;
    *direccion_frecuenciaCardiacaPSD=frecuenciaMaxima*60;
    
    
}

void eliminarPicos(float senal[512]){
    
    //printf("Senal de entrada \n");
    int i;
    for (i=0;i<512;i++){
        senalEliminandoPicos[i]=senal[i];
        /*printf("%d ",i);
        printf("  ");
        printf("%.1f \n",senalEliminandoPicos[i]);*/
    }



    int numeroDatos=512;

    float p[512];
    for (i=0;i<512;i++){
        p[i]=0;
    }

    float atraz;
    float adelante;
    for (i=1;i<(numeroDatos-1);i++){
        atraz=fabs(senalEliminandoPicos[i]-senalEliminandoPicos[i-1]);
        adelante=fabs(senalEliminandoPicos[i+1]-senalEliminandoPicos[i]);
        if (atraz<adelante){
            p[i]=atraz;
        }
        else
        {
            p[i]=adelante;
        }
    }
    
    float ubicacionPicos[512];
    for (i=0;i<512;i++){
        ubicacionPicos[i]=0;
    }

    float threshold=500;
    for (i=0;i<512;i++){
        if (p[i]>threshold){
            ubicacionPicos[i]=1;
        }
        
    }
    /*
    for (i=0;i<512;i++){
        printf("%d ",i);
        printf("  ");
        printf("%.1f \n",ubicacionPicos[i]);
    }
    */
    numeroPicosEncontrados=0;
    for (i=0;i<512;i++){
        if (ubicacionPicos[i]==1){
            senalEliminandoPicos[i]=senalEliminandoPicos[i-1];
            numeroPicosEncontrados=numeroPicosEncontrados+1;
            //Serial.println("pico eliminado");
        }        
    }

    for (i=0;i<512;i++){
        //senal[i]=senalEliminandoPicos[i];  /// QUITAR COMENTARIO
        //printf("%d ",i);
        //printf("  ");
        //printf("%.1f \n",senalEliminandoPicos[i]);


    }

}

void enviarDatosServidor(float SENAL_ENVIAR[256], float frecuencia_cardiaca_enviar, float oximetria_enviar, int numero_picos_enviar, float BalanceIRRDC_enviar){


    //Serial.print("enviando datos");
    
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
        
        //int frecuencia_cardiaca = doc["frecuenciaCardiaca"].as<int>();
        //Serial.print("la frecuencia cardíaca es");
        //Serial.println(frecuencia_cardiaca);
        

        // -------- SOBREESCRIBIR DATOS EN BASE DE DATOS
        
        client.addHeader("Content-Type", "application/json");
        
        for (int j=0;j<256;j++){doc["pletismografia"]["senal"][j]=int(SENAL_ENVIAR[j]);}
        doc["frecuenciaCardiaca"] = int(frecuencia_cardiaca_enviar);
        doc["oximetria"] = int(oximetria_enviar);
        doc["NumeroPicos"]=int(numero_picos_enviar);
        doc["BalanceIRRDC"]=int(BalanceIRRDC_enviar);

        String message = "";
        serializeJson(doc, message);
        //Serial.print(message);
        httpCode = client.PUT(message);
    }  
    //Serial.print("El tiempo transcurrido es:");
    //Serial.print(micros()-tiempo);

}

void lecturaDatosMAX30102(){
    int i;
    if (tomarDatos==true){
        // LECTURA MAX30102
        particleSensor.check();
        while (particleSensor.available()){ //do we have new data?
            punteroValorSensado=punteroValorSensado+1;
            for (i=0;i<511;i++){senalAnalizarIR[i]=senalAnalizarIR[i+1];}
            //senalAnalizarIR[511]=senalIR[punteroValorSensado];
            senalAnalizarIR[511]=float(particleSensor.getFIFOIR());
            for (i=0;i<511;i++){senalAnalizarRojo[i]=senalAnalizarRojo[i+1];}
            //senalAnalizarRojo[511]=senalRojo[punteroValorSensado];
            senalAnalizarRojo[511]=float(particleSensor.getFIFORed());
            particleSensor.nextSample();
            muestrasRepresadas++;    
        }
        Serial.print("el número de muestras represadas fué: ");
        Serial.println(muestrasRepresadas);
        muestrasRepresadas=0;  

        // ACTUALIZACIÓN DE INTENSIDAD DE LEDS
        
        if (disminuirIntensidadIR==true){
            ledBrightnessIR=ledBrightnessIR-0x01;
            particleSensor.setPulseAmplitudeIR(ledBrightnessIR);
            disminuirIntensidadIR=false;
        }
        if (aumentarIntensidadIR==true){
            ledBrightnessIR=ledBrightnessIR+0x01;
            particleSensor.setPulseAmplitudeIR(ledBrightnessIR);
            aumentarIntensidadIR=false;
        }

    }
    else{
        Serial.print("aún no se inicia ejecución de tarea");
    }
    
}

void envioDatosPantalla(){
    int i;
    u8g2.clearBuffer();
    int OLEDvalorActual;
    int OLEDvalorAnterior=37;
    if (tomarDatos==true){
        for (i=128;i<256;i++){
            OLEDvalorActual=int(map(imprimirOLED[i], -1200, 1000, 0, 40));
            OLEDvalorActual=55-OLEDvalorActual;
            u8g2.drawLine(i-128,OLEDvalorAnterior,i+1-128,OLEDvalorActual);
            OLEDvalorAnterior=OLEDvalorActual;
        }
        u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
        u8g2.drawStr(0,10,"Fc= ");	// write something to the internal memory
        u8g2.setCursor(20,10);
        if (resultadoFrecuenciaCardiaca>30){
            u8g2.print(u8x8_u8toa(resultadoFrecuenciaCardiaca, 3));    
        }
        else{
            u8g2.print(" --");   
        }
        u8g2.drawStr(40,10,"bpm");
        u8g2.drawStr(70,10,"SpO2= ");  // write something to the internal memory
        u8g2.setCursor(105,10);
        if (resultadoSpo2>60){
            u8g2.print(u8x8_u8toa(resultadoSpo2, 2));
        }
        else {
            u8g2.print("---");
        }
        u8g2.drawStr(120,10,"%");
        u8g2.setCursor(0,60);
        u8g2.print(u8x8_u8toa(numeroPicosEncontrados, 2));
        u8g2.sendBuffer();
        u8g2.setCursor(80,60);
        u8g2.print(u8x8_u8toa((valorDCIR-valorDCRojo)*1000/valorDCIR, 3));
        u8g2.setCursor(110,60);
        u8g2.print(u8x8_u8toa(int(ledBrightnessIR),3));
        u8g2.sendBuffer();
    }

}
