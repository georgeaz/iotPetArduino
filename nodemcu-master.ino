#include <HTTPClient.h>
#include <Wire.h>
#include <WiFi.h>

#define ssid "TechPublic"
#define password ""

#define requestSucceeded 200

const char* host = "https://europe-west3-iotpet-6a3d7.cloudfunctions.net";
String send_message = "/sendMessage";
String get_uid = "/getCurrentUid";
String get_uid_data = "/getCurrentLoggedInInfo";
String update_uid_data = "/updateCurrentLoggedInInfo";

String pet_status = "";
//int message_sent_flag = 1;
String payload_result = "";
String logged_uid = "NUN";
//int new_account_flag = 1;
String last_command = "";

//*****************************************************************************
String level = "";
String mood = "";
int points = 0;
String petType = "";
//*****************************************************************************

int SDAPin = 17;
int SCLPin = 16;

//*****************************************************************************
String getLoggedUid();
//*****************************************************************************


void setup() {
 Serial.begin(9600); /* begin serial for debug */
 Wire.begin(SDAPin, SCLPin); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
 delay(6000);

 WiFi.begin(ssid, password);
 Serial.println("Connecting...");
 
 while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.println("Pending :)");
  }
 Serial.println("Successful");
}

void loop() {
  logged_uid = getLoggedUid(); //"QH3gwAXkTGXNjVOmtydqJPJSBM23";
  delay(2000);
  if(logged_uid != "NUN") {
    getCurrentUidData();
    delay(1000);
//  Wire.beginTransmission(8); /* begin with device address 8 */
//  Wire.write("Hello Arduino");  /* sends hello string */
//  Wire.endTransmission();    /* stop transmitting */
  
    Wire.requestFrom(8, 13); /* request & read data of size 13 from slave */
    pet_status = "";
    //play_command_flag = 0;
    Serial.println("i'm outside the while");
    while(Wire.available()){
      char c = Wire.read();
      Serial.println("in while: " + pet_status);
      if (c == '$'){
        
        Serial.println("pet status: " + pet_status + ".");
        
        if (pet_status == "Play" && last_command != "Play") {
          last_command = "Play";
          Serial.println("i'm in nodeMCU play");
          delay(1000);
          if (logged_uid != "NUN") {
            mood = "bored";
            sendMessage(logged_uid, "Hey+Buddy,+Please+pet+me.");
            updateCurrentUidData();
            delay(5000);
          }
        }
    
        if (pet_status == "Sleep" && last_command != "Sleep") {
          Serial.println("i'm in nodeMCU sleep");
          last_command = "Sleep";
          delay(1000);
          if (logged_uid != "NUN") {
            mood = "sleepy";
            sendMessage(logged_uid, "Hey+Buddy,+I+want+to+sleep.");
            updateCurrentUidData();
            delay(5000);
            // eza bt3mle flag la kol mood, lma tfote 3la flag el hungry mtlan, bt3mle flag_hungry = 0, wba2e el flags = 1
          }
        }
  
        if (pet_status == "Hungry" && last_command != "Hungry") {
          Serial.println("i'm in nodeMCU hungry");
          last_command = "Hungry";
          delay(1000);
          if (logged_uid != "NUN") {
            mood = "hungry";
            sendMessage(logged_uid, "Hey+Buddy,+I+am+hungry.");
            updateCurrentUidData();
            delay(5000);
            //message_sent_flag = 0;
            // eza bt3mle flag la kol mood, lma tfote 3la flag el hungry mtlan, bt3mle flag_hungry = 0, wba2e el flags = 1
          }
        }
          
        if (pet_status == "Thirsty" && last_command != "Thirsty") {
          Serial.println("i'm in nodeMCU thirsty");
          last_command = "Thirsty";
          delay(1000);
            if (logged_uid != "NUN") {
              mood = "thirsty";
              sendMessage(logged_uid, "Hey+Buddy,+I+am+thirsty.");
              updateCurrentUidData();
              delay(5000);
              //message_sent_flag = 0;
              // eza bt3mle flag la kol mood, lma tfote 3la flag el hungry mtlan, bt3mle flag_hungry = 0, wba2e el flags = 1
            }
        }
    
        if (pet_status == "Dirty" && last_command != "Dirty") {
          Serial.println("i'm in nodeMCU dirty");
          last_command = "Dirty";
          delay(1000);
            if (logged_uid != "NUN") {
              mood = "dirty";
              sendMessage(logged_uid, "Hey+Buddy,+I+want+to+shower.");
              updateCurrentUidData();
              delay(5000);
              //message_sent_flag = 0;
              // eza bt3mle flag la kol mood, lma tfote 3la flag el hungry mtlan, bt3mle flag_hungry = 0, wba2e el flags = 1
            }
        }
  
        if (pet_status == "InLove" && last_command != "InLove") {
          Serial.println("i'm in nodeMCU inLove");
          last_command = "InLove";
          delay(1000);
            if (logged_uid != "NUN") {
              mood = "inLove";
              //sendMessage(logged_uid, "Hey+Buddy,+I+am+in+love.");
              updateCurrentUidData();
              delay(5000);
            }
        }
  
        if (pet_status == "Happy" && last_command != "Happy") {
          Serial.println("i'm happy");
          last_command = "Happy";
          delay(1000);
            if (logged_uid != "NUN") {
              mood = "happy";
              points += 10;
              if(points == 100) {
                points = 0;
                int tmp = atoi(level.c_str());
                tmp += 1;
                level = String(tmp);
                mood = "celebrating";
               }
               updateCurrentUidData();
               delay(5000);
              //
              // eza bt3mle flag la kol mood, lma tfote 3la flag el hungry mtlan, bt3mle flag_hungry = 0, wba2e el flags = 1
            }
        }
        Serial.println(pet_status);
        if (pet_status == "Sad" && last_command != "Sad") {
          Serial.println("i'm sad");
          last_command = "Sad";
            if (logged_uid != "NUN") {
               mood = "sad";
               // display happy face move it's ears
               updateCurrentUidData();
               delay(5000);
              // eza bt3mle flag la kol mood, lma tfote 3la flag el hungry mtlan, bt3mle flag_hungry = 0, wba2e el flags = 1
            }
        }
  
        // add different pet statuses.
        
        pet_status = "";
        delay(800);
      } else {
          pet_status += String(c);
      }
    
    //Serial.print(c);
  }
 Serial.println();
}

 delay(1000);
}

void sendMessage(String uid, String msg) {
  WiFiClient client_req;
  const int httpsPort = 443;
  if (!client_req.connect(host, httpsPort)) {
    Serial.println("connection failed!");
    return;
  }

  Serial.println("Requesting URL: ");
  Serial.println(send_message);

  String addr = host + send_message + "?uid=" + uid + "&message=" + msg;
  HTTPClient http;
  http.begin(addr);
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // in POST
  auto httpCode = http.GET();
  Serial.println(httpCode);
  if (httpCode != requestSucceeded) {
      //sendMessage(uid, msg);
      Serial.println("http request failed");
  }
  payload_result = http.getString();
  //Serial.println(payload_result);
 if (payload_result == "message sent") {
    Serial.println("Message has been sent successfully.");
 } else {
    Serial.println("Message was not sent.");
 }
  http.end();
  Serial.println("closing connection");
}

String getLoggedUid() {
  WiFiClient client_req;
  const int httpsPort = 443;
  if (!client_req.connect(host, httpsPort)) {
    Serial.println("connection failed!");
    return "NUN";
  }

  //Serial.println("Requesting URL: ");
  //Serial.println(get_uid);

  String addr = host + get_uid;
  HTTPClient http;
  http.begin(addr);
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // in POST
  auto httpCode = http.GET();
  //Serial.println(httpCode);
  if (httpCode != requestSucceeded) {
      //getLoggedUid();
      Serial.println("http request failed");
  }
  payload_result = http.getString();
  //Serial.println(payload_result);
 if (payload_result == "NUN" || petType == "null") {
    Wire.beginTransmission(8); /* begin with device address 8 */
    Wire.write('0');  /* sends 0 which indicates that no user is logged */
    Wire.endTransmission();    /* stop transmitting */
    Serial.println("Please Login");
    //getLoggedUid();
 } else {
    Wire.beginTransmission(8); /* begin with device address 8 */
    Wire.write('1');  /* sends 1 which indicates that a user is logged */
    Wire.endTransmission();    /* stop transmitting */
    Serial.println("You are logged in");
  }
  http.end();
  //Serial.println("closing connection");
  return payload_result;
}

void getCurrentUidData() {
  WiFiClient client_req;
  const int httpsPort = 443;
  if (!client_req.connect(host, httpsPort)) {
    Serial.println("connection failed!");
    return;
  }

  //Serial.println("Requesting URL: ");
  //Serial.println(get_uid_data);

  String addr = host + get_uid_data;
  HTTPClient http;
  http.begin(addr);
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // in POST
  auto httpCode = http.GET();
  //Serial.println(httpCode);
  if (httpCode != requestSucceeded) {
     // getCurrentUidData();
     Serial.println("in getCurrentUidData: http request failed");
  }
  payload_result = http.getString();
  if (payload_result == "NUN") {
      //new_account_flag = 1;
      //getCurrentUidData();
  }
  else /*if (new_account_flag)*/ {
    // hon lazem n2asem el payload_result l 4 el sadot elle b2albha {level, mood, points, petType}
    extractDataFromPayload(payload_result);
    delay(500);
    if(petType != "" && petType != "null") {
      String str = "@";
      str += petType;
      str += ',';
      str += level;
      str += "#";
      Wire.beginTransmission(8); /* begin with device address 8 */
      Wire.write(str.c_str());  /* sends string */
      Wire.endTransmission();    /* stop transmitting */
    }
    
//    for (int i = 0; i < petType.length(); i++) {
//      Wire.beginTransmission(8); /* begin with device address 8 */
//      Wire.write(petType[i]);  /* sends hello string */
//      Wire.endTransmission();    /* stop transmitting */
//    }
   // new_account_flag = 0;
  }
  http.end();
  //Serial.println("closing connection");
}

void updateCurrentUidData() {
  if(level == "" || mood == "" || (level == "" && points == 0)) {
    Serial.println("http request failed");
    return;
  }
  WiFiClient client_req;
  const int httpsPort = 443;
  if (!client_req.connect(host, httpsPort)) {
    Serial.println("connection failed!");
    return;
  }

  //Serial.println("Requesting URL: ");
  //Serial.println(get_uid_data);

  String points_str = String(points);  
  String addr = host + update_uid_data + "?petLevel=" + level + "&petMood=" + mood + "&petPoints=" + points_str;
  HTTPClient http;
  http.begin(addr);
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // in POST
  auto httpCode = http.GET();
  //Serial.println(httpCode);
  if (httpCode != requestSucceeded) {
      //getCurrentUidData();
      Serial.println("http request failed");
  }
  payload_result = http.getString();
    
  if(payload_result != "success")
    Serial.println("Update failed.");
  else
    Serial.println("Update succeeded.");
    
  http.end();
  //Serial.println("closing connection");
}

//************************************************************************************
void extractDataFromPayload(String str) {
  level = "";
  petType = "";
  mood = "";
  
  str.replace("\"","");
  str.replace("{","");
  str.replace("}","");
  str.replace("petLevel:","");
  str.replace("petType:","");
  str.replace("petMood:","");
  str.replace("petPoints:","");
  int counter = 1;
  String c = "";
  for(int i = 0; i < str.length(); i++) {
    if(String(str[i]) != ",") {
      switch(counter) {
        case 1: level += str[i]; continue;
        case 2: petType += str[i]; continue;
        case 3: mood += str[i]; continue;
        case 4: c += str[i]; continue;
      }
    }
    else {
      counter++;
    }
  }
  points = atoi(c.c_str());
//    Serial.println("in extract function: " + str);
//    Serial.println(level);
//    Serial.println("in extract function: " + petType);
//    Serial.println(mood);
//    Serial.println(points);

}
