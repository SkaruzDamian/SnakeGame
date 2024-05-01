#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const int numRows = 2;
const int numCols = 16;
const int numScreens = 4;
byte lcdAddresses[numScreens] = {0x27, 0x26, 0x25, 0x24};
LiquidCrystal_I2C lcds[numScreens] = {
  {lcdAddresses[0], numCols, numRows},
  {lcdAddresses[1], numCols, numRows},
  {lcdAddresses[2], numCols, numRows},
  {lcdAddresses[3], numCols, numRows}
};

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int snakeLength = 3;
int snakeX[64], snakeY[64]; 
char currentDirection = 'R'; 

void setup() {
  for (int i = 0; i < numScreens; i++) {
    lcds[i].init();
    lcds[i].backlight();
    lcds[i].clear();
  }
  snakeX[0] = 1; snakeY[0] = 0;
  snakeX[1] = 0; snakeY[1] = 0;
}

void loop() {
  char key = keypad.getKey();
  if (key) handleInput(key);
  moveSnake();
  if (checkCollision()) resetGame();
  updateGameDisplay();
  delay(200);
}

void handleInput(char key) {
  switch (key) {
    case '2': if (currentDirection != 'D') currentDirection = 'U'; break;
    case '8': if (currentDirection != 'U') currentDirection = 'D'; break;
    case '4': if (currentDirection != 'R') currentDirection = 'L'; break;
    case '6': if (currentDirection != 'L') currentDirection = 'R'; break;
  }
}

void moveSnake() {
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }
  switch (currentDirection) {
    case 'U': snakeY[0]--; break;
    case 'D': snakeY[0]++; break;
    case 'L': snakeX[0]--; break;
    case 'R': snakeX[0]++; break;
  }
}

bool checkCollision() {
  if (snakeX[0] < 0 || snakeX[0] >= numCols || snakeY[0] < 0 || snakeY[0] >= numRows * numScreens / 2) return true;
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) return true;
  }
  return false;
}

void updateGameDisplay() {
  for (int i = 0; i < numScreens; i++) {
    lcds[i].clear();
  }
  for (int i = 0; i < snakeLength; i++) {
    int screenIndex = (snakeY[i] / numRows) * 2 + (snakeX[i] / numCols);
    int localX = snakeX[i] % numCols;
    int localY = snakeY[i] % numRows;
    lcds[screenIndex].setCursor(localX, localY);
    lcds[screenIndex].write('*');
  }
}

void resetGame() {
  snakeLength = 3;
  snakeX[0] = 1; snakeY[0] = 0;
  snakeX[1] = 0; snakeY[1] = 0;
  currentDirection = 'R';
  for (int i = 0; i < numScreens; i++) {
    lcds[i].clear();
    lcds[i].print("Game Over!");
  }
  delay(2000);
  for (int i = 0; i < numScreens; i++) {
    lcds[i].clear();
  }
}
