void antiJammer(){

  lcd.clear();
  lcd.setCursor(4,1);
  lcd.print("Temporalmente");
  lcd.setCursor(0,2);
  lcd.print("Fuera de servicio");
  inicio = millis();
  fin = millis();
  tiempo = fin - inicio;
  while(tiempo < 2000){
    pulseCoin = 0;
    pulseBill = 0;
    CreditAcum = 0;
    fin = millis();
    tiempo = fin - inicio;  
  }
  lcd.clear();
}

/*=============== mensajes de status de inicio en el lcd ===============*/
bool isEnabledOptional(lcd_msg_id_t id)
{
  switch (id)
  {
    case MSG_NO_CAMBIO:
    {
      return (!Cambio1);
    }
    case MSG_LLENANDO:
    {
      return (f_despacho_producto_terminado == true);
    }
    default:
    {
      return false;
    }
  }
}

// Busca mensaje opcional habilitado en orden (NO incluye Bienvenido)
lcd_msg_id_t firstEnabledOptional(void)
{
  for (int i = 1; i < MSG_COUNT; i++)
  {
    if (isEnabledOptional((lcd_msg_id_t)i) == true)
    {
      return (lcd_msg_id_t)i;
    }
  }
  return MSG_BIENVENIDO;
}

lcd_msg_id_t nextEnabledOptionalAfter(lcd_msg_id_t currentOptional)
{
  for (int i = ((int)currentOptional + 1); i < MSG_COUNT; i++)
  {
    if (isEnabledOptional((lcd_msg_id_t)i) == true)
    {
      return (lcd_msg_id_t)i;
    }
  }
  return MSG_BIENVENIDO;
}

lcd_msg_id_t computeNextMsg(lcd_msg_id_t current)
{
  // Si no hay opcionales, siempre Bienvenido
  if (((isEnabledOptional(MSG_NO_CAMBIO) == false) && (isEnabledOptional(MSG_LLENANDO) == false)) == true)
  {
    return MSG_BIENVENIDO;
  }

  // Si estamos en Bienvenido, pasamos al primer opcional habilitado
  if (current == MSG_BIENVENIDO)
  {
    return firstEnabledOptional();
  }

  // Si estamos en un opcional, buscamos el siguiente opcional habilitado;
  // si no existe, regresamos a Bienvenido
  return nextEnabledOptionalAfter(current);
}

void renderBienvenido(void)
{
  lcd.setCursor(3,0); lcd.write(1);
  lcd.setCursor(2,1); lcd.write(2);
  lcd.setCursor(2,2); lcd.write(3);
  lcd.setCursor(2,3); lcd.write(4);
  lcd.setCursor(3,3); lcd.write(5);
  lcd.setCursor(4,3); lcd.write(6);
  lcd.setCursor(4,2); lcd.write(7);
  lcd.setCursor(4,1); lcd.write(8);

  lcd.setCursor(8,1); lcd.print("BIENVENIDO");
  lcd.setCursor(8,2); lcd.print("HIDROMATIC");
  digitalWrite(ReleOzono, LOW);
}

void renderMsg(lcd_msg_id_t id)
{
  lcd.clear();
  lcd.home();
  lcd.print("********************");

  switch (id)
  {
    case MSG_NO_CAMBIO:
    {
      lcd.setCursor(5,1);
      lcd.print("Lo  siento");
      lcd.setCursor(4,2);
      lcd.print("No hay cambio");
      break;
    }

    case MSG_LLENANDO:
    {
      lcd.setCursor(5,1);
      lcd.print("Lo  siento");
      lcd.setCursor(1,2);
      lcd.print("Llenando deposito");
      break;
    }

    default:
    {
      // Si llega algo raro, cae a Bienvenido afuera
      break;
    }
  }

  lcd.setCursor(0,3);
  lcd.print("********************");
}

void cero(void)
{
  // Siempre “por default” Bienvenido cuando current_msg == BIENVENIDO
  if (current_msg == MSG_BIENVENIDO)
  {
    renderBienvenido();
  }

  // Cambios cada 2 segundos
  if ((millis() - lcd_msg_info_time_millis_last) >= 2000)
  {
    lcd_msg_info_time_millis_last = millis();

    // Calcula siguiente
    lcd_msg_id_t next = computeNextMsg(current_msg);

    // Si cambia pantalla, renderiza
    if (next != current_msg)
    {
      current_msg = next;

      if (current_msg == MSG_BIENVENIDO)
      {
        lcd.clear();
        renderBienvenido();
      }
      else
      {
        renderMsg(current_msg);
      }
    }
    else
    {
      // Si no cambia y estamos en opcional, re-render por si cambió el LCD/estado
      if (current_msg != MSG_BIENVENIDO)
      {
        renderMsg(current_msg);
      }
    }
  }

}
/*=====================================================================*/

void saldo () {
  if ( clearCoin == true ) {
    lcd.clear();
    lcd.setCursor (7, 0);
    lcd.print("Saldo: ");
    lcd.setCursor(7,2);
    lcd.print("$");
    clearCoin = false;  
    digitalWrite(ReleOzono, HIGH);
  }
  
  unsigned long timeReset = millis() - resetCredit;
  if(timeReset > 300000){ // 5 minnutos
    CreditAcum = 0;
    lcd.clear();
    clearCoin = true;
  } 
  
  lcd.setCursor(8,2);
  lcd.print(CreditAcum);  
} 

void noCambio(){  
  lcd.clear();  
  lcd.home();
  lcd.print("*******************");  
  lcd.setCursor(4,1);
  lcd.print("Lo siento");
  lcd.setCursor(4,2);
  lcd.print("No hay cambio");
  lcd.setCursor(0,3);
  lcd.print("*******************");    
  delay(1500);  
  clearCoin = true; 
}

void recojaCambio(){  
  lcd.clear();
  lcd.home();
  lcd.print("*******************");  
  lcd.setCursor(5,1);
  lcd.print("Recoja su");
  lcd.setCursor(2,2);
  lcd.print("cambio porfavor");
  lcd.setCursor(0,3);
  lcd.print("*******************");
  delay(2500);
  clearCoin = true;
}

void entregarCambio5 () {
  
  if (CreditAcum < 81){
    inicio = millis ();
    fin = millis ();
    debounceHopper = false;
    digitalWrite(ReleHop2, HIGH);
    while (feria5 == true){
      if (CreditAcum > 4) {
        fin = millis();
        tiempo = fin - inicio; 

        if (digitalRead(ph2) == LOW && debounceHopper == true) {
          CreditAcum -= 5;
          debounceHopper = false;
          inicio = millis();
        }

        if (digitalRead(ph2) == HIGH && debounceHopper == false) {          
          debounceHopper = true;          
        }

        if (tiempo > 5000) {
          digitalWrite(ReleHop2, LOW);          
          Cambio5 = false;
          feria5 = false;
        }
      }
      else {
        digitalWrite(ReleHop2, LOW);          
        feria5 = false;
      }
    }
  }

  if (CreditAcum > 80) {
    inicio = millis ();
    fin = millis ();
    CreditAcumAnterior = 0;
    debounceHopper = false;
    digitalWrite(ReleHop2, HIGH);
    while( feria5 == true){
      fin = millis();
      tiempo = fin - inicio;
      if(digitalRead(ph2) == LOW && debounceHopper == true ) {
          CreditAcum -= 5;    
          CreditAcumAnterior += 1;      
          debounceHopper = false;
          inicio = millis();
        }

        if(digitalRead(ph2) == HIGH && debounceHopper == false){
          debounceHopper = true;          
        }
      if (tiempo > 5000){
        digitalWrite(ReleHop2, LOW);       
        Cambio5 = false;
        feria5 = false;
      }
      if (CreditAcumAnterior == 16){
        digitalWrite(ReleHop2, LOW);        
        feria5 = false;
        recoja= true;
        recojaCambio();
      }
    }    
  }
  feria5 = true;      
}

void entregarCambio1 () {
  
  if(CreditAcum < 40){
    inicio = millis();
    fin = millis();
    debounceHopper = false;
    digitalWrite(ReleHop1, HIGH);
    while( feria1 == true){
      if(CreditAcum > 0){
        fin = millis();
        tiempo = fin - inicio;     
        if(digitalRead(ph1) == LOW && debounceHopper == true ) {
          CreditAcum -= 1;              
          debounceHopper = false;
          inicio = millis();
        }

        if(digitalRead(ph1) == HIGH && debounceHopper == false){
          debounceHopper = true;          
        }
        if (tiempo > 5000){
          digitalWrite(ReleHop1, LOW);          
          Cambio1 = false;
          feria1 = false;
        } 
      }  
      else {
        digitalWrite(ReleHop1, LOW);      
        feria1 = false;
      }     
    }
  }

  if ( CreditAcum > 39 ){
    inicio = millis();
    fin = millis();
    CreditAcumAnterior = 0;
    debounceHopper = false;
    digitalWrite(ReleHop1, HIGH);
    while( feria1 == true){
      fin = millis();
      tiempo = fin - inicio;

      if(digitalRead(ph1) == LOW && debounceHopper == true ) {
          CreditAcum -= 1;         
          CreditAcumAnterior += 1;     
          debounceHopper = false;
          inicio = millis();
        }

        if(digitalRead(ph1) == HIGH && debounceHopper == false){
          debounceHopper = true;          
        }
      if (tiempo > 5000){
        digitalWrite(ReleHop1, LOW);      
        Cambio1 = false;
        feria1 = false;
      }

      if (CreditAcumAnterior == 30){
        digitalWrite(ReleHop1, LOW);        
        feria1 = false;          
        recojaCambio();
      }
    }    
  }
  feria1 = true;   
}

void precioMostrar () {
  lcd.clear();
  lcd.home();
  lcd.print("*******************");
  lcd.setCursor (6,1);
  lcd.print("Precio:");
  lcd.setCursor(9,2);
  lcd.print("$");
  lcd.print(precio);
  lcd.setCursor(0,3);
  lcd.print("*******************");
  delay(1500);
  clearCoin = true;
  lcd.clear();
}

void enjuague () {
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Enjuague");
  lcd.setCursor(5,2);
  lcd.print("activado");   
  inicio = millis();
  fin = millis();
  tiempo = fin - inicio;
  digitalWrite(ReleEnjuague, HIGH);
  while( tiempo < tiempoEnjuague){
    fin = millis();
    tiempo = fin - inicio;
  }
  digitalWrite(ReleEnjuague, LOW);  
  EnjuagueFirst = false;  
  CreditAcum -= priceEnjuague;
  clearCoin = true;
}

void paro () {
  digitalWrite(ReleProducto, LOW);  
  paroActivado = true;
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Paro activado");
  lcd.setCursor(0,1);
  lcd.print("presione nuevamente");
  lcd.setCursor(2,2);
  lcd.print("para desactivar");
  lcd.setCursor(0,3);
  lcd.print("Tiempo:");  
  inicio = millis();  
  tiempoParo = 60000;
  while(paroActivado == true){    
    fin = millis();
    tiempo = fin - inicio; 
    if( tiempo > 1000){
      tiempoParo -= 1000;
      inicio = millis();
      tiempoMuestra = tiempoParo / 1000;
    }        
    if (tiempoMuestra < 10){
      lcd.setCursor(9,3);
      lcd.print(" ");
    }
    lcd.setCursor(8,3);
    lcd.print(tiempoMuestra);
    if(tiempoParo < 2000){
      paroActivado = false;
      tiempoExcedido = true;
    }
    if(digitalRead(bt1) == LOW){
      delay(100);
      if(digitalRead(bt1) == LOW){
        paroActivado = false;                
        digitalWrite(ReleProducto, HIGH);
      }
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Despachando...");
  lcd.setCursor(5,2);
  lcd.print("%");
}
void producto () { // NOTE: Despacho: despacho de producto
  unsigned long Umbral_pulsos_verify_time_last = 0;
  unsigned long Umbral_pulsos_verify_time_current = 0;
  unsigned long UMBRAL_PULSOS_VERIFY_INTERVAL = 2000;
  unsigned int pulsos_Compara = 0;
  unsigned int error_umbral_index = 0;
  bool f_error_de_flujo = false;
  uint8_t PULSOS_UMBRAL_INTERVALO_TOLERANCIA = 4;
  uint8_t PULSOS_UMBRAL_LIMITE_ERRORES = 3;
  
  pulseSensor = 0;
  pulseAcumSensor = 0;

#if DEBUG_LOG_UART_ENABLED == 1
  Serial.println("Despacho - INICIADO "); // HACK: Log: despacho de producto iniciado
  Serial.println("Despacho - precio: " + String(precio));
  Serial.println("Despacho - credit acum: " + String(CreditAcum));
  Serial.println("Despacho - pulsos calibracion: " + String(product));
  Serial.println("Despacho - umbral pulsos: " + String(Despacho_Umbral_Pulsos));
#endif

  if (Despacho_Umbral_Pulsos > PULSOS_UMBRAL_INTERVALO_TOLERANCIA)
  {
    Despacho_Umbral_Pulsos -= PULSOS_UMBRAL_INTERVALO_TOLERANCIA;
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Despachando...");
  lcd.setCursor(5,2);
  lcd.print("%");

  despacho_porcentaje_lcd_refresh_time_millis_last = millis();
  Umbral_pulsos_verify_time_last = millis();
  digitalWrite(ReleProducto, HIGH);
  while(pulseAcumSensor < product)
  {
    if(digitalRead(bt1) == LOW)
    {
      delay(100);
      if(digitalRead(bt1) == LOW)
      {
          if(contadorParo<=2)
          {
            paro();
            contadorParo++;
          }
      }
    }

    if(pulseSensor > 0)
    {
      pulseAcumSensor += pulseSensor;
      pulsos_Compara += pulseSensor; 
      pulseSensor = 0;     
    }

    qlitros = (unsigned int)((pulseAcumSensor * 100.0)/ product);

    despacho_porcentaje_lcd_refresh_time_millis_current = millis();
    if(despacho_porcentaje_lcd_refresh_time_millis_current - despacho_porcentaje_lcd_refresh_time_millis_last >= DEPACHO_PORCENTAJE_LCD_REFRESH_INTERVAL)
    {
      despacho_porcentaje_lcd_refresh_time_millis_last = despacho_porcentaje_lcd_refresh_time_millis_current;
            
      lcd.setCursor(7,2);
      lcd.print(qlitros);
#if DEBUG_LOG_UART_ENABLED == 1
      // Serial.println("Despacho - pulsos/porcentaje: " + String(pulseAcumSensor) + " / " + String(qlitros)); // HACK: Log: porcentaje/porcentaje despachado
#endif
    }

    Umbral_pulsos_verify_time_current = millis();
    if ((Umbral_pulsos_verify_time_current - Umbral_pulsos_verify_time_last) > UMBRAL_PULSOS_VERIFY_INTERVAL)
    {
      Umbral_pulsos_verify_time_last = Umbral_pulsos_verify_time_current;

      if (pulsos_Compara < Despacho_Umbral_Pulsos)
      {
        error_umbral_index += 1;

#if DEBUG_LOG_UART_ENABLED == 1
        Serial.print("Despacho pulsos - error_umbral_index: "); // HACK: log: despacho pulsos: umbral error index
        Serial.print(error_umbral_index);
        Serial.print(" / ");
        Serial.println(PULSOS_UMBRAL_LIMITE_ERRORES);

        Serial.print("Despacho pulsos - insuficientes pulsos_check/umbral: ");
        Serial.print(pulsos_Compara);
        Serial.print(" / ");
        Serial.println(Despacho_Umbral_Pulsos);

        Serial.print("Despacho pulsos - pulsos/porcentaje: ");
        Serial.print(pulseAcumSensor);
        Serial.print(" / ");
        Serial.println(qlitros);
#endif

        pulseAcumSensor -= (pulsos_Compara);

        if (error_umbral_index == PULSOS_UMBRAL_LIMITE_ERRORES)
        {
          // detiene el despacho por error de flujo
          f_error_de_flujo = true;
#if DEBUG_LOG_UART_ENABLED == 1
          Serial.print("Despacho pulsos - f_error_de_flujo: "); // HACK: log: despacho pulsos: f_error_de_flujo
          Serial.println(f_error_de_flujo);
#endif
          break;
        }
      }
      else
      {
        error_umbral_index = 0;
      }

      pulsos_Compara = 0;
    }
   
    if(tiempoExcedido == true)
    {
      // pulseAcumSensor = product;
      tiempoExcedido = false;
      break;
    }
  }

  digitalWrite(ReleProducto, LOW);
  // CreditAcum -= precio;
  if (qlitros < 100)
  {
    unsigned int rest_prod = (qlitros * precio) / 100;
    CreditAcum -= rest_prod;
  }
  else
  {
    CreditAcum -= precio;
  }

  lcd.clear();
  EnjuagueFirst = true;
  clearCoin = true; //REVIEW: creditos: ¿resetear creditos acumulados en el monedero al finalizar el despacho?
  // pulseCoin = 0;
  // pulseBill = 0; 
  resetCredit = millis();

  if(f_error_de_flujo)
  {    
    lcd.setCursor(2,1);
    lcd.print("Producto agotado");
#if FLAG_LCD_MSG_PRODUCTO_AGOTADO == 1
    f_despacho_producto_terminado = true;
    llenando_deposito_time_millis_last = millis();
#endif
  }
  else
  {
    lcd.setCursor(3,1);
    lcd.print("Gracias por su");
    lcd.setCursor(6,2);
    lcd.print("compra!");
    f_despacho_producto_terminado = false;
  }

#if DEBUG_LOG_UART_ENABLED == 1
  Serial.println("Despacho - FINALIZADO");
  Serial.println("Despacho - pulsos/porcentaje despachados: " + String(pulseAcumSensor) + " / " + String(qlitros)); // HACK: Log: pulsos/porcentaje despachado
  Serial.println("Despacho - credit acum: " + String(CreditAcum));
#endif

  delay(2000);
  lcd.clear();
}
 
