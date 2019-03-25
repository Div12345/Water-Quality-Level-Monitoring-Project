void setup() {
  // put your setup code here, to run once:
  digitalWrite(2,INPUT);
  digitalWrite(3,INPUT);
  digitalWrite(4,INPUT);
  digitalWrite(8,OUTPUT);
  digitalWrite(9,OUTPUT);
  digitalWrite(10,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(2)==1)
  {digitalWrite(8,HIGH);}
  if(digitalRead(2)==0)
  {digitalWrite(8,LOW);}
  if(digitalRead(3)==1)
  {digitalWrite(9,HIGH);}
  if(digitalRead(3)==0)
  {digitalWrite(9,LOW);}
  if(digitalRead(4)==1)
  {digitalWrite(10,HIGH);}
  if(digitalRead(4)==0)
  {digitalWrite(10,LOW);}
}
