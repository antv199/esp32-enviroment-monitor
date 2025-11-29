#define THINGER_SERIAL_DEBUG

#include <ThingerESP32.h> //  Thinger library
#include "arduino_secrets.h" // Αρχείο με τα credentials
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> // LCD
#include "DHT.h" // DHT 
#include <WiFi.h> // Wifi Connection

// Ο κάθετος στην πλακέτα DHT είναι 
#define DHTPIN 2
#define DHTTYPE DHT11

#define LEDPIN 3
int ledState; // Κατάσταση του LED

// Config για δοκιμες LCD, γιατι αν δεν υπαρχει στην πλακετα, ο watchdog timer τρελένεται με errors.
// test = 0 για να ενεργοποιηθεί το LCD
// test = 1 χωρίς LCD
int test = 0;

// Αρχικοποιήσεις
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DHT dht(DHTPIN, DHTTYPE);
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// Γενικές μεταβλητές
float temp, humid, temp5mins, tempSum;
int tempCount;

// Επιστροφή της θερμοκρασίας
float getTemp(){
  temp = round(dht.readTemperature());

  tempSum = tempSum+temp;
  tempCount = tempCount + 1;

  return temp;
}

// Επιστροφή της υγρασίας
float getHumid(){
  humid = round(dht.readHumidity());
  return humid;
}

// Επιστροφή της μέσης θερμοκρασίας των τελευταίων 5 λεπτών
float get5minTemp(){
  return temp5mins;
}

// Επιστροφή της κατάστασης του LED
int getLedState(){
  return ledState;
}

void setup() {
  Serial.begin(115200); // Serial Monitor στα 115200 baud

  if (test == 0) {
    dht.begin();

    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Not connected");
    lcd.setCursor(0, 1);
    lcd.print("0.0.0.0");

    // LED Logic
    pinMode(LEDPIN, OUTPUT);
    ledState = digitalRead(LEDPIN);
  }

  thing.add_wifi(SSID); // ξεχνάμε το SSID_PASSWORD, γιατί δημιουργεί πρόβλημα στην σύνδεση

  thing["TempHum"] >> [](pson &out){
    out["temperature"] = getTemp(); // (i) Στρογγυλοποίηση αποτελέσματος
    out["humidity"] = getHumid(); // (ii) Στρογγυλοποίηση αποτελέσματος
    out["ledState"] = getLedState(); // (iii)	Επίδειξη κατάστασης του LED μέσω του dashboard
    out["avg5MinTemp"] = get5minTemp(); // (iv)	Υπολογισμός και απεικόνιση της μέσης θερμοκρασίας ανά 5 λεπτά
  };

  thing["GPIO_LED"] << digitalPin(LEDPIN); // (iii)	Δυνατότητα αλλαγής κατάστασης του LED μέσω του dashboard
}

void loop() {

  thing.handle();
  thing.write_bucket("BucketId", "TempHum");

  if (test == 0) {

    ledState = digitalRead(LEDPIN); // Ενημέρωση της κατάστασης του LED

    // Δημιουργία επαναχρησιμοποιήσιμου string με τα δεδομένα
    String stats = "T:" + String(temp) + " H:" + String(humid);

    // Εμφάνιση στο Serial Monitor
    Serial.println(stats);
    Serial.println("tempSum: " + String(tempSum) + " tempCount: " + String(tempCount));
    Serial.println(WiFi.localIP().toString());

    // Εμφάνιση στο LCD
    lcd.setCursor(0, 0);
    lcd.print(stats); // Εμφάνιση θερμοκρασίας και υγρασίας
    lcd.setCursor(0, 1);

    // Εμφάνιση IP και κατάστασης LED
    // 
    // π.χ.
    // 192.168.1.1 L:1
    lcd.print(WiFi.localIP().toString() + " L:" + String(ledState));

  }

  // Υπολογισμός μέσης θερμοκρασίας ανά 5 λεπτά
  // (5 mins x 60 sec) / 2 sec = 150 counts per 5 mins
  if (tempCount >= 150) {
    temp5mins = tempSum / tempCount;
    Serial.println("5 mins" + String(get5minTemp()));

    tempSum = 0;
    tempCount = 0;
  }

  // (iv) Λιγότερο απο 2 δευτερόλεπτα δεν γίνεται, γιατί ο DHT δεν μπορεί να διαβάσει τιμές τόσο συχνά
  // και το thinger.io ρίχνει cooldown στα αιτήματα δεδομένων
  delay(2000);
}
