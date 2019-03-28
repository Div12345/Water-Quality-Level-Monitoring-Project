void setup() {
  // put your setup code here, to run once:
  digitalWrite(2,INPUT);
  digitalWrite(3,INPUT);
  digitalWrite(4,INPUT);
  digitalWrite(8,OUTPUT);
  digitalWrite(9,OUTPUT);
  digitalWrite(10,OUTPUT);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(2)==HIGH)
  {digitalWrite(8,HIGH);}
  else if(digitalRead(2)==LOW)
  {digitalWrite(8,LOW);}
  if(digitalRead(3)==HIGH)
  {digitalWrite(9,HIGH);}
  else if(digitalRead(3)==LOW)
  {digitalWrite(9,LOW);}
  if(digitalRead(4)==HIGH)
  {digitalWrite(10,HIGH);}
  else if(digitalRead(4)==LOW)
  {digitalWrite(10,LOW);}
}
