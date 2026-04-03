//Kaua Silva Olivera
//Engenharia de controle e automacao furg

#include <WiFi.h>
#include <WebServer.h>

// ================= CONFIG =================
#define MODO_SERIAL 2  
// 1 = DEBUG
// 2 = CSV

// ================= ORDEM DAS COISAS  QUE APARECEM NO SEIRAL MEU CHAPA ENRIENW =================
// x ; y ; z ; angle ; mx ; my ; mz ; mag
// | Posição | Nome  | Descrição                      |
// | ------- | ----- | ------------------------------ |
// | 1       | x     | Giroscópio eixo X              |
// | 2       | y     | Giroscópio eixo Y              |
// | 3       | z     | Giroscópio eixo Z              |
// | 4       | angle | Bússola (azimuth em graus)     |
// | 5       | mx    | Magnetômetro eixo X            |
// | 6       | my    | Magnetômetro eixo Y            |
// | 7       | mz    | Magnetômetro eixo Z            |
// | 8       | mag   | Intensidade do campo magnético |

//D A VOM OU QUER MAIS MEUS GRAXAMANOS
// SIM TO COM TEMPO PRA FAZER GRACA
// ================= WIFI =================
const char* ssid = "ESP_ROBO";
const char* password = "12345678";

WebServer server(80);

// ================= CONTROLE =================
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

  if (!server.hasArg("x") || !server.hasArg("y") || !server.hasArg("z") ||
      !server.hasArg("angle")) {
    server.send(400, "text/plain", "Parametros faltando");
    return;
  }

  float x = server.arg("x").toFloat();
  float y = server.arg("y").toFloat();
  float z = server.arg("z").toFloat();
  float angle = server.arg("angle").toFloat();

  float mx = server.arg("mx").toFloat();
  float my = server.arg("my").toFloat();
  float mz = server.arg("mz").toFloat();
  float mag = server.arg("mag").toFloat();

  ultimoPacote = millis();

  // ================= DEBUG =================
#if MODO_SERIAL == 1

  Serial.println("------ STATUS ------");
  Serial.printf("Gyro -> X: %.2f | Y: %.2f | Z: %.2f\n", x, y, z);
  Serial.printf("Bussola -> %.1f°\n", angle);
  Serial.printf("Mag -> X: %.2f | Y: %.2f | Z: %.2f | Campo: %.2f\n", mx, my, mz, mag);
  Serial.println("--------------------");

#endif

  // ================= CSV =================
#if MODO_SERIAL == 2

  // formato:
  // x;y;z;angle;mx;my;mz;mag
  Serial.printf("%.2f;%.2f;%.2f;%.1f;%.2f;%.2f;%.2f;%.2f\n",
                x, y, z, angle, mx, my, mz, mag);

#endif

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

  Serial.println("🚀 ESP32 INICIADO");
  Serial.print("📡 IP: ");
  Serial.println(WiFi.softAPIP());

#if MODO_SERIAL == 1
  Serial.println("🟢 MODO DEBUG ATIVO");
#else
  Serial.println("🟢 MODO CSV ATIVO");
#endif

  server.on("/", handleRoot);
  server.on("/ping", handlePing);
  server.on("/status", receberStatus);
  server.onNotFound(handleNotFound);

  server.begin();
}

// ================= LOOP =================
void loop() {
  server.handleClient();

  if (WiFi.softAPgetStationNum() == 0) {
    Serial.println("⚠️ Sem cliente conectado");
    delay(1000);
    return;
  }

  if (millis() - ultimoPacote > timeout) {
    Serial.println("⚠️ Sem dados do app");
    delay(1000);
  }
}