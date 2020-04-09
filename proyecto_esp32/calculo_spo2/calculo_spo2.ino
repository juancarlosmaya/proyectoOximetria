
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int rojo[] = {85,86,87,88,89,90,91,92,91,90,89,89,90,89,88,87,86,85,84,83};
  int infrarojo[] = {35,36,37,38,38,38,39,42,40,39,39,39,38,38,39,38,37,36,35,35};
  
  for (int i=0; i<19;i++){
    
      Serial.print(rojo[i]);
      Serial.print(",");
      Serial.println(infrarojo[i]);
      delay(100);
  }

}
