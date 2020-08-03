#include <WiFi.h>

// Konek ke WIFI
const char* ssid     = "66";
const char* password = "11111111";

// Port Web Server
WiFiServer server(80);

// inisialisasi variable untuk HTTP Request
String header;

// Variable Status
String statusPin18 = "off";
String statusPin19 = "off";
String statusPinSemua="off";
String a1 = "";
String a2 = "";

// Variable OutPut
const int outputPin18 = 18;
const int outputPin19 = 19;
//const int outputPin21 = 21;

void setup() {
  Serial.begin(115200);
  // Penentuan Pin Output
  pinMode(outputPin18, OUTPUT);
  pinMode(outputPin19, OUTPUT);
//  pinMode(outputPin21, OUTPUT);
  // pin di set OFF
  digitalWrite(outputPin18, LOW);
  digitalWrite(outputPin19, LOW);
//  digitalWrite(outputPin21, LOW);
  // Connect Ke jaringan WIFI
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Menampilkan IP ke Serial Monitor
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //Memulai Server
  server.begin();
  digitalWrite(outputPin18, HIGH);
  digitalWrite(outputPin19, HIGH);
//  digitalWrite(outputPin21, HIGH);
  digitalWrite(outputPin18, LOW);
  digitalWrite(outputPin18, LOW);
//  digitalWrite(outputPin21, LOW);
}

void loop() {
  // Inisialisasi Objek penggunan dengan mendengarkan request dari Client/Pengguna
  WiFiClient client = server.available(); 
  
  // Mengeksekusi permintaan pengguna saat tombol ditekan

  // MENGECEK TOMBOL MANA YANG DITEKAN DI HALAMAN WEB, DAN MENGONTROL HASILNYA. SEPERTI YANG TELAH DILIHAT SEBELUMNYA, MEMBUAT PERMINTAAN PADA BERBAGAI URL TERGANTUNG PADA TOMBOL YANG DITEKAN.
  if (client) {                             // Jika Client Konnek
    Serial.println("Perintah Baru");        // Tampilkan Client
    String currentLine = "";                // Inisialisasi menyimpan data dari client
    while (client.connected()) {            // perulangan jika client Connect
      if (client.available()) {             // Jika client ada
        char c = client.read();             // membaca nilai client kemudian
        Serial.write(c);                    // cetak
        header += c;
        if (c == '\n') {                    // 
          if (currentLine.length() == 0) {      // jika panjang data sama dengan 0 berarti tidak ada daata
            client.println("HTTP/1.1 200 OK");        // Cetak di serial monitor HTTP REQUEST OK
            client.println("Connection: close");
            client.println();

            // JIKA TOMBOL DITEKAN MEMRINTAHKAN REQUEST PERINTAH PADA URL/GET

            // MISALNYA, JIKA TELAH MENEKAN TOMBOL LED ON 18 , ESP32 MENERIMA PERMINTAAN PADA URL / 18 / ON 
            // (KITA DAPAT MELIHAT BAHWA INFORMASI ITU DI HEADER HTTP PADA SERIAL MONITOR). JADI, KITA DAPAT MEMERIKSA APAKAH HEADER BERISI EKSPRESI GET / 18/ ON . 
            // JIKA ITU BERISI, KAMI MENGUBAH VARIABEL STATUS OUTPUT18 KE ON, DAN ESP32 MENYALAKAN LED.
            if (header.indexOf("GET /18/on") >= 0) { // jika tombol ditekan lampu pin 18 ditekan mengganti request HTTP di url 
              Serial.println("PIN 18 on");          // cetak di Serial monitor
              statusPin18 = "on";                   //  Mengganti status Pin jadi ON
              digitalWrite(outputPin18, HIGH);      // Dan memerintahkan pin lampu menyala
            } else if (header.indexOf("GET /18/off") >= 0) {  // sebaliknya, lain jika ditekan OFF
              Serial.println("PIN 18 off");
              statusPin18 = "off";
              digitalWrite(outputPin18, LOW);
            } else if (header.indexOf("GET /19/on") >= 0) {
              Serial.println("PIN 19 on");
              statusPin19 = "on";
              digitalWrite(outputPin19, HIGH);
            } else if (header.indexOf("GET /19/off") >= 0) {
              Serial.println("PIN 19 off");
              statusPin19 = "off";
              digitalWrite(outputPin19, LOW);
            } else if (header.indexOf("GET /on") >= 0) {
              Serial.println("PIN Semua ON");
              statusPin18 = "on";
              statusPin19 = "on";
              statusPinSemua = "on";
              digitalWrite(outputPin18, HIGH);
              digitalWrite(outputPin19, HIGH);
            } else if (header.indexOf("GET /off") >= 0) {
              Serial.println("PIN Semua OFF");
              statusPinSemua = "off";
              statusPin18 = "off";
              statusPin19 = "off";
              digitalWrite(outputPin18, LOW);
              digitalWrite(outputPin19, LOW);
            }

            // HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            // CSS
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #5bc0de; border: none; border-radius:10%; color: white; padding: 10px 20px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 5px 5px 5px #aaaaaa;}");
            client.println("h3{background-size: auto;background-color:#E0F2F1;padding:10 0;}");
            client.println(".button2 {background-color: #0f887d;}</style></head>");

            // JUDUL
            client.println("<body><h1>Control LED</h1>");
            
            // PIN 18
            client.println("<p>PIN 18 - Status " + statusPin18 + "</p>");
            // If the statusPin18 is off, it displays the ON button
            if (statusPin18 == "off") {
              client.println("<p><a href=\"/18/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/18/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // PIN 19
            client.println("<p>PIN 19 - Status " + statusPin19 + "</p>");
            if (statusPin19 == "off") {
              client.println("<p><a href=\"/19/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/19/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            
            // PIN SEMUA
            client.println("<p>Semua PIN - Status " + statusPinSemua + "</p>");
//            if(statusPinSemua=="off"){
//                 
//            }else{
//              client.println("<p><a href=\"/off\"><button class=\"button button2\">OFF</button></a></p>");  
//            }
            if (statusPin19=="on"&&statusPin18=="on") {
              client.println("<p><a href=\"/on\"><button class=\"button\">ON</button></a></p>");  
            }
            client.println("</body></html>");
            client.println();
            //perintah tombol ditekan berakhir
            break;
          } else { // lain jika tidak ada data yang diberikan Client
            currentLine = "";
          }
        } else if (c != '\r') {  // jika  tampilannya tidak mempunyai data maka tidak di tampilkan/return
          currentLine += c;      // 
        }
      }
    }
    // menghapus nilai dari variable penyimpan Request
    // KETIKA RESPONS BERAKHIR, KAMI MENGHAPUS VARIABEL HEADER , DAN MENGHENTIKAN KONEKSI DENGAN KLIEN DENGAN CLIENT.STOP()
    header = "";
    // Kluar dari Koneksi
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
