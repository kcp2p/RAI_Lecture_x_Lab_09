/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Motor_SSL.ino                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krchuaip <krchuaip@student.42bangkok.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 13:26:41 by krchuaip          #+#    #+#             */
/*   Updated: 2023/09/26 15:07:25 by krchuaip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Arduino IDE AVR will automatically be appended on compilation.
// Concept:
//   1. SC to ENA ENB on IRA IRB IRC met
//   2. Manual Takeover
//
// Objective:
//   1. Complete Lecture exercises, why am I even doing this???? What's the
//      purpose of doing this without an actual structure.....

#define IR_DEBUG    0
#define LOGIC_DEBUG 1
static bool MANUAL = 0;

#define MOTOR_EN_A    10
#define MOTOR_EN_B    5

#define MOTOR_A1    9
#define MOTOR_A2    8
#define MOTOR_B1    7
#define MOTOR_B2    6

#define LEFT_IR     11
#define MIDDLE_IR   12
#define RIGHT_IR    13

#define MAX_SPEED    255 // DEFINED MAX == 255

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);
  pinMode(MOTOR_EN_A, OUTPUT);

  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
  pinMode(MOTOR_EN_B, OUTPUT);

  if (!MANUAL)
  {
    pinMode(LEFT_IR, INPUT);
    pinMode(MIDDLE_IR, INPUT);
    pinMode(RIGHT_IR, INPUT);
  }
}

void loop() {
  if (!MANUAL)
  {
    manualCheck();
    bool leftVal = digitalRead(LEFT_IR); // Bool 0 or 1 only, true false accepted as boolean
    bool middleVal = digitalRead(MIDDLE_IR);
    bool rightVal = digitalRead(RIGHT_IR);

    if (IR_DEBUG)
    {
      Serial.print("Current: left: ");
      Serial.print(leftVal);
      Serial.print(" middle: ");
      Serial.print(middleVal);
      Serial.print(" right: ");
      Serial.println(rightVal);
    }

    if (leftVal == 1 && middleVal == 0 && rightVal == 1)
      moveForward();
    else if (leftVal == 0 && middleVal == 0 && rightVal == 0)
      stopMoving();
    else if (leftVal == 1 && middleVal == 1 && rightVal == 1)
      stopMoving();
    else if (leftVal == 0 && middleVal == 0 && rightVal == 1)
      turnLeft();
    else if (leftVal == 1 && middleVal == 0 && rightVal == 0)
      turnRight();
    else if (leftVal == 0 && middleVal == 1 && rightVal == 1)
      turnLeft();
    else if (leftVal == 1 && middleVal == 1 && rightVal == 0)
      turnRight();
  }
  else
  {
    // Manual Control
    if (Serial.available() > 0)
    {
      Serial.println("Acquiring control....");
      char readChar = Serial.read();
      
      if (readChar == '0')
        moveForward();
      else if (readChar == '1')
        moveBackward();
      else if (readChar == '2')
        turnLeft();
      else if (readChar == '3')
        turnRight();
      else if (readChar == '4')
        stopMoving();
      else if (readChar == 'd')
      {
        MANUAL = 0;
        Serial.println("Looping in normal mode");
        return ;
      }
    }
  }
}

void  manualCheck(void)
{
  if (!MANUAL && Serial.available() > 0)
  {
    Serial.println("Acquiring manual control....");
    if (Serial.read() == 'd')
    {
      MANUAL = 1;
      Serial.println("Looping in manual mode");
    }
  }
}

void moveForward(void)
{
  if (LOGIC_DEBUG)
    Serial.println("Moving forward");
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
  analogWrite(MOTOR_EN_A, MAX_SPEED);
  analogWrite(MOTOR_EN_B, MAX_SPEED);
}

void  moveBackward(void)
{
  if (LOGIC_DEBUG)
    Serial.println("Moving backward");
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
  analogWrite(MOTOR_EN_A, MAX_SPEED);
  analogWrite(MOTOR_EN_B, MAX_SPEED);
}

void turnLeft(void)
{
  if (LOGIC_DEBUG)
    Serial.println("Turning left");
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
  analogWrite(MOTOR_EN_A, MAX_SPEED / 2);
  analogWrite(MOTOR_EN_B, 0);
  }
  
void turnRight(void)
{
  if (LOGIC_DEBUG)
    Serial.println("Turning right");
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
  analogWrite(MOTOR_EN_A, 0);
  analogWrite(MOTOR_EN_B, MAX_SPEED / 2);
}

void stopMoving(void)
{
  Serial.println("Stop moving");
  analogWrite(MOTOR_EN_A, 0);
  analogWrite(MOTOR_EN_B, 0);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);

}
