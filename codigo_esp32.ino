#include <WiFi.h>
#include <WebServer.h>

//  ESP32 vira Wi-Fi
const char* ssid = "ESP_ROBO";
const char* password = "12345678";

WebServer server(80);

unsigned long ultimoPacote = 0;
const int timeout = 2000;

// ================= ROOT =================
void handleRoot() {
  server.send(200, "text/plain", "ESP32 ONLINE");
}

// ================= PING =================
void handlePing() {
  Serial.println(" Ping recebido!");

  String resposta = "{";
  resposta += "\"status\":\"ok\",";
  resposta += "\"ip\":\"" + WiFi.softAPIP().toString() + "\",";
  resposta += "\"clientes\":\"" + String(WiFi.softAPgetStationNum()) + "\"";
  resposta += "}";

  server.send(200, "application/json", resposta);
}

// ================= GYRO =================
void receberGyro() {

  Serial.print(" Cliente: ");
  Serial.println(server.client().remoteIP());

  Serial.print(" URL: ");
  Serial.println(server.uri());

  if (!server.hasArg("x") || !server.hasArg("y") || !server.hasArg("z")) {
    server.send(400, "text/plain", "ERRO: parametros faltando");
    Serial.println(" ERRO: faltando parametros");
    return;
  }

  float x = server.arg("x").toFloat();
  float y = server.arg("y").toFloat();
  float z = server.arg("z").toFloat();

  if (isnan(x) || isnan(y) || isnan(z)) {
    server.send(400, "text/plain", "ERRO: valores invalidos");
    Serial.println(" ERRO: dados invalidos");
    return;
  }

  if (abs(x) > 50 || abs(y) > 50 || abs(z) > 50) {
    server.send(400, "text/plain", "ERRO: valores fora do limite");
    Serial.println(" ERRO: valores fora do limite");
    return;
  }

  ultimoPacote = millis();

  Serial.println("------ GYRO OK ------");
  Serial.printf("X: %.2f | Y: %.2f | Z: %.2f\n", x, y, z);
  Serial.println("---------------------");

  server.send(200, "text/plain", "OK");
}

// ================= ERRO =================
void handleNotFound() {
  Serial.println(" Rota inexistente acessada");
  server.send(404, "text/plain", "Rota nao encontrada");
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println(" ESP32 AP iniciado!");
  Serial.print(" IP: ");
  Serial.println(WiFi.softAPIP()); // normalmente 192.168.4.1

  server.on("/", handleRoot);
  server.on("/ping", handlePing);
  server.on("/gyro", receberGyro);
  server.onNotFound(handleNotFound);

  server.begin();
}

// ================= LOOP =================
void loop() {
  server.handleClient();

  if (WiFi.softAPgetStationNum() == 0) {
    Serial.println("⚠️ Nenhum celular conectado!");
    delay(1000);
  }

  if (millis() - ultimoPacote > timeout) {
    Serial.println("⚠️ SEM DADOS DO APP!");
    delay(1000);
  }
}#include <WiFi.h>
#include <WebServer.h>

//  ESP32 vira Wi-Fi
const char* ssid = "ESP_ROBO";
const char* password = "12345678";

WebServer server(80);

unsigned long ultimoPacote = 0;
const int timeout = 2000;

// ================= ROOT =================
void handleRoot() {
  server.send(200, "text/plain", "ESP32 ONLINE");
}

// ================= PING =================
void handlePing() {
  Serial.println(" Ping recebido!");

  String resposta = "{";
  resposta += "\"status\":\"ok\",";
  resposta += "\"ip\":\"" + WiFi.softAPIP().toString() + "\",";
  resposta += "\"clientes\":\"" + String(WiFi.softAPgetStationNum()) + "\"";
  resposta += "}";

  server.send(200, "application/json", resposta);
}

// ================= GYRO =================
void receberGyro() {

  Serial.print(" Cliente: ");
  Serial.println(server.client().remoteIP());

  Serial.print(" URL: ");
  Serial.println(server.uri());

  if (!server.hasArg("x") || !server.hasArg("y") || !server.hasArg("z")) {
    server.send(400, "text/plain", "ERRO: parametros faltando");
    Serial.println(" ERRO: faltando parametros");
    return;
  }

  float x = server.arg("x").toFloat();
  float y = server.arg("y").toFloat();
  float z = server.arg("z").toFloat();

  if (isnan(x) || isnan(y) || isnan(z)) {
    server.send(400, "text/plain", "ERRO: valores invalidos");
    Serial.println(" ERRO: dados invalidos");
    return;
  }

  if (abs(x) > 50 || abs(y) > 50 || abs(z) > 50) {
    server.send(400, "text/plain", "ERRO: valores fora do limite");
    Serial.println(" ERRO: valores fora do limite");
    return;
  }

  ultimoPacote = millis();

  Serial.println("------ GYRO OK ------");
  Serial.printf("X: %.2f | Y: %.2f | Z: %.2f\n", x, y, z);
  Serial.println("---------------------");

  server.send(200, "text/plain", "OK");
}

// ================= ERRO =================
void handleNotFound() {
  Serial.println(" Rota inexistente acessada");
  server.send(404, "text/plain", "Rota nao encontrada");
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println(" ESP32 AP iniciado!");
  Serial.print(" IP: ");
  Serial.println(WiFi.softAPIP()); // normalmente 192.168.4.1

  server.on("/", handleRoot);
  server.on("/ping", handlePing);
  server.on("/gyro", receberGyro);
  server.onNotFound(handleNotFound);

  server.begin();
}

// ================= LOOP =================
void loop() {
  server.handleClient();

  if (WiFi.softAPgetStationNum() == 0) {
    Serial.println("⚠️ Nenhum celular conectado!");
    delay(1000);
  }

  if (millis() - ultimoPacote > timeout) {
    Serial.println("⚠️ SEM DADOS DO APP!");
    delay(1000);
  }
}#include <WiFi.h>
#include <WebServer.h>

//  ESP32 vira Wi-Fi
const char* ssid = "ESP_ROBO";
const char* password = "12345678";

WebServer server(80);

unsigned long ultimoPacote = 0;
const int timeout = 2000;

// ================= ROOT =================
void handleRoot() {
  server.send(200, "text/plain", "ESP32 ONLINE");
}

// ================= PING =================
void handlePing() {
  Serial.println(" Ping recebido!");

  String resposta = "{";
  resposta += "\"status\":\"ok\",";
  resposta += "\"ip\":\"" + WiFi.softAPIP().toString() + "\",";
  resposta += "\"clientes\":\"" + String(WiFi.softAPgetStationNum()) + "\"";
  resposta += "}";

  server.send(200, "application/json", resposta);
}

// ================= GYRO =================
void receberGyro() {

  Serial.print(" Cliente: ");
  Serial.println(server.client().remoteIP());

  Serial.print(" URL: ");
  Serial.println(server.uri());

  if (!server.hasArg("x") || !server.hasArg("y") || !server.hasArg("z")) {
    server.send(400, "text/plain", "ERRO: parametros faltando");
    Serial.println(" ERRO: faltando parametros");
    return;
  }

  float x = server.arg("x").toFloat();
  float y = server.arg("y").toFloat();
  float z = server.arg("z").toFloat();

  if (isnan(x) || isnan(y) || isnan(z)) {
    server.send(400, "text/plain", "ERRO: valores invalidos");
    Serial.println(" ERRO: dados invalidos");
    return;
  }

  if (abs(x) > 50 || abs(y) > 50 || abs(z) > 50) {
    server.send(400, "text/plain", "ERRO: valores fora do limite");
    Serial.println(" ERRO: valores fora do limite");
    return;
  }

  ultimoPacote = millis();

  Serial.println("------ GYRO OK ------");
  Serial.printf("X: %.2f | Y: %.2f | Z: %.2f\n", x, y, z);
  Serial.println("---------------------");

  server.send(200, "text/plain", "OK");
}

// ================= ERRO =================
void handleNotFound() {
  Serial.println(" Rota inexistente acessada");
  server.send(404, "text/plain", "Rota nao encontrada");
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println(" ESP32 AP iniciado!");
  Serial.print(" IP: ");
  Serial.println(WiFi.softAPIP()); // normalmente 192.168.4.1

  server.on("/", handleRoot);
  server.on("/ping", handlePing);
  server.on("/gyro", receberGyro);
  server.onNotFound(handleNotFound);

  server.begin();
}

// ================= LOOP =================
void loop() {
  server.handleClient();

  if (WiFi.softAPgetStationNum() == 0) {
    Serial.println("⚠️ Nenhum celular conectado!");
    delay(1000);
  }

  if (millis() - ultimoPacote > timeout) {
    Serial.println("⚠️ SEM DADOS DO APP!");
    delay(1000);
  }
}