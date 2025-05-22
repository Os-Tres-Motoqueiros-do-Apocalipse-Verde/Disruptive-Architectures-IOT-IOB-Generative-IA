#include <WiFi.h>
#include <WebServer.h>

// Configurações do ponto de acesso
const char* ssid = "Os tres Cavaleiros do Apocalipse Verde";
const char* password = "maldita-mottu";

WebServer server(80);

// Página HTML com JavaScript para pegar localização
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head><title>Enviar Localizacao Manual</title></head>
<body>
  <h2>Enviar Localizacao para o ESP32</h2>
  <p>Abra o Google Maps, pressione no mapa, copie latitude e longitude, e cole abaixo:</p>

  <input type="text" id="lat" placeholder="Latitude"><br><br>
  <input type="text" id="lon" placeholder="Longitude"><br><br>
  <button onclick="enviarLocalizacao()">Enviar</button>

  <script>
    function enviarLocalizacao() {
      const lat = document.getElementById("lat").value;
      const lon = document.getElementById("lon").value;

      if (lat && lon) {
        fetch(`/location?lat=${lat}&lon=${lon}`)
          .then(() => alert("Localizacao enviada"))
          .catch(err => alert("Erro ao enviar: " + err.message));
      } else {
        alert("Preencha latitude e longitude.");
      }
    }
  </script>
</body>
</html>
)rawliteral";


// Handler para página inicial
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// Handler para localização recebida
void handleLocation() {
  if (server.hasArg("lat") && server.hasArg("lon")) {
    String latitude = server.arg("lat");
    String longitude = server.arg("lon");

    Serial.println("Localização recebida:");
    Serial.println("Latitude: " + latitude);
    Serial.println("Longitude: " + longitude);

    server.send(200, "text/plain", "Localização recebida com sucesso!");
  } else {
    server.send(400, "text/plain", "Parâmetros ausentes.");
  }
}


void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  Serial.print("AP iniciado. IP: ");
  Serial.println(WiFi.softAPIP());

  // Define rotas
  server.on("/", handleRoot);
  server.on("/location", handleLocation);

  server.begin();
  Serial.println("Servidor iniciado.");
}

void loop() {
  server.handleClient();
}
