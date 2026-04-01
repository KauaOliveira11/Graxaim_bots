# Graxaim_bots
# 🤖 ESP32 + Android Gyroscope Control

Projeto de comunicação em tempo real entre um aplicativo Android e um ESP32 utilizando Wi-Fi.
O app envia dados do giroscópio do celular para o ESP32, que recebe e processa via servidor HTTP.

---

## 🚀 Funcionalidades

* 📡 ESP32 funcionando como Access Point (Wi-Fi próprio)
* 📱 Aplicativo Android com interface em Jetpack Compose
* 🎮 Envio de dados do giroscópio em tempo real
* 🔍 Sistema completo de debug e diagnóstico
* ✅ Teste de conexão via rota `/ping`
* 📊 Logs em tempo real no app e no Serial Monitor

---

## 🧠 Arquitetura

```
Celular (App Android)
        ↓ Wi-Fi
ESP32 (Servidor HTTP + AP)
        ↓
Processamento / Robô
```

---

## 📦 Tecnologias utilizadas

* ESP32 (Arduino Framework)
* WiFi.h / WebServer.h
* Android (Kotlin + Jetpack Compose)
* HTTP (GET)

---

## 🔧 Configuração do ESP32

1. Abra o código no Arduino IDE
2. Faça upload para o ESP32
3. Abra o Serial Monitor (115200 baud)

Após iniciar, será exibido:

```
ESP32 AP iniciado!
IP: 192.168.4.1
```

---

## 📶 Conexão

1. No celular, conecte no Wi-Fi:

```
Nome: ESP_ROBO
Senha: 12345678
```

2. Teste no navegador:

```
http://192.168.4.1/ping
```

Resposta esperada:

```json
{
  "status": "ok",
  "ip": "192.168.4.1",
  "clientes": "1"
}
```

---

## 📱 Uso do App

### ▶️ Testar conexão

* Clique em **"Testar Conexão"**
* Status deve mudar para:

```
Conectado ✅
```

---

### 🎮 Ativar giroscópio

* Clique em **"Ativar Giroscópio"**
* Mova o celular

---

### 📊 Resultado no ESP32 (Serial Monitor)

```
GYRO OK
X: 0.12 | Y: -1.45 | Z: 0.33
```

---

## 🔍 Sistema de Debug

### No ESP32:

* `/ping` → testa conexão
* Mostra IP do cliente
* Alerta se nenhum celular conectado
* Alerta se parar de receber dados

---

### No App:

* Status de conexão em tempo real
* Logs de envio e erro
* Feedback visual (conectado / erro)

---

## ❗ Possíveis Problemas

### ❌ Não conecta

* Verifique se está conectado ao Wi-Fi `ESP_ROBO`

### ❌ Erro de conexão

* Teste no navegador `/ping`

### ❌ Sem resposta do ESP

* Verifique o Serial Monitor
