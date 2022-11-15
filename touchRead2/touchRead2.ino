const int touchPin = 4;
int touchValue;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:

  touchValue = touchRead(touchPin);
  Serial.print(touchValue);

  if(touchValue >20){
    Serial.println("TOCAAAA");
    }

  delay(500);

}
