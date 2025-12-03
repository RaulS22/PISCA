#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp;

// --- CONFIGURAÇÃO DO WIFI ---
const char* ssid = "WIFI";  //Nome do Hotspot ou rede
const char* password = "SENHA"; // Senha do Hotspot ou rede


// --- ENDEREÇO DO SEU SERVIDOR NODE.JS ---
const char* serverURL = "http://SEUIP:3000/sensor/dados"; 
// TROQUE o IP acima pelo IP DO SEU PC NO HOTSPOT DO CELULAR OU REDE
// TROQUE a porta caso não use a 3000


void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA, SCL

  WiFi.mode(WIFI_STA);
  delay(500);
  WiFi.begin(ssid, password);
  
  Serial.print("Status: ");
  Serial.println(WiFi.status());
  
  Serial.println("Conectando ao WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✔ Conectado ao WiFi!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());

  // ---- INICIA SENSORES ----
  Serial.println("Inicializando sensores...");

  if (!aht.begin()) {
    Serial.println("❌ Erro AHT20!");
    while (1) delay(10);
  }
  Serial.println("✔ AHT20 OK!");

  if (!bmp.begin(0x77)) {
    Serial.println("❌ Erro BMP280!");
    while (1) delay(10);
  }
  Serial.println("✔ BMP280 OK!");

  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500
  );
}

void loop() {
  sensors_event_t humidity, tempAHT;

  // Lê AHT20
  aht.getEvent(&humidity, &tempAHT);

  // Lê BMP280
  float temperatureBMP = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F; // em hPa
  float altitude = bmp.readAltitude(1013.25);

  Serial.println("---- SENSOR READINGS ----");
  Serial.printf("Temp AHT20: %.2f °C\n", tempAHT.temperature);
  Serial.printf("Umidade: %.2f %%\n", humidity.relative_humidity);
  Serial.printf("Pressão: %.2f hPa\n", pressure);
  Serial.printf("Altitude: %.2f m\n\n", altitude);

  // -------- ENVIA DADOS PARA O SERVIDOR --------
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");

    String json = "{";
    json += "\"temperatura\":" + String(tempAHT.temperature, 2) + ",";
    json += "\"umidade\":" + String(humidity.relative_humidity, 2) + ",";
    json += "\"pressao\":" + String(pressure, 2) + ",";
    json += "\"altitude\":" + String(altitude, 2);
    json += "}";

    Serial.println("Enviando JSON:");
    Serial.println(json);

    int status = http.POST(json);

    Serial.print("Status do servidor: ");
    Serial.println(status);

    if (status > 0) {
      String resposta = http.getString();
      Serial.println("Resposta:");
      Serial.println(resposta);
    } else {
      Serial.println("❌ Erro ao enviar POST");
    }

    http.end();
  } else {
    Serial.println("❌ WiFi desconectado!");
  }

  delay(30000); // envia a cada 30s
}
