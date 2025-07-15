# ESP32-Tic-Tac-Toe-Server

## 🚀 Project Overview

This project transforms a low-cost ESP32 microcontroller into a standalone web server that hosts an interactive Tic-Tac-Toe game. The ESP32 creates its own Wi-Fi network (acting as a Soft Access Point), allowing any device with a web browser to connect, navigate to the local server IP, and play the game.

This is a great example of a full-stack embedded application, demonstrating skills in hardware programming, network communication, and responsive web development.

![Tic-Tac-Toe Screenshot](https://github.com/user-attachments/assets/6bba10fd-53e2-4db1-b238-fac361b31f36)


---

## ✨ Key Features

* **Autonomous Web Server:** The ESP32 hosts the entire application; no external server or internet connection is required.
* **Wi-Fi Access Point:** The ESP32 broadcasts its own Wi-Fi network for devices to connect to.
* **Real-time Game State:** The game board updates in real-time on all connected clients.
* **Full Game Logic:** Includes logic to handle player turns, check for win conditions (rows, columns, and diagonals), and detect a draw.
* **Responsive UI:** The web interface is built with HTML, CSS, and JavaScript, ensuring it's playable on both desktop and mobile browsers.

---

## 🛠️ Technologies Used

### Hardware
* **Microcontroller:** ESP32 (e.g., NodeMCU-32S, ESP32-WROOM-32)

### Software & Libraries
* **Development Environment:** Arduino IDE
* **Programming Languages:** C++, HTML5, CSS3, JavaScript
* **C++ Libraries:**
    * `WiFi.h` (for network management)
    * `WebServer.h` (to create the web server)
    * `ArduinoJson.h` (for robust JSON data handling)

---

## ⚙️ How It Works

1.  **Wi-Fi AP:** The ESP32 is configured as a Wi-Fi Access Point with the SSID "ESP32-TicTacToe".
2.  **Web Server:** It runs a web server on port 80.
3.  **Frontend:** The browser loads the game's HTML, CSS, and JavaScript from the ESP32.
4.  **Client-Server Communication:**
    * When a player clicks a cell, the JavaScript sends a `POST` request to the `/move` endpoint on the ESP32.
    * The ESP32's C++ code updates the `gameState` and replies.
    * The client's JavaScript periodically sends a `GET` request to the `/state` endpoint to fetch the updated game board in JSON format.
    * The `ArduinoJson` library is used to reliably format and send the game state data.

---

## 💻 Setup and Installation

Follow these steps to get the project running on your ESP32:

1.  **Install Arduino IDE:** If you don't have it, download and install the Arduino IDE.
2.  **Add ESP32 Board Manager:** Go to **File > Preferences** and add the following URL to "Additional Boards Manager URLs":
    `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3.  **Install ESP32 Boards:** Go to **Tools > Board > Boards Manager**, search for `esp32`, and install the `esp32` package.
4.  **Install Required Libraries:** Go to **Sketch > Include Library > Manage Libraries...** and install the `ArduinoJson` library.
5.  **Connect Your ESP32:** Plug your ESP32 board into your computer. In the Arduino IDE, go to **Tools > Board** and select your specific ESP32 model. Also, select the correct COM port under **Tools > Port**.
6.  **Upload the Code:** Copy the full C++ code into the Arduino IDE and upload it to your ESP32.

---

## 🎮 How to Play

1.  Connect your phone or computer to the Wi-Fi network named **"ESP32-TicTacToe"**.
2.  The password is **"12345678"**.
3.  Open a web browser and navigate to the IP address **`http://192.168.4.1`**.
4.  The game interface will load, and you can start playing!

---

## 👤 Author

* [**Niranjan H**](https://github.com/codebyniranjan) - GitHub
* [**Niranjan H**](https://www.linkedin.com/in/niranjanh088/) - LinkedIn

---
