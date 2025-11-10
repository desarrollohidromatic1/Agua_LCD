/*

Dispensador de Agua con Triac
ESP32

1.3
Se agrega boton de reset

1.4
Se cambio el void billInterrupt() - debido a que en ocasiones mandaba uno o dos pulsos de más

1.5
Se cambia la interrupcion del billetero y monedero a los otros pines con el opto y se agrega en hardware divisor de voltaje en sensores a 5V

1.6
Se cambia para que el control de ventas no se reinicie cuando llega a 256 compras

1.7
Se arregla el bug que permitia despachar presionando el boton de cambio
*/

#define firmware "1.7"

#include <EEPROM.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "variables.h"
#include "set.h"
#include "master.h"
#include "menu.h"



void setup() {
 // Serial.begin(9600);
  EEPROM.begin(512);
  setupGeneral();
  if(digitalRead(btReset) == LOW){
    delay(100);
    if(digitalRead(btReset) == LOW){
      moodRestaurar = true;
      digitalWrite(LedReset, HIGH);
      lcd.clear();
      lcd.home();
      lcd.print("Deseas restaurar?");
      lcd.setCursor(0,1);
      lcd.print("  Si");
      lcd.setCursor(0,2);
      lcd.print("  No");
      while(moodRestaurar == true){
        // Movernos en el menu
        if(digitalRead(bt1) == LOW) {
          delay(100);
          if(digitalRead(bt1) == LOW) {
            flecha += 1;
            if(flecha == 3){
              flecha = 1;          
            }        
          }
        }

        if( flecha == 1 ) {
          lcd.setCursor(0,1);
          lcd.print("->");  
          if(digitalRead(bt2) == LOW){
            delay(100);
            if(digitalRead(bt2) == LOW){
              firstTime ();
              moodRestaurar = false;
              digitalWrite(LedReset, LOW);
              delay(50);
              digitalWrite(LedReset, HIGH);
              delay(50);
              digitalWrite(LedReset, LOW);
              delay(50);
              digitalWrite(LedReset, HIGH);
              delay(50);              
            }
          }    
        }

        if( flecha == 2 ) {
          lcd.setCursor(0,1);
          lcd.print("  ");
          lcd.setCursor(0,2);
          lcd.print("->");
          if(digitalRead(bt2) == LOW){
            delay(100);
            if(digitalRead(bt2) == LOW){
              moodRestaurar = false;
            }
          } 
        }
      }      
      digitalWrite(LedReset, LOW);
      lcd.clear();
      flecha = 1;
    }
  }
  readEEPROM ();  
  if(hopper2 == 1){
    Cambio5 = true;
  }
  else {
    Cambio5 = false;
  }
  inicio = millis();
  /*
  // Pruebas con Serial
  Serial.begin(9600);
  pinMode (13, OUTPUT);
  digitalWrite(13, HIGH); 
  */
}

void loop() {
  readMenu();  
  /*
  // Pruebas con Serial
  digitalWrite(13, LOW); 
  delay(100);
  digitalWrite(13, HIGH);
  delay(100); 
  if ( Serial.available() > 0){
    int numero = Serial.parseInt();
    //variable3 = 0;
    //Serial.print("Variable 1: "); Serial.println(variable1);
    if (numero == 1){
      variable1 += 100;
      variable2 = ((variable1 * 100) / variable3);
      Serial.print("numero: "); Serial.println(numero);
      Serial.print("Variable 3: "); Serial.println(variable3);
      Serial.print("Variable 2: "); Serial.println(variable2);
    }
  }
  */
  //variable2 = ((variable1 * 100) / variable3);
  //Serial.print("Variable: "); Serial.println(variable2);
  
  if ( CreditAcum == 0) {
    cero ();
    contadorParo=1;
  }

  // Monedero  
  if(pulseCoin > 0) {
    delay(90);
    if(pulseCoin > 2){
      pulseCoin = 0;
      antiJammer();
    }
    else{
      pulseCoinAcum = pulseCoin;
      pulseCoin = 0;
      CreditAcum += pulseCoinAcum;    
      creditTime = millis();
      resetCredit = millis();
    }    
  }  

  // Billetero
  unsigned long lastTimeBill = millis() - millisUltBill;
  if(pulseBill > 0 && lastTimeBill > 300){      
    if(pulseBill > 2){
      pulseBill = 0;
    }
    pulseBillAcum = pulseBill * 10;     
    CreditAcum += pulseBillAcum;
    creditTime = millis();
    resetCredit = millis();        
    pulseBill = 0;
    millisUltBill = millis();
  }
  else{
    pulseBill = 0;
  }
  
  /*
  // Billetero
  unsigned long lastTimeBill = millis() - millisUltBill;
  if (pulseBill > 0 && lastTimeBill > 100){           
    /*if(pulseBill == 1){
      pulseBill = 0;
    }
    /*
    delay(100);
    if(pulseBill > 2){
      pulseBill = 0;
      antiJammer();
    }
    //else{
      pulseBillAcum = pulseBill * 10;
      pulseBill = 0;
      CreditAcum += pulseBillAcum;
      creditTime = millis();
      resetCredit = millis();      
   // }    
  }
  else{
    pulseBill = 0;
  }*/
  if( CreditAcum > 0 ) {
    saldo ();
    unsigned long debouncePulse = millis() - creditTime;
    if ( debouncePulse > timeWait ) {

      // Cambio
      if ( digitalRead(bt5) == LOW){
        delay(100);
        if ( digitalRead(bt5) == LOW) {
          if (CreditAcum > 4){
            if (Cambio5 == true){
              entregarCambio5();
            }
          }
          if ((CreditAcum > 0 && recoja == false) && Cambio1 == true) {
            entregarCambio1();
          }
          recoja = false;
          if (Cambio1 == false) {
            noCambio();
          }
          lcd.clear();
          clearCoin = true;
          pulseCoin = 0;
          pulseBill = 0;
          resetCredit = millis();
        }
      }

      // Enjuague
      if( digitalRead(bt1) == LOW) {
        delay(100);
        if (digitalRead(bt1) == LOW) {
          
          if(CreditAcum < priceEnjuague) {
            precio = priceEnjuague; 
            precioMostrar();
          }
          if (CreditAcum >= priceEnjuague && EnjuagueFirst == true) {
            tiempoEnjuague = EEPROM.readInt(100); 
            enjuague();
            pulseCoin = 0;
            pulseBill = 0;
            resetCredit = millis();
          }
        }
      }

      // Producto 1
      if (digitalRead(bt2) == LOW) {
        delay(100);
        if (digitalRead(bt2) == LOW) {
          contadorParo=1;
          precio = price1;
          if(CreditAcum >= price1) {
            producto1 = EEPROM.readInt(4);    
            product = producto1;
            producto();
            
            ventasProducto1 += 1;  
            EEPROM.writeInt(36,ventasProducto1);            
            EEPROM.commit(); 
          }
          else {            
            precioMostrar();
          }                
        }
      }

      // Producto 2
      if (digitalRead(bt3) == LOW) {
        delay(100);
        if (digitalRead(bt3) == LOW) {
          contadorParo=1;
          precio = price2;
          if(CreditAcum >= price2) {
            producto2 = EEPROM.readInt(8);    
            product = producto2;
            producto();
            
            ventasProducto2 += 1;  
            EEPROM.writeInt(40,ventasProducto2);
            EEPROM.commit(); 
          }
          else {            
            precioMostrar();
          }                
        }
      }

      // Producto 3
      if (digitalRead(bt4) == LOW) {
        delay(100);
        if (digitalRead(bt4) == LOW) {
          contadorParo=1;
          precio = price3;
          if(CreditAcum >= price3) {
            producto3 = EEPROM.readInt(12);    
            product = producto3;
            producto();
            
            ventasProducto3 += 1;  
            EEPROM.writeInt(44,ventasProducto3);
            EEPROM.commit(); 
          }
          else {            
            precioMostrar();
          }                
        }
      }
    }
  }
}