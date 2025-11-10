unsigned long primerTiempo = 0;
bool primeraVez = true;
bool verdadero = false;
//bool firstTimeBill = true;
//volatile long firstTime = 0;
// Pantalla LCD
#define I2C_ADDR 0x27 
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Botones
#define bt1 4 // Enjuague
#define bt2 5 // Producto 1
#define bt3 6 // Producto 2
#define bt4 7 // Producto 3
#define bt5 15 // Cambio
#define bt6 12 // Menu
#define btReset 10 //RESET
int LedReset = 21;
bool moodRestaurar = false;
int LedProg = 13;
bool ModoMenu=false;
unsigned int CreditAcum = 0;


// Multimoneda

const byte interruptCoin = 47; //47 16
volatile int pulseCoin = 0;
unsigned long millisUltCoin = 0;
unsigned int pulseCoinAcum = 0;
//byte MaxTimePulseCoin = 45; // Configurar con el monedero 

unsigned long creditTime = 0;
unsigned long timeWait = 1000;
unsigned long resetCredit = 0;

// Billetero
unsigned long DEBOUNCE_TIME = 100;
const byte interruptBill = 45; //45 19
volatile int pulseBill = 0;
volatile long millisUltBill = 0;
unsigned int pulseBillAcum = 0;
unsigned int pulseBillAnterior = 0;
//byte MaxTimePulseBill = 100; // Configurar con el billetero

// HOPPER
const int ph1 = 3;
const int ph2 = 20;
bool Cambio5 = false;
bool Cambio1 = true;
bool debounceHopper = true;
bool feria5 = true;
int CreditAcumAnterior = 0;
bool feria1 = true;
bool recoja = false;

// Sensor de flujo
const byte sensor = 2;
volatile int pulseSensor = 0;
unsigned long LastTimeSensor = 0;
unsigned long pulseAcumSensor = 0;

float producto1 = 1, producto2 = 1, producto3 = 1, product = 1, tiempoEnjuague = 0;
unsigned long tiempo = 0, inicio = 0, fin = 0, tiempoParo = 0;
unsigned long tiempoMuestra = 0;
unsigned long qlitros = 0;
int lastState = LOW;

// ====================================================================

// OUTPUTS 
const int ReleOzono = 41;
const int ReleHop1 = 40;
const int ReleHop2 = 39;
const int ReleEnjuague = 38;
const int ReleProducto = 37;

// ====================================================================

// MENU
byte flecha = 1;
byte hopper2 = 0;
bool MoodSalesControl = false;
bool MoodCalibration = false;
bool MoodChangePrice = false;
bool MoodHopper2 = false;
bool MoodVersion = false;

bool moodEnjuague = false;
bool moodProducto1 = false;
bool moodProducto2 = false;
bool moodProducto3 = false;

bool changepriceEn = false;
bool changeprice1 = false;
bool changeprice2 = false;
bool changeprice3 = false;

unsigned int ventasEnjuague = 0;
unsigned int ventasProducto1 = 0;
unsigned int ventasProducto2 = 0;
unsigned int ventasProducto3 = 0;

unsigned int priceEnjuague = 2;
unsigned int price1 = 4;
unsigned int price2 = 10;
unsigned int price3 = 14;
unsigned int precio = 0;

// ====================================================================

bool show = true;
bool borrar = true;
bool clearCoin = true;

bool EnjuagueFirst = true;
bool paroActivado = false;
bool tiempoExcedido = false;
int contadorParo = 1;

// DISPLAY LCD 
 byte f1[8] = {
0b00000,
0b00000,
0b01110,
0b10001,
0b10001,
0b10001,
0b10001,
0b10001
};

byte f2[8] = {
0b00001,
0b00110,
0b01000,
0b01000,
0b10000,
0b10000,
0b01000,
0b01000
};

byte f3[8] = {
0b01000,
0b01000,
0b01000,
0b01000,
0b01000,
0b01000,
0b01000,
0b01000
};

byte f4 [8] = {
0b01000,
0b01000,
0b10000,
0b10000,
0b10000,
0b01000,
0b00100,
0b00011
};

byte f5 [8] = {
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b11111

};

byte f6 [8] = {
0b00010,
0b00010,
0b00001,
0b00001,
0b00001,
0b00010,
0b00100,
0b11000

};
byte f7 [8] = {
0b00010,
0b00010,
0b00010,
0b00010,
0b00010,
0b00010,
0b00010,
0b00010

};

byte f8 [8] = {
0b10000,
0b01100,
0b00010,
0b00010,
0b00001,
0b00001,
0b00010,
0b00010

};
