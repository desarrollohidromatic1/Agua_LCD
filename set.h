void coinInterrupt() {    
  unsigned long lastTimeCoin = millis() - millisUltCoin;
  if(lastTimeCoin > 50){
    pulseCoin ++;    
  }
  
   millisUltCoin = millis(); 
}
/*
void billInterrupt() { 
  //pulseBill ++;  
  pulseBill ++;           
 
}*/

void billInterrupt() {
  static unsigned long lastInterruptTime = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - lastInterruptTime > DEBOUNCE_TIME) {
    pulseBill++;
  }
  lastInterruptTime = currentTime;
}

void flujo () {
  pulseSensor = 1;
}
void setupGeneral () {

  lcd.begin (20,4);
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);  
  lcd.createChar(1,f1);
  lcd.createChar(2,f2);
  lcd.createChar(3,f3);
  lcd.createChar(4,f4);
  lcd.createChar(5,f5);
  lcd.createChar(6,f6);
  lcd.createChar(7,f7);
  lcd.createChar(8,f8); 

  pinMode(bt1, INPUT);
  pinMode(bt2, INPUT);
  pinMode(bt3, INPUT);
  pinMode(bt4, INPUT);
  pinMode(bt5, INPUT);
  pinMode(bt6, INPUT);
  pinMode(btReset, INPUT);
  
  pinMode(ph1, INPUT); 
  pinMode(ph2, INPUT); 

  pinMode(LedProg, OUTPUT);
  pinMode(LedReset, OUTPUT);
  digitalWrite(LedProg, LOW);

  pinMode(ReleOzono, OUTPUT);
  digitalWrite(ReleOzono, LOW);
  pinMode(ReleHop1, OUTPUT);
  digitalWrite(ReleHop1, LOW);
  pinMode(ReleHop2, OUTPUT);
  digitalWrite(ReleHop2, LOW);
  pinMode(ReleEnjuague, OUTPUT);
  digitalWrite(ReleEnjuague, LOW);
  pinMode(ReleProducto, OUTPUT);      
  digitalWrite(ReleProducto, LOW);

  pinMode(interruptCoin, INPUT); 
  attachInterrupt(digitalPinToInterrupt(interruptCoin), coinInterrupt, FALLING);
  pinMode(interruptBill, INPUT); 
  attachInterrupt(digitalPinToInterrupt(interruptBill), billInterrupt, FALLING);
  pinMode(sensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensor), flujo, FALLING);
} 

void firstTime () {
  tiempoEnjuague = 4000;
  EEPROM.writeInt(100, tiempoEnjuague);
  EEPROM.commit();  
  producto1 = 1;
  EEPROM.writeInt(4, producto1);
  EEPROM.commit();  
  producto2 = 1;
  EEPROM.writeInt(8, producto2);
  EEPROM.commit();
  producto3 = 1;
  EEPROM.writeInt(12, producto3);
  EEPROM.commit();  
  priceEnjuague = 0;
  EEPROM.writeInt(16, priceEnjuague);
  EEPROM.commit();  
  price1 = 7;
  EEPROM.writeInt(20, price1);  
  EEPROM.commit();    
  price2 = 10;
  EEPROM.writeInt(24, price2);   
  EEPROM.commit();   
  price3 = 14;
  EEPROM.writeInt(28, price3);   
  EEPROM.commit(); 
  ventasProducto1 = 0;
  ventasProducto2 = 0;
  ventasProducto3 = 0;
  EEPROM.writeInt(36,ventasProducto1);
  EEPROM.commit(); 
  EEPROM.writeInt(40,ventasProducto2);
  EEPROM.commit(); 
  EEPROM.writeInt(44,ventasProducto3);  
  EEPROM.commit(); 
  hopper2 = 0;
  EEPROM.writeInt(48, hopper2);
  EEPROM.commit(); 
  feriar = 1;
  EEPROM.writeInt(250, feriar);
  EEPROM.commit();
}

void readEEPROM () {
  tiempoEnjuague = EEPROM.readInt(100);  
  producto1 = EEPROM.readInt(4);    
  producto2 = EEPROM.readInt(8);    
  producto3 = EEPROM.readInt(12);    
  priceEnjuague = EEPROM.readInt(16);      
  price1 = EEPROM.readInt(20);    
  price2 = EEPROM.readInt(24);    
  price3 = EEPROM.readInt(28);            
  ventasProducto1 = EEPROM.readInt(36);
  ventasProducto2 = EEPROM.readInt(40);
  ventasProducto3 = EEPROM.readInt(44);          
  hopper2 = EEPROM.readInt(48);
  feriar  = EEPROM.readInt(250);  
  if ( hopper2 == 1) {
    Cambio5 = true;
  }
}
