//Kaua Silva Oliveira
//Engenharia de contrlole e automcao FURG
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Husky";
const char* password = "kaua12345";

WebServer server(80);

unsigned long ultimoPacote = 0;
const int timeout = 2000; 

void handleRoot() {
  server.send(200, "text/plain", "ESP32 ONLINE");
}

void receberGyro() {

  if (!server.hasArg("x") || !server.hasArg("y") || !server.hasArg("z")) {
    server.send(400, "text/plain", "ERRO: parametros faltando");
    Serial.println("ERRO: faltando parametros");
    return;
  }

  String xStr = server.arg("x");
  String yStr = server.arg("y");
  String zStr = server.arg("z");

  float x = xStr.toFloat();
  float y = yStr.toFloat();
  float z = zStr.toFloat();

  if (isnan(x) || isnan(y) || isnan(z)) {
    server.send(400, "text/plain", "ERRO: valores invalidos");
    Serial.println("ERRO: dados invalidos");
    return;
  }

  if (abs(x) > 50 || abs(y) > 50 || abs(z) > 50) {
    server.send(400, "text/plain", "ERRO: valores fora do limite");
    Serial.println("ERRO: valores fora do limite");
    return;
  }

  ultimoPacote = millis();

  // ✔️ Print bonito
  Serial.println("------ GYRO OK ------");
  Serial.printf("X: %.2f | Y: %.2f | Z: %.2f\n", x, y, z);
  Serial.println("---------------------");

  server.send(200, "text/plain", "OK");
}

void handleNotFound() {
  Serial.println("Rota inexistente acessada");
  server.send(404, "text/plain", "Rota nao encontrada");
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/gyro", receberGyro);
  server.onNotFound(handleNotFound);

  server.begin();
}
void loop() {
  server.handleClient();

  //  Detecta perda de conexão (sem dados)
  if (millis() - ultimoPacote > timeout) {
    Serial.println("⚠️ SEM DADOS DO CELULAR!");
    delay(1000); 
  }
}