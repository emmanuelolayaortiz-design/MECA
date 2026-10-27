# Prototipo M.E.C.A "Módulo Excavadora de Control Automatizado"

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![ESP32](https://img.shields.io/badge/ESP32-000000?style=for-the-badge&logo=espressif&logoColor=white)
![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=for-the-badge&logo=html5&logoColor=white)
![CSS3](https://img.shields.io/badge/CSS3-1572B6?style=for-the-badge&logo=css3&logoColor=white)
![JavaScript](https://img.shields.io/badge/JavaScript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black)

El siguiente documento consolida la información técnica del código fuente y los datos académicos documentados en la ficha técnica del proyecto.

## 📖 Descripción del Proyecto
El presente proyecto consiste en la construcción de un prototipo denominado MECA, un módulo de excavadora automatizada a escala que simula procesos de la industria de maquinaria pesada. Su desarrollo permite aplicar conocimientos de algoritmos y programación, promoviendo la optimización y el control de procesos, además de servir como base para futuros aprendizajes en circuitos y robótica.

## 🎓 Información Académica
* **Integrantes / Autores:** Emmanuel Olaya Ortiz, Ximena Rivero Barón, María Natalia R.
* **Docente Asesor:** Natalia Alcalde Mejia.
* **Facultad:** Facultad de Ingenierías.
* **Programa Académico:** Ingeniería Industrial.
* **Semestre Cursado:** 1.
* **Semillero de Investigación:** Semillero Tecnovadora.
* **Grupo de Investigación:** Grupo de Investigación (G-3IN).
* **Modalidad:** Prototipo - INCIETEC 2026-2.

---

## ⚙️ Especificaciones Técnicas del Sistema (Artemis)
El código en C++ (Framework Arduino) estructura el controlador del prototipo (denominado "Artemis" en su interfaz) utilizando un microcontrolador ESP32 y un controlador de motores Puente H L298N.

### Configuración de Red (Punto de Acceso ESP32)
El microcontrolador genera su propia red Wi-Fi para que el usuario pueda conectarse de forma local e interactuar con el servidor web.
* **SSID (Nombre de red):** `MECA`
* **Contraseña:** *(Abierta / Sin contraseña)*
* **Puerto del Servidor:** 80

### Asignación de Pines (Puente H L298N)
La tracción diferencial del módulo se controla mediante cuatro pines de salida digital conectados a las direcciones de los motores:
* **Motor Derecho:**
  * Dirección A (IN1): `Pin 26`
  * Dirección B (IN2): `Pin 25`
* **Motor Izquierdo:**
  * Dirección A (IN3): `Pin 17`
  * Dirección B (IN4): `Pin 16`

### Interfaz Web y Control (Panel Artemis)
El ESP32 aloja una página web responsiva construida con **HTML**, **CSS** y **JavaScript** que actúa como control remoto. El panel incluye las siguientes características:
* **Botones Interfaz:** Controles en pantalla para `ADELANTE`, `ATRAS`, `IZQ`, `DER` y `DETENER`.
* **Control por Teclado:** Integración de *Event Listeners* en JavaScript para mapear las teclas a comandos de movimiento mediante solicitudes asíncronas (`fetch()`):
  * `W` o `w`: Hacia adelante (`/forward`)
  * `S` o `s`: Hacia atrás (`/backward`)
  * `A` o `a`: Giro a la izquierda (`/left`)
  * `D` o `d`: Giro a la derecha (`/right`)
* **Parada Automática:** El sistema cuenta con un evento `keyup` en JavaScript que envía la instrucción de detenerse (`/stop`) en el instante exacto en el que el usuario suelta cualquier tecla, garantizando un manejo seguro y preciso de los actuadores.
