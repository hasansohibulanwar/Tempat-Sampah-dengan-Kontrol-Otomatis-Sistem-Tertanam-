// Tempat Sampah dengan Kontrol Otomatis (Sistem Tertanam) 
// kelompok B1 



#include <NewPing.h>          //memanggil library ultrasonic
#include <Servo.h>            //memanggil library servo

//sensor sampah
#define TRIGGER_PIN   8       //pin trigger dihubungkan ke pin 8 arduino
#define ECHO_PIN      7       //pin echo dihubungkan ke pin 7 arduino
//sensor kapasitas
#define TRIGGER_PIN2  10      //pin trigger dihubungkan ke pin 10 arduino
#define ECHO_PIN2     9       //pin echo dihubungkan ke pin 9 arduino
#define MAX_DISTANCE 200      //batas jarak dari sensor

#include <LiquidCrystal_I2C.h>         //memanggil library LCD
//inisialisasi nomor pin LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);   //kalau masih tidak tampil, ganti menjadi 0x3f(alamat i2c)

#include <DFPlayer_Mini_Mp3.h>        //memanggil library DFplayer
#include <SoftwareSerial.h>

Servo servoAne;
NewPing sampah(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);      //membuat variabel sampah
NewPing kapasitas(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE); //membuat variabel kapasitas

const int ledHijau = 11;         //inisialisasi pin led hijau
const int ledMerah = 12;         //inisialisasi pin led merah

void setup() {
  Serial.begin(9600);
  lcd.begin();
  servoAne.attach(6);            //inisialisasi pin servo
  servoAne.write(0);

  mp3_set_serial (Serial);
  mp3_set_volume (50);           //pengaturan volume (0-50)

  pinMode (ledHijau, OUTPUT);     //menetapkan pin 11 sebagai Output
  pinMode (ledMerah, OUTPUT);     //menetapkan pin 10 sebagai Output
}

void loop() {
  //Rumus
  delay(50);
  int US1 = sampah.ping_cm();
  int US2 = kapasitas.ping_cm()+71;   //nilai +70 di sesuaikan dengan tinggi tong sampah

  //tampil ke serial monitor 
  Serial.print("sampah : ");
  Serial.print(US1);
  Serial.println("cm ");

  //tampilkan ke lcd
  lcd.setCursor(0, 0); //set kolom dan baris
  lcd.print(" Sisa Kapasitas ");
  lcd.setCursor(0, 1); //set kolom dan baris
  lcd.print("Tong Sampah:");
  lcd.print(US2);
  lcd.print("%");
  delay (50);

  //Jika sensor mendeteksi orang mendekati tong sampah, maka tetapkan servo di posisi 180 derajat 
  //dan nyalakan led hijau serta matikan led merah. kemudian DFpalyer menyala sebagai suara
  if(US1 <= 20){
    servoAne.write(180);            //posisikan servo di 180 derajat
    digitalWrite (ledMerah, LOW);   //LED merah padam
    digitalWrite (ledHijau, HIGH);  //LED hijau nyala
    mp3_play(1);
    delay (4000); //durasi disesuaikan dengan durasi musik
  }

   //Jika sensor tidak mendeteksi ada orang di dekat tong sampah, maka tetapkan servo di posisi 0 derajat 
   //dan nyalakan led merah serta matikan led hijau.
  else{
    servoAne.write(0);  //posisikan servo di 0 derajat
    delay(5);
    mp3_stop();
    

  digitalWrite (ledMerah, HIGH);    //LED merah nyala
  digitalWrite (ledHijau, LOW);     //LED hijau padam
  }
}
