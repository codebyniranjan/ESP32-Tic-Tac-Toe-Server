#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "ESP32-TicTacToe";
const char* password = "12345678";

WebServer server(80);

String gameState = "---------";
bool playerTurn = true;

char checkWinner() {
  int wins[8][3] = {{0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6}};
  for (int i = 0; i < 8; i++) {
    if (gameState[wins[i][0]] != '-' &&
        gameState[wins[i][0]] == gameState[wins[i][1]] &&
        gameState[wins[i][1]] == gameState[wins[i][2]]) {
      return gameState[wins[i][0]];
    }
  }
  for (int i = 0; i < 9; i++) {
    if (gameState[i] == '-') {
      return '-';
    }
  }
  return 'D';
}

const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Tic-Tac-Toe</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    :root {
      --bg-color: #212121;
      --board-color: #2c2c2c;
      --x-color: #ff5252;
      --o-color: #4fc3f7;
      --text-color: #e0e0e0;
      --turn-x-color: #ff8a80;
      --turn-o-color: #81d4fa;
    }
    body {
      background-color: var(--bg-color);
      color: var(--text-color);
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
      margin: 0;
      text-align: center;
      padding: 20px;
    }
    .container {
      background-color: var(--board-color);
      padding: 20px;
      border-radius: 15px;
      box-shadow: 0 10px 30px rgba(0, 0, 0, 0.4);
      max-width: 400px;
      width: 90%;
    }
    h1 {
      font-size: 2.5em;
      margin-bottom: 0.5em;
      text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
    }
    #status {
      font-size: 1.5em;
      font-weight: bold;
      margin-bottom: 20px;
      min-height: 25px;
    }
    #board {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 10px;
      width: 100%;
      aspect-ratio: 1 / 1;
      margin: 0 auto;
    }
    .cell {
      background-color: #383838;
      border-radius: 10px;
      display: flex;
      align-items: center;
      justify-content: center;
      cursor: pointer;
      font-size: 5em;
      font-weight: bold;
      transition: background-color 0.2s, transform 0.2s;
    }
    .cell:hover {
      background-color: #4a4a4a;
    }
    .cell.x-player {
      color: var(--x-color);
    }
    .cell.o-player {
      color: var(--o-color);
    }
    button {
      background-color: #616161;
      color: var(--text-color);
      border: none;
      border-radius: 8px;
      padding: 12px 24px;
      font-size: 1em;
      cursor: pointer;
      margin-top: 20px;
      transition: background-color 0.2s, transform 0.2s;
    }
    button:hover {
      background-color: #757575;
      transform: translateY(-2px);
    }
    .x-turn {
      color: var(--turn-x-color);
    }
    .o-turn {
      color: var(--turn-o-color);
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>Tic-Tac-Toe</h1>
    <div id="status"></div>
    <div id="board"></div>
    <button onclick="resetGame()">Reset Game</button>
  </div>
  <script>
    const boardDiv = document.getElementById('board');
    const statusDiv = document.getElementById('status');
    let winner = null;

    function updateBoard(state) {
      boardDiv.innerHTML = '';
      const cells = state.state;
      winner = state.winner;

      if (winner === 'X' || winner === 'O') {
        statusDiv.innerText = `${winner} wins!`;
      } else if (winner === 'D') {
        statusDiv.innerText = "It's a draw!";
      } else {
        statusDiv.innerHTML = `It's <span class="${state.turn === 'X' ? 'x-turn' : 'o-turn'}">${state.turn}'s</span> turn`;
      }

      for (let i = 0; i < 9; i++) {
        const cell = document.createElement('div');
        cell.className = 'cell';
        cell.innerText = cells[i] === '-' ? '' : cells[i];
        if (cells[i] === 'X') {
            cell.classList.add('x-player');
        } else if (cells[i] === 'O') {
            cell.classList.add('o-player');
        }
        cell.onclick = () => {
          if (winner === '-' && cells[i] === '-') { // Only allow clicks on empty cells
            makeMove(i);
          }
        };
        boardDiv.appendChild(cell);
      }
    }

    function fetchState() {
      fetch('/state')
        .then(res => res.json())
        .then(updateBoard)
        .catch(err => console.error('Error fetching state:', err));
    }

    function makeMove(cell) {
      fetch('/move', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'cell=' + cell
      }).then(res => {
          // Check for a successful response before fetching the new state
          if (res.ok) {
              fetchState();
          } else {
              console.error('Failed to make move.');
          }
      }).catch(err => console.error('Error making move:', err));
    }

    function resetGame() {
      fetch('/move', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'cell=-1'
      }).then(() => {
          fetchState();
      });
    }

    setInterval(fetchState, 1000);
    fetchState();
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.println("Access Point: " + String(ssid));
  Serial.println("IP Address: " + WiFi.softAPIP().toString());

  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", MAIN_page);
  });

  server.on("/state", HTTP_GET, []() {
    char winner = checkWinner();
    StaticJsonDocument<100> doc;
    doc["state"] = gameState;
    doc["turn"] = (playerTurn ? "X" : "O");
    doc["winner"] = String(winner);
    String jsonResponse;
    serializeJson(doc, jsonResponse);
    server.send(200, "application/json", jsonResponse);
  });

  server.on("/move", HTTP_POST, []() {
    // Debugging: Print all received arguments
    Serial.println("Received POST request on /move");
    if (server.args() > 0) {
        Serial.print("Number of arguments: ");
        Serial.println(server.args());
        for (int i = 0; i < server.args(); i++) {
            Serial.print("Arg " + String(i) + ": ");
            Serial.print(server.argName(i));
            Serial.print(" = ");
            Serial.println(server.arg(i));
        }
    } else {
        Serial.println("No arguments received.");
    }
    
    String cellArg = server.arg("cell");
    int cell = -2; // Use a default invalid value
    if (cellArg.length() > 0) {
        cell = cellArg.toInt();
    }
    
    Serial.print("Parsed cell value: ");
    Serial.println(cell);

    if (cell == -1) {
      Serial.println("Resetting game.");
      gameState = "---------";
      playerTurn = true;
      server.send(200, "text/plain", "reset");
    } else {
      char winner = checkWinner();
      if (winner == '-' && cell >= 0 && cell < 9 && gameState[cell] == '-') {
        Serial.print("Valid move. Updating cell ");
        Serial.println(cell);
        gameState[cell] = playerTurn ? 'X' : 'O';
        playerTurn = !playerTurn;
        server.send(200, "text/plain", "ok");
      } else {
        Serial.print("Invalid move. Cell: ");
        Serial.print(cell);
        Serial.print(", Winner: ");
        Serial.println(winner);
        server.send(400, "text/plain", "invalid move");
      }
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();
}