#include <WiFi.h>
#include <WebServer.h>

// Dados do WiFi
const char* ssid = "Husky";
const char* password = "kaua12345";

WebServer server(80);

// Declara função
void receberGyro();

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Servidor ESP32</h1>";
  html += "<p>Status: ONLINE</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
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

  server.begin();
}

void loop() {
  server.handleClient();
}

// 👇 Função de receber dados
void receberGyro() {
  String x = server.arg("x");
  String y = server.arg("y");
  String z = server.arg("z");

  Serial.println("------ GYRO ------");
  Serial.print("X: ");
  Serial.println(x);
  Serial.print("Y: ");
  Serial.println(y);
  Serial.print("Z: ");
  Serial.println(z);

  Serial.println("------------------");

  server.send(200, "text/plain", "OK");
}