
void sales () {

  ventasProducto1= EEPROM.readInt(36);
  ventasProducto2= EEPROM.readInt(40);
  ventasProducto3= EEPROM.readInt(44);
  ventasProducto4= EEPROM.readInt(addr_ventasProducto4);

  while ( MoodSalesControl == true ) {
    //lcd.setCursor(0, 0);
    //lcd.print("Control de ventas:");
    //Producto 1
    lcd.setCursor(0, 0);
    lcd.print("Producto 1:");
    lcd.setCursor(12, 0);
    lcd.print(ventasProducto1);
    //Producto 2
    lcd.setCursor(0, 1);
    lcd.print("Producto 2:");
    lcd.setCursor(12, 1);
    lcd.print(ventasProducto2);
    //Producto 3
    lcd.setCursor(0, 2);
    lcd.print("Producto 3:");
    lcd.setCursor(12, 2);
    lcd.print(ventasProducto3);
    //Producto 4
    lcd.setCursor(0, 3);
    lcd.print("Producto 4:");
    lcd.setCursor(12, 3);
    lcd.print(ventasProducto4);

    // Resetear con el boton de producto 3
    if(digitalRead(bt4) == LOW){
      delay(100);
      if(digitalRead(bt4) == LOW){
        ventasProducto1 = 0;
        ventasProducto2 = 0;
        ventasProducto3 = 0;
        ventasProducto4 = 0;
        EEPROM.writeInt(36,ventasProducto1);
        EEPROM.commit();
        EEPROM.writeInt(40,ventasProducto2);
        EEPROM.commit();
        EEPROM.writeInt(44,ventasProducto3);        
        EEPROM.commit();
        EEPROM.writeInt(addr_ventasProducto4,ventasProducto4);        
        EEPROM.commit();
        lcd.clear();
      }
    }

    // Salir
    if(digitalRead(bt5) == LOW || digitalRead(bt6) == LOW) {
      delay(100);
      if(digitalRead(bt5) == LOW || digitalRead(bt6) == LOW) {
        MoodSalesControl = false;
        flecha = 1;        
        lcd.clear();        
      }
    }
  }
}

void calibration () {
  
  digitalWrite(ReleOzono, HIGH);
  while( MoodCalibration == true ) {
    
    // Enjuague
    if(digitalRead(bt1) == LOW) {
      delay(100);
      if(digitalRead(bt1) == LOW) { 
         
        tiempoEnjuague = EEPROM.readFloat(100);    
        tiempoMuestra = tiempoEnjuague / 1000;          
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Modificacion de");
        lcd.setCursor(6,1);
        lcd.print("enjuague");
        lcd.setCursor(0,2);
        lcd.print("Tiempo 1:");
        lcd.setCursor(10,2);
        lcd.print(tiempoMuestra);
        lcd.setCursor(13,2);
        lcd.print("seg");
        lcd.setCursor(0,3);
        lcd.print("Tiempo 2:");
        lcd.setCursor(13,3);
        lcd.print("seg");
        moodEnjuague = true;
        while(moodEnjuague == true) {
          tiempoMuestra = tiempoEnjuague / 1000;   
          lcd.setCursor(10,3);
          lcd.print(tiempoMuestra);
          
          
          // Aumenta con boton cambio
          if(digitalRead(bt5) == LOW){
            delay(100);
            if(digitalRead(bt5) == LOW){
              tiempoEnjuague += 1000;
            }
          }

          // Decrementa con boton producto 3
          if(digitalRead(bt4) == LOW){
            delay(100);
            if(digitalRead(bt4) == LOW){ 
              if (tiempoEnjuague  != 0){
                tiempoEnjuague -= 1000;                                
              }             
              if (tiempoEnjuague < 10000){
                lcd.setCursor(11,3);
                lcd.print(" ");
              }              
            }
          }

          // Confirmar con el boton de enjuague
          if(digitalRead(bt1) == LOW){
            delay(100);
            if(digitalRead(bt1) == LOW){              
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Tiempo de enjuague");
              lcd.setCursor(6,1);
              lcd.print("guardado");
              lcd.setCursor(8,2);
              lcd.print(tiempoMuestra);
              lcd.setCursor(10,2);
              lcd.print("seg");
              moodEnjuague = false;      
              EEPROM.writeInt(100, tiempoEnjuague);
              EEPROM.commit();

              delay(time_exit);
              MoodCalibration = false;
              flecha = 1;        
              lcd.clear();
              digitalWrite(ReleOzono, LOW);       
            }
          }
        }
        
      }
    }

    // Producto 1
    if(digitalRead(bt2) == LOW) {
      delay(100);      
      if(digitalRead(bt2) == LOW) {   
        producto1 = EEPROM.readInt(4);             
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Modificacion de");
        lcd.setCursor(5,1);
        lcd.print("producto 1");  
        lcd.setCursor(0,2);
        lcd.print("Pulsos 1:");      
        lcd.setCursor(10,2);
        lcd.print(producto1);      
        lcd.setCursor(0,3);
        lcd.print("Pulsos 2:");   
        moodProducto1 = true;           
        delay(500);
        while(moodProducto1 == true){
          if ( digitalRead(bt2) == LOW ){     
            delay(100);
            if ( digitalRead(bt2) == LOW ){                           
            pulseSensor = 0;
            pulseAcumSensor = 0;
            digitalWrite(ReleProducto, HIGH);
            while ( digitalRead(bt2) == LOW ){ 
              if(digitalRead(bt1) == LOW){
                delay(100);
                if(digitalRead(bt1) == LOW){ 
                  paro();
                }
              }       
              /*                    
              int state = digitalRead(sensor);
              if(state == HIGH && lastState == LOW){
                pulseSensor ++;
              }
              lastState = state;
              */
              if (pulseSensor > 0){
                pulseAcumSensor += pulseSensor;
                pulseSensor = 0;
                qlitros = ((pulseAcumSensor * 100)/ producto1);
                lcd.setCursor(10,3);
                lcd.print(pulseAcumSensor);  
              }

              if(tiempoExcedido == true){
                pulseAcumSensor = producto1;
                tiempoExcedido = false;
              }   
              producto1 = pulseAcumSensor;             
            }
            }
            digitalWrite(ReleProducto, LOW);          
            
            if (producto1 < 1){
              producto1 = 1;
            }                    
          }                    
          // GUARDAR Y SALIR
          if(digitalRead(bt1) || digitalRead(bt6) == LOW ) {
            delay(100);
            if(digitalRead(bt1) == LOW || digitalRead(bt6) == LOW) {
              EEPROM.writeInt(4, producto1);
              EEPROM.commit();
              moodProducto1 = false;
              delay(100);
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Producto 1");
              lcd.setCursor(6,1);
              lcd.print("guardado");
              lcd.setCursor(8,2);
              lcd.print(producto1);

              delay(time_exit);
              MoodCalibration = false;
              flecha = 1;        
              lcd.clear();
              digitalWrite(ReleOzono, LOW); 
            }
          }
        }        
      }
    }

    // Producto 2
    if(digitalRead(bt3) == LOW) {
      delay(100);      
      if(digitalRead(bt3) == LOW) {   
        producto2 = EEPROM.readInt(8);             
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Modificacion de");
        lcd.setCursor(5,1);
        lcd.print("producto 2");  
        lcd.setCursor(0,2);
        lcd.print("Pulsos 1:");      
        lcd.setCursor(10,2);
        lcd.print(producto2);      
        lcd.setCursor(0,3);
        lcd.print("Pulsos 2:");   
        moodProducto2 = true;           
        delay(500);
        while(moodProducto2 == true){
          if ( digitalRead(bt3) == LOW ){                           
            delay(100);
            if ( digitalRead(bt3) == LOW ) {                          
            pulseSensor = 0;
            pulseAcumSensor = 0;
            digitalWrite(ReleProducto, HIGH);
            while ( digitalRead(bt3) == LOW ){ 
              if(digitalRead(bt1) == LOW){
                delay(100);
                if(digitalRead(bt1) == LOW){ 
                  paro();
                }
              }            
              /*               
              int state = digitalRead(sensor);
              if(state == HIGH && lastState == LOW){
                pulseSensor ++;
              }
              lastState = state;
              */
              if (pulseSensor > 0){
                pulseAcumSensor += pulseSensor;
                pulseSensor = 0;
                qlitros = ((pulseAcumSensor * 100)/ producto2);
                lcd.setCursor(10,3);
                lcd.print(pulseAcumSensor);  
               // Serial.print("Pulsos:"); Serial.println(pulseAcumSensor);
              }

              if(tiempoExcedido == true){
                pulseAcumSensor = producto2;
                tiempoExcedido = false;
              }      
              producto2 = pulseAcumSensor;          
            }
          }
           // Serial.print("Pulsos producto 2:"); Serial.println(producto2);
            digitalWrite(ReleProducto, LOW);          
             
            /* 
            if (producto2 < 1){
              producto2 = 1;
            } 
            */                   
          }                    
          // GUARDAR Y SALIR
          if(digitalRead(bt1) == LOW || digitalRead(bt6) == LOW) {
            delay(100);
            if(digitalRead(bt1) == LOW || digitalRead(bt6) == LOW) {
              EEPROM.writeInt(8, producto2);
              EEPROM.commit();
              moodProducto2 = false;
              delay(100);
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Producto 2");
              lcd.setCursor(6,1);
              lcd.print("guardado");
              lcd.setCursor(8,2);
              lcd.print(producto2);
              
              delay(time_exit);
              MoodCalibration = false;
              flecha = 1;        
              lcd.clear();
              digitalWrite(ReleOzono, LOW); 
            }
          }
        }        
      }
    }

    // Producto 3
    if(digitalRead(bt4) == LOW) {
      delay(100);      
      if(digitalRead(bt4) == LOW) {   
        producto3 = EEPROM.readInt(12);             
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Modificacion de");
        lcd.setCursor(5,1);
        lcd.print("producto 3");  
        lcd.setCursor(0,2);
        lcd.print("Pulsos 1:");      
        lcd.setCursor(10,2);
        lcd.print(producto3);      
        lcd.setCursor(0,3);
        lcd.print("Pulsos 2:");   
        moodProducto3 = true;           
        delay(500);
        while(moodProducto3 == true){
          if ( digitalRead(bt4) == LOW ){ 
            delay(100);
            if ( digitalRead(bt4) == LOW ) {                         
            pulseSensor = 0;
            pulseAcumSensor = 0;
            digitalWrite(ReleProducto, HIGH);
            while ( digitalRead(bt4) == LOW ){ 
              if(digitalRead(bt1) == LOW){
                delay(100);
                if(digitalRead(bt1) == LOW){ 
                  paro();
                }
              }                
              /*           
              int state = digitalRead(sensor);
              if(state == HIGH && lastState == LOW){
                pulseSensor ++;
              }
              lastState = state;
              */
              if (pulseSensor > 0){
                pulseAcumSensor += pulseSensor;
                pulseSensor = 0;
                qlitros = ((pulseAcumSensor * 100)/ producto3);
                lcd.setCursor(10,3);
                lcd.print(pulseAcumSensor);  
              }

              if(tiempoExcedido == true){
                pulseAcumSensor = producto3;
                tiempoExcedido = false;
              } 
              producto3 = pulseAcumSensor;                 
            }
          }
            digitalWrite(ReleProducto, LOW);          
            producto3 = pulseAcumSensor;    
            if (producto3 < 1){
              producto3 = 1;
            }                    
          }                    
          // GUARDAR Y SALIR
          if(digitalRead(bt1) == LOW || digitalRead(bt6) == LOW) {
            delay(100);
            if(digitalRead(bt1) == LOW || digitalRead(bt6) == LOW) {
              EEPROM.writeInt(12, producto3);
              EEPROM.commit();
              moodProducto3 = false;
              delay(100);
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Producto 3");
              lcd.setCursor(6,1);
              lcd.print("guardado");
              lcd.setCursor(8,2);
              lcd.print(producto3);

              delay(time_exit);
              MoodCalibration = false;
              flecha = 1;        
              lcd.clear();
              digitalWrite(ReleOzono, LOW); 
            }
          }
        }        
      }
    }

  
    // Producto 4
    if(digitalRead(bt5) == LOW) {
      delay(100);      
      if(digitalRead(bt5) == LOW) {   
        producto4 = EEPROM.readInt(addr_producto4);             
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Modificacion de");
        lcd.setCursor(5,1);
        lcd.print("producto 4");  
        lcd.setCursor(0,2);
        lcd.print("Pulsos 1:");      
        lcd.setCursor(10,2);
        lcd.print(producto4);      
        lcd.setCursor(0,3);
        lcd.print("Pulsos 2:");   
        moodProducto4 = true;           
        delay(500);
        while(moodProducto4 == true){
          if ( digitalRead(bt5) == LOW ){ 
            delay(100);
            if ( digitalRead(bt5) == LOW ) {                         
            pulseSensor = 0;
            pulseAcumSensor = 0;
            digitalWrite(ReleProducto, HIGH);
            while ( digitalRead(bt5) == LOW ){ 
              if(digitalRead(bt1) == LOW){
                delay(100);
                if(digitalRead(bt1) == LOW){ 
                  paro();
                }
              }                
              /*           
              int state = digitalRead(sensor);
              if(state == HIGH && lastState == LOW){
                pulseSensor ++;
              }
              lastState = state;
              */
              if (pulseSensor > 0){
                pulseAcumSensor += pulseSensor;
                pulseSensor = 0;
                qlitros = ((pulseAcumSensor * 100)/ producto4);
                lcd.setCursor(10,3);
                lcd.print(pulseAcumSensor);  
              }

              if(tiempoExcedido == true){
                pulseAcumSensor = producto4;
                tiempoExcedido = false;
              } 
              producto4 = pulseAcumSensor;                 
            }
          }
            digitalWrite(ReleProducto, LOW);          
            producto4 = pulseAcumSensor;    
            if (producto4 < 1){
              producto4 = 1;
            }                    
          }                    
          // GUARDAR Y SALIR
          if(digitalRead(bt1) == LOW || digitalRead(bt6) == LOW) {
            delay(100);
            if(digitalRead(bt1) == LOW || digitalRead(bt6) == LOW) {
              EEPROM.writeInt(addr_producto4, producto4);
              EEPROM.commit();
              moodProducto4 = false;
              delay(100);
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Producto 4");
              lcd.setCursor(6,1);
              lcd.print("guardado");
              lcd.setCursor(8,2);
              lcd.print(producto4);

              delay(time_exit);
              MoodCalibration = false;
              flecha = 1;        
              lcd.clear();
              digitalWrite(ReleOzono, LOW); 
            }
          }
        }        
      }
    }


  }
}

void price () {
  while ( MoodChangePrice == true ) {

    // Precio enjuague
    if(digitalRead(bt1) == LOW) {
      delay(100);      
      if(digitalRead(bt1) == LOW) {            
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Precio enjuague");
        lcd.setCursor(8,1);
        lcd.print("$");
        lcd.setCursor(0,2);
        lcd.print("Bt5:+1");
        lcd.setCursor(0,3);
        lcd.print("Bt4:-1");
        lcd.setCursor(14,3);
        lcd.print("Bt1:OK");
        changepriceEn = true;
        while (changepriceEn == true) {
          lcd.setCursor(9, 1);
          lcd.print(priceEnjuague); 

          if(digitalRead(bt5) == LOW){
            delay(100);
            if(digitalRead(bt5) == LOW){
              priceEnjuague += 1;
            }
          }

          if(digitalRead(bt4) == LOW){
            delay(100);
            if(digitalRead(bt4) == LOW){              
              if(priceEnjuague == 0){
                priceEnjuague = 0;
              }
              else {
                priceEnjuague -= 1;
              }
              if(priceEnjuague < 10){
                lcd.setCursor(10, 1);
                lcd.print(" "); 
              }
            }
          }

          if(digitalRead(bt1) == LOW){
            delay(100);
            if(digitalRead(bt1) == LOW){
              EEPROM.writeInt(16, priceEnjuague);    
              EEPROM.commit();
              lcd.clear();          
              lcd.setCursor(0,2);
              lcd.print("Precio Guardado");
              lcd.setCursor(8,3);
              lcd.print("$");
              lcd.setCursor(9,3);
              priceEnjuague = EEPROM.readInt(16);
              lcd.print(priceEnjuague);
              changepriceEn = false;
              delay(500);

              MoodChangePrice = false;
              flecha = 1;        
              lcd.clear();      
            }
          }
        }
      }
    }

    // Precio producto 1        
    if(digitalRead(bt2) == LOW) {
      delay(100);      
      if(digitalRead(bt2) == LOW) {                
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Precio producto 1");
        lcd.setCursor(8,1);
        lcd.print("$");
        lcd.setCursor(0,2);
        lcd.print("Bt5:+1");
        lcd.setCursor(0,3);
        lcd.print("Bt4:-1");
        lcd.setCursor(14,3);
        lcd.print("Bt1:OK");
        changeprice1 = true;
        while (changeprice1 == true) {
          lcd.setCursor(9, 1);
          lcd.print(price1); 

          if(digitalRead(bt5) == LOW){
            delay(100);
            if(digitalRead(bt5) == LOW){
              price1 += 1;
            }
          }

          if(digitalRead(bt4) == LOW){
            delay(100);
            if(digitalRead(bt4) == LOW){              
              if(price1 == 0){
                price1 = 0;
              }
              else{
                price1 -= 1;
              }
              if(price1 < 10){
                lcd.setCursor(10, 1);
                lcd.print(" "); 
              }
            }
          }

          if(digitalRead(bt1) == LOW){
            delay(100);
            if(digitalRead(bt1) == LOW){
              lcd.clear();
              EEPROM.writeInt(20, price1);              
              EEPROM.commit();
              lcd.setCursor(0,2);
              lcd.print("Precio Guardado");
              lcd.setCursor(8,3);
              lcd.print("$");
              lcd.setCursor(9,3);
              price1 = EEPROM.readInt(20);
              lcd.print(price1);
              changeprice1 = false;

              delay(time_exit);
              MoodChangePrice = false;
              flecha = 1;        
              lcd.clear(); 
            }
          }
        }
      }
    }

    // Precio producto 2
    if(digitalRead(bt3) == LOW) {
      delay(100);      
      if(digitalRead(bt3) == LOW) {                
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Precio producto 2");
        lcd.setCursor(8,1);
        lcd.print("$");
        lcd.setCursor(0,2);
        lcd.print("Bt5:+1");
        lcd.setCursor(0,3);
        lcd.print("Bt4:-1");
        lcd.setCursor(14,3);
        lcd.print("Bt1:OK");
        changeprice2 = true;
        while (changeprice2 == true) {
          lcd.setCursor(9, 1);
          lcd.print(price2); 

          if(digitalRead(bt5) == LOW){
            delay(100);
            if(digitalRead(bt5) == LOW){
              price2 += 1;
            }
          }

          if(digitalRead(bt4) == LOW){
            delay(100);
            if(digitalRead(bt4) == LOW){              
              if(price2 == 0){
                price2 = 0;
              }
              else{
                price2 -= 1;
              }
              if(price2 < 10){
                lcd.setCursor(10, 1);
                lcd.print(" "); 
              }
            }
          }

          if(digitalRead(bt1) == LOW){
            delay(100);
            if(digitalRead(bt1) == LOW){
              lcd.clear();
              EEPROM.writeInt(24, price2);              
              EEPROM.commit();
              lcd.setCursor(0,2);
              lcd.print("Precio Guardado");
              lcd.setCursor(8,3);
              lcd.print("$");
              lcd.setCursor(9,3);
              price2 = EEPROM.readInt(24);
              lcd.print(price2);
              changeprice2 = false;

              delay(time_exit);
              MoodChangePrice = false;
              flecha = 1;        
              lcd.clear(); 
            }
          }
        }
      }
    }

    // Precio producto 3
    if(digitalRead(bt4) == LOW) {
      delay(100);      
      if(digitalRead(bt4) == LOW) {           
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Precio producto 3");
        lcd.setCursor(8,1);
        lcd.print("$");
        lcd.setCursor(0,2);
        lcd.print("Bt3:+1");
        lcd.setCursor(0,3);
        lcd.print("Bt2:-1");
        lcd.setCursor(14,3);
        lcd.print("Bt1:OK");
        changeprice3 = true;
        while (changeprice3 == true) {
          lcd.setCursor(9, 1);
          lcd.print(price3); 

          if(digitalRead(bt3) == LOW){
            delay(100);
            if(digitalRead(bt3) == LOW){
              price3 += 1;
            }
          }

          if(digitalRead(bt2) == LOW){
            delay(100);
            if(digitalRead(bt2) == LOW){              
              if(price3 == 0){
                price3 = 0;
              }
              else{
                price3 -= 1;
              }
              if(price3 < 10){
                lcd.setCursor(10, 1);
                lcd.print(" "); 
              }
            }
          }

          if(digitalRead(bt1) == LOW){
            delay(100);
            if(digitalRead(bt1) == LOW){
              lcd.clear();
              EEPROM.writeInt(28, price3);              
              EEPROM.commit();
              lcd.setCursor(0,2);
              lcd.print("Precio Guardado");
              lcd.setCursor(8,3);
              lcd.print("$");
              lcd.setCursor(9,3);
              price3 = EEPROM.readInt(28);
              lcd.print(price3);
              changeprice3 = false;

              delay(time_exit);
              MoodChangePrice = false;
              flecha = 1;        
              lcd.clear(); 
            }
          }
        }
      }
    }

    // Precio producto 4
    if(digitalRead(bt5) == LOW) {
      delay(100);      
      if(digitalRead(bt5) == LOW) {           
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Precio producto 4");
        lcd.setCursor(8,1);
        lcd.print("$");
        lcd.setCursor(0,2);
        lcd.print("Bt3:+1");
        lcd.setCursor(0,3);
        lcd.print("Bt2:-1");
        lcd.setCursor(14,3);
        lcd.print("Bt1:OK");
        changeprice4 = true;
        while (changeprice4 == true) {
          lcd.setCursor(9, 1);
          lcd.print(price4); 

          if(digitalRead(bt3) == LOW){
            delay(100);
            if(digitalRead(bt3) == LOW){
              price4 += 1;
            }
          }

          if(digitalRead(bt2) == LOW){
            delay(100);
            if(digitalRead(bt2) == LOW){              
              if(price4 == 0){
                price4 = 0;
              }
              else{
                price4 -= 1;
              }
              if(price4 < 10){
                lcd.setCursor(10, 1);
                lcd.print(" "); 
              }
            }
          }

          if(digitalRead(bt1) == LOW){
            delay(100);
            if(digitalRead(bt1) == LOW){
              lcd.clear();
              EEPROM.writeInt(addr_price4, price4);              
              EEPROM.commit();
              lcd.setCursor(0,2);
              lcd.print("Precio Guardado");
              lcd.setCursor(8,3);
              lcd.print("$");
              lcd.setCursor(9,3);
              price4 = EEPROM.readInt(addr_price4);
              lcd.print(price4);
              changeprice4 = false;

              delay(time_exit);
              MoodChangePrice = false;
              flecha = 1;        
              lcd.clear(); 
            }
          }
        }
      }
    }

    // Salir de calibracion
    //if(digitalRead(bt5) == LOW || digitalRead(bt6) == LOW) {
    //  delay(100);
    //  if(digitalRead(bt5) == LOW || digitalRead(bt6) == LOW) {
    //    MoodChangePrice = false;
    //    flecha = 1;        
    //    lcd.clear();        
    //  }
    //}
  }
}

void availableHopper2 () {
  
  while (MoodHopper2 == true) {

    // Movernos en el menu
    if(digitalRead(bt1) == LOW) {
      delay(100);
      if(digitalRead(bt1) == LOW) {
        flecha += 1;
        if(flecha == 6){
          flecha = 4;          
        }        
      }
    }

    if( flecha == 4 ) {
      lcd.setCursor(0,1);
      lcd.print("->");      
    }

    if( flecha == 5 ) {
      lcd.setCursor(0,1);
      lcd.print("  ");
      lcd.setCursor(0,2);
      lcd.print("->");
    }

    if(digitalRead(bt2) == LOW && flecha == 4 ) {
      delay(100);
      if(digitalRead(bt2) == LOW) {
        lcd.clear();
        lcd.home();
        lcd.print("Hopper 2 Activado");
        delay(2000);
        hopper2 = 1;
        EEPROM.writeInt(48, hopper2);
        EEPROM.commit();
        MoodHopper2 = false;
        flecha = 1;        
        lcd.clear();    
        Cambio5 = true;    
      }
    } 

    if(digitalRead(bt2) == LOW && flecha == 5 ) {
      delay(100);
      if(digitalRead(bt2) == LOW) {
        lcd.clear();
        lcd.home();
        lcd.print("Hopper 2 Desactivado");
        delay(2000);
        hopper2 = 0;
        EEPROM.writeInt(48, hopper2);
        EEPROM.commit();
        Cambio5 = false;
        MoodHopper2 = false;
        flecha = 1;        
        lcd.clear();        
      }
    }

    // Salir de hopper
    if(digitalRead(bt5) == LOW || digitalRead(bt6) == LOW) {
      delay(100);
      if(digitalRead(bt5) == LOW || digitalRead(bt6) == LOW) {
        MoodHopper2 = false;
        flecha = 1;        
        lcd.clear();        
      }
    }
  }
}

void moodMenu () {

  while(ModoMenu == true){
    if(digitalRead(bt6) == LOW){
      delay(100);
      if(digitalRead(bt6) == LOW){
        ModoMenu = false;
        lcd.clear();
        digitalWrite(LedProg, LOW);
      }
    }
  // Movernos en el menu
  if(digitalRead(bt1) == LOW) {
    delay(100);
    if(digitalRead(bt1) == LOW) {
      flecha += 1;
      if(flecha == 4){
        lcd.clear();
      }
      if(flecha == 6){        
        lcd.clear();
        flecha = 1;        
      }
    }
  }

  // Control de ventas
  if (flecha == 1) {
    lcd.setCursor(8,0);
    lcd.print("Menu");
    lcd.setCursor(0,1);
    lcd.print("->");
    lcd.setCursor(2,1);
    lcd.print("Control de ventas");  
    lcd.setCursor(2,2);
    lcd.print("Calibracion");
    lcd.setCursor(2,3);
    lcd.print("Precios");

    if(digitalRead(bt2) == LOW) {
      delay(100);
      if(digitalRead(bt2) == LOW) {        
        lcd.clear();
        MoodSalesControl = true;
        sales();
      }
    }
  }

  // Calibracion
  if (flecha == 2){    
    lcd.setCursor(0,1);
    lcd.print("  ");
    lcd.setCursor(0,2);
    lcd.print("->");      
    lcd.setCursor(0,3);
    lcd.print("  ");

    if(digitalRead(bt2) == LOW){
      delay(100);
      if(digitalRead(bt2) == LOW){        
        lcd.clear();
        lcd.setCursor(8,0);
        lcd.print("Modo");
        lcd.setCursor(5,1);
        lcd.print("calibracion");
        lcd.setCursor(1,3);
        lcd.print("Seleccione producto");
        MoodCalibration = true;
        delay(1000);
        calibration();
      }
    }
  }

  // Precios
  if (flecha == 3){    
    lcd.setCursor(0,1);
    lcd.print("  ");
    lcd.setCursor(0,2);
    lcd.print("  ");      
    lcd.setCursor(0,3);
    lcd.print("->");

    if(digitalRead(bt2) == LOW){
      delay(100);
      if(digitalRead(bt2) == LOW) {        
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Modificacion de"); 
        lcd.setCursor(6,2);
        lcd.print("precios"); 
        MoodChangePrice = true;
        delay(1000);
        price();
      }
    }
  }  

  // Hopper
  if (flecha == 4){    
    lcd.setCursor(8,0);
    lcd.print("Menu");
    lcd.setCursor(0,1);
    lcd.print("->");
    lcd.setCursor(2,1);
    lcd.print("Hopper 2");  
    lcd.setCursor(2,2);
    lcd.print("Version");    

    if(digitalRead(bt2) == LOW){
      delay(100);
      if(digitalRead(bt2) == LOW) {        
        lcd.clear();
        lcd.setCursor(6, 0);
        lcd.print("Hopper 2"); 
        lcd.setCursor(0,1);
        lcd.print("->");
        lcd.setCursor(2,1);
        lcd.print("Habilitar");
        lcd.setCursor(2,2);
        lcd.print("Deshabilitar");
        MoodHopper2 = true;
        delay(1000);
        availableHopper2();
      }
    }
  }  

  // Version
  if (flecha == 5){        
    lcd.setCursor(0,1);
    lcd.print("  ");
    lcd.setCursor(2,1);
    lcd.print("Hopper 2");  
    lcd.setCursor(0,2);
    lcd.print("->");
    lcd.setCursor(2,2);
    lcd.print("Version");    

    if(digitalRead(bt2) == LOW){
      delay(100);
      if(digitalRead(bt2) == LOW) {        
        lcd.clear();
        lcd.setCursor(6, 1);
        lcd.print("Version:"); 
        lcd.setCursor(3,2);
        lcd.print(firmware);        
        MoodVersion = true;
        while(MoodVersion == true){
          // Salir de version 
          if(digitalRead(bt5) == LOW || digitalRead(bt6) == LOW){
            delay(100);
            if(digitalRead(bt5) == LOW || digitalRead(bt6) == LOW){
              MoodVersion = false;
              lcd.clear();
              flecha = 1;
            }
          }
        }
      }
    }
  }
}

}


void readMenu () {
  if ( digitalRead(bt6) == LOW ) {
    delay(100);
    if( digitalRead(bt6) == LOW ){
      ModoMenu = true;
      lcd.clear();
      digitalWrite(LedProg, HIGH);
      delay(1000);
      moodMenu();
    }
    lcd.clear();
  }
}
 
