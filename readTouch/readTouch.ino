const int ain=A0; 
const int LED=4;
int inputVal=0;
void setup()
{
Serial.begin(9600);
pinMode(LED, OUTPUT);
digitalWrite(LED, LOW);

}
void loop()
{
  inputVal=analogRead(ain);
  if(inputVal>20)
  {
  Serial.println("TOCAAAAA");
  digitalWrite(LED, HIGH);
  delay(5000);
  digitalWrite(LED,LOW);
  
  }
  
  //for serial monitor
  delay(15);
  Serial.println(inputVal);
}
