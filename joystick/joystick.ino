#define XOUT 2
#define YOUT 3
#define ZOUT 4
#define XIN A0
#define YIN A1
#define ZIN A2

#define XXOUT 2
#define YYOUT 3
#define ZZOUT 4
#define XXIN A3
#define YYIN A4
#define ZZIN A5
void setup() {
  pinMode(XIN, INPUT);
  pinMode(YIN, INPUT);
  pinMode(ZIN, INPUT);
  pinMode(XOUT, OUTPUT);
  pinMode(YOUT, OUTPUT);
  pinMode(ZOUT, OUTPUT);
  pinMode(XXIN, INPUT);
  pinMode(YYIN, INPUT);
  pinMode(ZZIN, INPUT);
  pinMode(XXOUT, OUTPUT);
  pinMode(YYOUT, OUTPUT);
  pinMode(ZZOUT, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int x, y, z, xx, yy, zz;

  x = analogRead(XIN);
  y = analogRead(YIN);
  z = analogRead(ZIN);
  xx = analogRead(XXIN);
  yy = analogRead(YYIN);
  zz = analogRead(ZZIN);

  if (x > 1000 && y < 1000 && y > 10) {
    Serial.print("up\n");
    digitalWrite(XOUT, LOW);
    digitalWrite(YOUT, LOW);
    return;
  }

  if (x < 10 && y < 1000 && y > 10) {
    Serial.print("down\n");
    digitalWrite(XOUT, LOW);
    digitalWrite(YOUT, HIGH);
    return;
  }

  if (y < 10 && x < 1000 && x > 10) {
    Serial.print("left\n");
    digitalWrite(XOUT, HIGH);
    digitalWrite(YOUT, LOW);
    return;
  }

  if (y > 1000 && x < 1000 && x > 10) {
    Serial.print("right\n");
    digitalWrite(XOUT, HIGH);
    digitalWrite(YOUT, HIGH);
    return;
  }

  //joystick 2
  if (xx > 1000 && yy < 1000 && yy > 10) {
    Serial.print("uup\n");
    digitalWrite(XXOUT, LOW);
    digitalWrite(YYOUT, LOW);
    return;
  }

  if (xx < 10 && yy < 1000 && yy > 10) {
    Serial.print("ddown\n");
    digitalWrite(XXOUT, LOW);
    digitalWrite(YYOUT, HIGH);
    return;
  }

  if (yy < 10 && xx < 1000 && xx > 10) {
    Serial.print("lleft\n");
    digitalWrite(XXOUT, HIGH);
    digitalWrite(YYOUT, LOW);
    return;
  }

  if (yy > 1000 && xx < 1000 && xx > 10) {
    Serial.print("rright\n");
    digitalWrite(XXOUT, HIGH);
    digitalWrite(YYOUT, HIGH);
    return;
  }

  //Serial.print("X=");
  //Serial.print(x);
  //Serial.print("\tY=");
  //Serial.print(y);
  //Serial.print("\tZ=");
  //Serial.println(z);

  delay(10);
}

