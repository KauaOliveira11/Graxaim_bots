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

  String resposta = "{";
  resposta += "\"status\":\"ok\",";
  resposta += "\"ip\":\"" + WiFi.softAPIP().toString() + "\",";
  resposta += "\"clientes\":\"" + String(WiFi.softAPgetStationNum()) + "\"";
  resposta += "}";

  server.send(200, "application/json", resposta);
}

// ================= STATUS =================
void receberStatus() {

  // 🔥 VALIDAÇÃO COMPLETA
  if (!server.hasArg("x") || !server.hasArg("y") || !server.hasArg("z") ||
      !server.hasArg("angle") ||
      !server.hasArg("mx") || !server.hasArg("my") || !server.hasArg("mz") ||
      !server.hasArg("mag")) {

    server.send(400, "text/plain", "ERRO: parametros faltando");
    Serial.println("❌ Parametros faltando");
    return;
  }

  // 🔥 LEITURA
  float x = server.arg("x").toFloat();
  float y = server.arg("y").toFloat();
  float z = server.arg("z").toFloat();
  float angle = server.arg("angle").toFloat();

  float mx = server.arg("mx").toFloat();
  float my = server.arg("my").toFloat();
  float mz = server.arg("mz").toFloat();
  float mag = server.arg("mag").toFloat();

  // 🔥 VALIDAÇÃO DE DADOS
  if (isnan(x) || isnan(y) || isnan(z) ||
      isnan(angle) ||
      isnan(mx) || isnan(my) || isnan(mz) || isnan(mag)) {

    server.send(400, "text/plain", "ERRO: dados invalidos");
    Serial.println("❌ Dados invalidos");
    return;
  }

  ultimoPacote = millis();

  // ================= PRINT BONITO =================
  Serial.println("\n====== 📡 STATUS RECEBIDO ======");

  Serial.printf("🎯 GYRO\n");
  Serial.printf("X: %.2f | Y: %.2f | Z: %.2f\n", x, y, z);

  Serial.printf("\n🧭 BUSSOLA\n");
  Serial.printf("Angulo: %.1f°\n", angle);

  if (angle >= 315 || angle < 45) {
    Serial.println("Direcao: NORTE ↑");
  } else if (angle >= 45 && angle < 135) {
    Serial.println("Direcao: LESTE →");
  } else if (angle >= 135 && angle < 225) {
    Serial.println("Direcao: SUL ↓");
  } else {
    Serial.println("Direcao: OESTE ←");
  }

  Serial.printf("\n🧲 MAGNETOMETRO\n");
  Serial.printf("X: %.2f | Y: %.2f | Z: %.2f\n", mx, my, mz);

  Serial.printf("\n⚡ CAMPO MAGNETICO\n");
  Serial.printf("Intensidade: %.2f µT\n", mag);

  Serial.println("================================\n");

  server.send(200, "text/plain", "OK");
}

// ================= ERRO =================
void handleNotFound() {
  Serial.print("❌ Rota invalida: ");
  Serial.println(server.uri());
  server.send(404, "text/plain", "Rota nao encontrada");
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println("🚀 ESP32 iniciado!");
  Serial.print("📡 IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/ping", handlePing);
  server.on("/status", receberStatus);

  server.onNotFound(handleNotFound);

  server.begin();
}

// ================= LOOP =================
void loop() {

  server.handleClient();

  // 🔥 SEM CLIENTE
  if (WiFi.softAPgetStationNum() == 0) {
    Serial.println("⚠️ Nenhum celular conectado!");
    delay(1000);
    return;
  }

  // 🔥 SEM DADOS
  if (millis() - ultimoPacote > timeout) {
    Serial.println("⚠️ SEM DADOS DO APP!");
    delay(1000);
  }
}