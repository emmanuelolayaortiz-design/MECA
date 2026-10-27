#include <WiFi.h>
#include <WebServer.h>

// Credenciales de red que emitirá el ESP32:
const char* ssid = "MECA";
const char* password = "";

// Asignación de pines para el Puente H L298N:
const int IN1 = 26; // Motor Derecho - Dirección A
const int IN2 = 25; // Motor Derecho - Dirección B

const int IN3 = 17; // Motor Izquierdo - Dirección A
const int IN4 = 16; // Motor Izquierdo - Dirección B

// Inicializador del servidor web en el puerto 80:
WebServer server(80);

// Página HTML de control (Interfaz de la web):

const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Panel de Control - Artemis</title>
    <style>

        body { font-family: Arial, sans-serif; text-align: center; background-color: #121212; color: #ffffff; margin: 0; padding: 20px; }
        h1 { color: white; margin-bottom: 30px; }
        .btn-container { display: grid; grid-template-columns: repeat(3, 1fr); gap: 15px; max-width: 300px; margin: 0 auto; }
        .btn-flex { display: flex; width: auto; justify-content: space-between; gap: 0.5rem; aling-items: center; }
        .btn { background-color: #4f4f4f; color: white; border: none; padding: 20px; font-size: 18px; font-weight: bold; border-radius: 10px; cursor: pointer; touch-action: manipulation; }
        .btn:active { background-color: #1565c0; }
        .btn-stop { background-color: #e53935; width: 300px; grid-column: span 3; }
        .empty { visibility: hidden; }

    </style>
</head>
<body>
    <h1>ARTEMIS CONTROL</h1>
    <div class="btn-container">

        <div class="btn-flex">
          <button class="btn" id="btn-izquierda" onclick="sendCommand('/left')">&#9664;<br>IZQ</button>
          <button class="btn" id="btn-adelante" onclick="sendCommand('/forward')">&#9650;<br>ADELANTE</button>
          <button class="btn" id="btn-derecha" onclick="sendCommand('/right')">&#9654;<br>DER</button>
        </div>
        
        <button class="btn btn-stop" id="btn-parar" onclick="sendCommand('/stop')">DETENER</button>
        
        <button class="btn" id="btn-atras" onclick="sendCommand('/backward')">&#9660;<br>ATRAS</button>

    </div>

    <script>
      function sendCommand(path) {
          fetch(path).catch(err => console.log(err));
      }

      // Mapeo de teclas con los ID de los botones y la acción que ejecuta:
      const teclasValidas = {
        'w': { id: 'btn-adelante', accion: '/forward' },
        'a': { id: 'btn-izquierda', accion:'/left' },
        's': { id: 'btn-atras', accion: '/backward' },
        'd': { id: 'btn-derecha', accion: '/right' },
        'W': { id: 'btn-adelante', accion: '/forward' },
        'A': { id: 'btn-izquierda', accion:'/left' },
        'S': { id: 'btn-atras', accion: '/backward' },
        'D': { id: 'btn-derecha', accion: '/right' },
      };

      // Detectar cuando se presiona la tecla (JAVASCRIPT):
      document.addEventListener('keydown', function(event) {
        const configuracion = teclasValidas[event.key];

        if (configuracion) {
          const boton = document.getElementById(configuracion.id);

          // Se evita enviar repeticiones del mismo comando mientras la tecla está hundida
          if (!event.repeat) {
            boton.classList.add('active'); //Mismo efecto de presionar la tecla
            sendCommand(configuracion.accion); //Envía la petición a EPS32
          }

        }

      });

      // Detectar cuando se deja de presionar la tecla (JAVASCRIPT):
      document.addEventListener('keyup', function(event) {
        const configuracion = teclasValidas[event.key];
        if (configuracion) {
          const boton = document.getElementById(configuracion.id);
          boton.classList.remove('active');

          //Si se deja de presionar la tecla, se activa el STOP:
          sendCommand('/stop')

        }
      });

    </script>
</body>
</html>
)rawliteral";

// Definición de funciones de movimiento:
void detendAll() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

void handleForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  Serial.println("Comando recibido: ADELANTE");
  server.send(200, "text/plain", "OK");
}

void handleBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  Serial.println("Comando recibido: ATRAS");
  server.send(200, "text/plain", "OK");
}

void handleLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  Serial.println("Comando recibido: GIRO IZQUIERDA");
  server.send(200, "text/plain", "OK");
}

void handleRight() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  Serial.println("Comando recibido: GIRO DERECHA");
  server.send(200, "text/plain", "OK");
}

void handleStop() {
  detendAll();
  Serial.println("Comando recibido: DETENER");
  server.send(200, "text/plain", "OK");
}

void setup() {
  // Inicia comunicación con la computadora a 115200 baudios:
  Serial.begin(115200);
  Serial.println("\n--- Iniciando Sistema Artemis ---");

  // Configurar pines como salidas digitales:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  detendAll(); // Asegurar motores apagados al iniciar

  // Configuración del ESP32 como Punto de Acceso:
  WiFi.softAP(ssid, password);

  // Se muestra información de la conexión en la consola:
  Serial.print("Red Wi-Fi creada con exito. Nombre:");
  Serial.println(ssid);
  Serial.print("Dirección IP de control:");
  Serial.println(WiFi.softAPIP());

  //Definir rutas del servidor web:
  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/stop", handleStop);

  server.begin();
  Serial.println("Servidor Web Iniciado.");



}

void loop() {
  // Código que se repite:

  server.handleClient(); // Atender peticiones de la red

}
