#include <WiFi.h>
#include <WebServer.h>

// ================= WIFI =================
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
  Serial.println("📡 Ping recebido!");

  String resposta = "{";
  resposta += "\"status\":\"ok\",";
  resposta += "\"ip\":\"" + WiFi.softAPIP().toString() + "\",";
  resposta += "\"clientes\":\"" + String(WiFi.softAPgetStationNum()) + "\"";
  resposta += "}";

  server.send(200, "application/json", resposta);
}

// ================= GYRO =================
void receberGyro() {

  if (!server.hasArg("x") || !server.hasArg("y") || !server.hasArg("z")) {
    server.send(400, "text/plain", "ERRO: parametros faltando");
    return;
  }

  float x = server.arg("x").toFloat();
  float y = server.arg("y").toFloat();
  float z = server.arg("z").toFloat();

  if (isnan(x) || isnan(y) || isnan(z)) {
    server.send(400, "text/plain", "ERRO: dados invalidos");
    return;
  }

  ultimoPacote = millis();

  Serial.println("------ GYRO ------");
  Serial.printf("X: %.2f | Y: %.2f | Z: %.2f\n", x, y, z);

  server.send(200, "text/plain", "OK");
}

// ================= BUSSOLA =================
void receberCompass() {

  if (!server.hasArg("angle")) {
    server.send(400, "text/plain", "ERRO: faltando angulo");
    return;
  }

  float angulo = server.arg("angle").toFloat();

  if (isnan(angulo)) {
    server.send(400, "text/plain", "ERRO: angulo invalido");
    return;
  }

  ultimoPacote = millis();

  Serial.println("------ BUSSOLA ------");
  Serial.printf("Angulo: %.1f°\n", angulo);

  // EXEMPLO: interpretar direção
  if (angulo >= 315 || angulo < 45) {
    Serial.println("Direcao: NORTE ↑");
  } else if (angulo >= 45 && angulo < 135) {
    Serial.println("Direcao: LESTE →");
  } else if (angulo >= 135 && angulo < 225) {
    Serial.println("Direcao: SUL ↓");
  } else {
    Serial.println("Direcao: OESTE ←");
  }

  server.send(200, "text/plain", "OK");
}

// ================= ERRO =================
void handleNotFound() {
  Serial.print("❌ Rota inexistente: ");
  Serial.println(server.uri());
  server.send(404, "text/plain", "Rota nao encontrada");
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println("🚀 ESP32 AP iniciado!");
  Serial.print("📡 IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/ping", handlePing);
  server.on("/gyro", receberGyro);
  server.on("/compass", receberCompass); // 🔥 NOVO
  server.onNotFound(handleNotFound);

  server.begin();
}

// ================= LOOP =================
void loop() {
  server.handleClient();

  // sem cliente conectado
  if (WiFi.softAPgetStationNum() == 0) {
    Serial.println("⚠️ Nenhum celular conectado!");
    delay(1000);
    return;
  }

  // sem dados do app
  if (millis() - ultimoPacote > timeout) {
    Serial.println("⚠️ SEM DADOS DO APP!");
    delay(1000);
  }
}