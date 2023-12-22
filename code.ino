const int leftMotorPin1 = 2;   // Connect left motor pin 1 to digital pin 2
const int leftMotorPin2 = 3;   // Connect left motor pin 2 to digital pin 3
const int rightMotorPin1 = 4;  // Connect right motor pin 1 to digital pin 4
const int rightMotorPin2 = 5;  // Connect right motor pin 2 to digital pin 5

// Binary maze representation (0 for open path, 1 for wall)
int maze[7][7] = {
  {0, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 1, 0},
  {0, 0, 0, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0}
};

int steps[100];  // Array to store the generated steps for maze solving
int stepCount = 0;  // Variable to keep track of the current step

void setup() {
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  Serial.begin(9600);

  // Solve the maze during setup and generate the array of steps
  // (startX, startY, endX, endY, stop1X, stop1Y, stop2X, stop2Y)
  solveMaze(0, 0, 6, 6, 2, 2, 4, 4);
}

// Function to control the left motor
void setLeftMotor(int speed) {
  analogWrite(leftMotorPin1, speed);
  analogWrite(leftMotorPin2, 0);
}

// Function to control the right motor
void setRightMotor(int speed) {
  analogWrite(rightMotorPin1, speed);
  analogWrite(rightMotorPin2, 0);
}

// Function to stop both motors
void stopMotors() {
  analogWrite(leftMotorPin1, 0);
  analogWrite(leftMotorPin2, 0);
  analogWrite(rightMotorPin1, 0);
  analogWrite(rightMotorPin2, 0);
}

void loop() {
  // Check if there are more steps to execute
  if (stepCount < sizeof(steps) / sizeof(steps[0])) {
    // Execute the current step
    executeStep(steps[stepCount]);
    stepCount++;  // Move to the next step
  } else {
    // Maze solving is complete
    stopMotors();
    while (true) {
      // Loop indefinitely once the maze is solved
    }
  }
}

// Function to solve the maze using Depth-First Search
void solveMaze(int startX, int startY, int endX, int endY, int stop1X, int stop1Y, int stop2X, int stop2Y) {
  // Start DFS from the top-left corner
  dfs(startX, startY, endX, endY, stop1X, stop1Y, stop2X, stop2Y);
}

// Depth-First Search function to generate steps for maze solving
void dfs(int x, int y, int endX, int endY, int stop1X, int stop1Y, int stop2X, int stop2Y) {
  // Base case: if the current position is the destination
  if (x == endX && y == endY) {
    return;
  }

  // Mark the current cell as visited
  maze[x][y] = 2;  // Change the value to indicate visited

  // Define possible moves (up, down, left, right)
  int dx[] = {-1, 1, 0, 0};
  int dy[] = {0, 0, -1, 1};

  // Check if the robot should stop at the first predetermined point
  if (x == stop1X && y == stop1Y) {
    // Stop at the first predetermined point
    steps[stepCount++] = 4;  // Custom action (e.g., stop for a specific task)
    stopMotors();
    return;
  }

  // Explore all possible moves
  for (int i = 0; i < 4; ++i) {
    int nextX = x + dx[i];
    int nextY = y + dy[i];

    // Check if the next move is valid and not visited
    if (nextX >= 0 && nextX < 7 && nextY >= 0 && nextY < 7 && maze[nextX][nextY] == 0) {
      // Execute the move based on the direction
      if (i == 0) {
        steps[stepCount++] = 1;  // Move forward
      } else if (i == 1) {
        steps[stepCount++] = 3;  // Turn right
      } else if (i == 2) {
        steps[stepCount++] = 2;  // Turn left
      }

      // Recursively explore the next move
      dfs(nextX, nextY, endX, endY, stop1X, stop1Y, stop2X, stop2Y);

      // Check if the robot should stop at the second predetermined point
      if (x == stop2X && y == stop2Y) {
        // Stop at the second predetermined point
        steps[stepCount++] = 4;  // Custom action (e.g., stop for a specific task)
        stopMotors();
        return;
      }

      // Backtrack if the destination is not reached
      if (steps[stepCount - 1] == 1) {
        steps[stepCount++] = 1;  // Move backward
      } else if (steps[stepCount - 1] == 2) {
        steps[stepCount++] = 3;  // Turn right (to correct left turn)
      } else if (steps[stepCount - 1] == 3) {
        steps[stepCount++] = 2;  // Turn left (to correct right turn)
      }
    }
  }
}

// Function to execute a specific step
void executeStep(int step) {
  switch (step) {
    case 1:
      // Move forward
      setLeftMotor(150);
      setRightMotor(150);
      delay(1000);  // Adjust the delay based on your robot's speed
      break;
    case 2:
      // Turn left
      setLeftMotor(0);
      setRightMotor(150);
      delay(500);  // Adjust the delay based on your robot's turning speed
      break;
    case 3:
      // Turn right
      setLeftMotor(150);
      setRightMotor(0);
      delay(500);  // Adjust the delay based on your robot's turning speed
      break;
    case 4:
      // Custom action (e.g., stop for a specific task)
      stopMotors();
      delay(2000);  // Adjust the delay based on your task's duration
      break;
    // Add more cases for other movements or actions
  }
  // Stop motors after each step
  stopMotors();
}
