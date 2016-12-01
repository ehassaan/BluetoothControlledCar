#define HANDLING 0.5

int motorPins[2][2] = {{5, 6}, {9, 10}};
int xAxis, yAxis, mag, left, right;
char c;

void setup() {
  Init();
}

void loop() {

  String line = ReadLine();
  UpdateAxis(&xAxis, &yAxis, line);
  mag = sqrt(xAxis * xAxis + yAxis * yAxis);
  mag = constrain(mag, 0, 127);

  ToDifferentialDrive(xAxis, yAxis);

  DriveMotor(0, left, 128);
  DriveMotor(1, right, 128);

  Serial.print(left);
  Serial.print('\t');
  Serial.println(right);

}

void Init()
{
  Serial.begin(9600);
  Serial.println("ready");
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      pinMode(motorPins[i][j], OUTPUT);
      digitalWrite(motorPins[i][j], LOW);
    }
  }

}

void DriveMotor(int motor, int spd, int maxSpeed)
{
  spd = 255 * ((double)spd / maxSpeed);

  Serial.print(spd);
  Serial.print(" === \t");
  if (spd > 0)
  {
    analogWrite(motorPins[motor][0], spd);
    digitalWrite(motorPins[motor][1], LOW);
  }
  else
  {
    analogWrite(motorPins[motor][0], (255 + spd));
    digitalWrite(motorPins[motor][1], HIGH);
  }

}

void StopMotor(int motor)
{
  digitalWrite(motorPins[motor][0], LOW);
  digitalWrite(motorPins[motor][1], LOW);
}

String ReadLine()
{
  String s;
  char c;

  while (1)
  {
    if (Serial.available())
    {
      c = Serial.read();
      if (c == '*')
      {
        continue;
      }
      else if (c == '#')
      {
        return s;
      }
      else
      {
        s += c;
      }
    }
  }

}

void UpdateAxis(int *x, int *y, String s)
{
  String s1, s2;
  bool comma;

  if (s.length() != 3) return;

  *x = (int)(byte)s[0] - 127;
  *y = (int)(byte)s[2] - 127;
}


void ToDifferentialDrive(int x, int y)
{
  if (y > 0)
  {
    if (x < 0)
    {
      left = mag + (HANDLING * x);
      right = mag;
    }
    else
    {
      left = mag;
      right = mag - (HANDLING * x);
    }
  }
  else
  {
    if (x < 0)
    {
      left = -mag;
      right = -mag - x;
    }
    else
    {
      left = -mag + x;
      right = -mag;
    }
  }
}

