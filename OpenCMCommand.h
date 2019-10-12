#include "Page.h"
#include "MPU9250.h"

//Status Bluetooth
volatile boolean isMusicPlayed = 0; // 0 mati, 1 nyala
volatile boolean isBluetoothPaired = 0; // 0 mati, 1 nyala

/* ----- Mengirimkan TeensyResponse ke semua OpenCM ----- */
void SendTeensyResponse(String input) {
  String teensyResponse = "!" + input + "#";
  Serial5.print(teensyResponse);
  Serial4.print(teensyResponse);
  Serial.println(teensyResponse);
  if (isFromBT){
    Serial3.println(teensyResponse);
  }
}

/* ----- Fungsi untuk mengeset off setiap servo ----- */
void OffEachServo(std::vector<int> servoParam) {
  String XLResponse = "!off";
  String MXResponse = "!off";

  boolean anyXLResponse = false;
  boolean anyMXResponse = false;
  
  for(byte i = 0; i < servoParam.size(); i++) {
    if(servoParam[i] < 13) { //Servo XL
      anyXLResponse = true;
      XLResponse += " ";
      XLResponse += servoParam[i];
    } else { //Servo MX
      anyMXResponse = true;
      MXResponse += " ";
      MXResponse += servoParam[i];
    }

    MotorServo[servoParam[i]].Set = false; //Agar nilainya bisa diubah
  }
  
  XLResponse += "#";
  MXResponse += "#";
  
  if(anyXLResponse || servoParam.size() == 0) {
    Serial5.print(XLResponse); //Debug
    Serial.print("XLResponse : ");Serial.println(XLResponse); //Debug
    if (isFromBT) {
      Serial3.print("XLResponse : ");Serial3.println(XLResponse); //Debug
    }
  }

  if(anyMXResponse || servoParam.size() == 0) {
    Serial4.print(MXResponse); //Debug
    Serial.print("MXResponse : ");Serial.println(MXResponse); //Debug
    if (isFromBT) {
      Serial3.print("MXResponse : ");Serial3.println(MXResponse); //Debug
    }
  }
}

/* ---- Mengeset bagian tubuh tertentu menjadi off ----- */
void OffBodyPart(String input, short *servoParam, byte numParam) {
  String teensyResponse = "!" + input + "#";
  Serial5.print(teensyResponse);
  Serial4.print(teensyResponse);
  Serial.println(teensyResponse);
  if (isFromBT) {
    Serial3.println(teensyResponse);
  }
  
  for(byte i = 0; i < numParam; i++) {
    MotorServo[servoParam[i]].Set = false;
  }
}

/* ----- Mengirimkan go time servo1 servo2 servo3 ... servo25 ----- */
void Go(short startPage, byte step) {
  boolean isStepSet = false;
  
  for(byte i = 0; i <= MAXNUM_MOTORSERVO; i++) {
    if(Page[startPage].Step[step].Position[i] != 0) {
      isStepSet = true;
    }

    if(Page[startPage].Step[step].Position[i] == 9999 ||
       Page[startPage].Step[step].Position[i] == 8888) {
      Serial.print("Servo ");Serial.print(i);Serial.print(" Page ");Serial.print(startPage);Serial.print(" bernilai ");Serial.println(Page[startPage].Step[step].Position[i]);
      if (isFromBT) {
        Serial3.print("Servo ");Serial3.print(i);Serial3.print(" Page ");Serial3.print(startPage);Serial3.print(" bernilai ");Serial3.println(Page[startPage].Step[step].Position[i]);
      }
    }
  }

  if(isStepSet == false) {
    Serial.println("Step belum diset!");
    if (isFromBT) {
      Serial3.println("Step belum diset!");
    }
  } else if(step >= 0 && step <= MAXNUM_STEP) {
    String XLResponse;
    String MXResponse;

    XLResponse = "!g ";
    MXResponse = "!g ";

    if(Page[startPage].Step[step].Time != 0) {
      XLResponse += String(Page[startPage].Step[step].Time);
      MXResponse += String(Page[startPage].Step[step].Time);
    } else {
      XLResponse += String(50);
      MXResponse += String(50);
    }

    if(Page[startPage].Step[step].Pause != 0) {
      Serial.print("Pause ");Serial.print(step);Serial.print(" Page ");Serial.print(startPage);Serial.print(" bernilai ");Serial.println(Page[startPage].Step[step].Pause);
      if (isFromBT) {
        Serial3.print("Pause ");Serial3.print(step);Serial3.print(" Page ");Serial3.print(startPage);Serial3.print(" bernilai ");Serial3.println(Page[startPage].Step[step].Pause);
      }
    }
    
    for(byte i = 0; i <= MAXNUM_MOTORSERVO; i++) {
      if(i < 13) { //Servo XL
        XLResponse += " ";
        XLResponse += String(Page[startPage].Step[step].Position[i]);
        MotorServo[i].Set = true;
        MotorServo[i].Position = Page[startPage].Step[step].Position[i];
      } else { // Servo MX
        MXResponse += " ";
        MXResponse += String(Page[startPage].Step[step].Position[i]);
        MotorServo[i].Set = true;
        MotorServo[i].Position = Page[startPage].Step[step].Position[i];
      }
    }

    XLResponse += "#";
    MXResponse += "#";
    
    Serial5.print(XLResponse);
    Serial4.print(MXResponse);
//    Serial.print("XLResponse : ");Serial.println(XLResponse); //Debug
//    Serial.print("MXResponse : ");Serial.println(MXResponse); //Debug
  } else {
    Serial.println("Input harus di antara 1 - 20");
    if (isFromBT) {
      Serial3.println("Input harus di antara 1 - 20");
    }
  }
}

/* ----- Fungsi untuk mengeset posisi servo -----*/
// Program menerima teensyParam kemudian membagi ID-nya sesuai XL dan MX
void Set(std::vector<int> servoParam) {
  String XLResponse = "!s";
  String MXResponse = "!s";

  boolean anyXLResponse = false;
  boolean anyMXResponse = false;
  
  for(byte i = 0; i < servoParam.size()/2; i++) {
    if(servoParam[i*2] <= 13) { //Servo XL
      anyXLResponse = true;
      XLResponse += " ";
      XLResponse += servoParam[i*2]; //ID
      XLResponse += " ";
      XLResponse += servoParam[i*2+1]; //Posisi
    } else { //Servo MX
      anyMXResponse = true;
      MXResponse += " ";
      MXResponse += servoParam[i*2]; //ID
      MXResponse += " ";
      MXResponse += servoParam[i*2+1]; //Posisi
    }
    MotorServo[servoParam[i*2]].Position = servoParam[i*2 + 1];
    MotorServo[servoParam[i*2]].Set = true;
  }
  
  XLResponse += "#";
  MXResponse += "#";
  
  if(anyXLResponse) {
    Serial5.print(XLResponse); //Debug
    Serial.print("XLResponse : ");Serial.println(XLResponse); //Debug
    if (isFromBT) {
      Serial3.print("XLResponse : ");Serial3.println(XLResponse); //Debug
    }
  }

  if(anyMXResponse) {
    Serial4.print(MXResponse); //Debug
    Serial.print("MXResponse : ");Serial.println(MXResponse); //Debug
    if (isFromBT) {
      Serial3.print("MXResponse : ");Serial3.println(MXResponse); //Debug
    }
  }
}

void SetPID(std::vector<int> servoParam) {
  String XLResponse = "!sPID";
  String MXResponse = "!sPID";

  boolean anyXLResponse = false;
  boolean anyMXResponse = false;

  for(byte i = 0; i < servoParam.size()/4; i++) {
    if(servoParam[i*4] <= 13) { //Servo XL
      anyXLResponse = true;
      XLResponse += " ";
      XLResponse += servoParam[i*4]; //ID
      XLResponse += " ";
      XLResponse += servoParam[i*4+1]; //P
      XLResponse += " ";
      XLResponse += servoParam[i*4+2]; //I
      XLResponse += " ";
      XLResponse += servoParam[i*4+3]; //D
    } else { //Servo MX
      anyMXResponse = true;
      MXResponse += " ";
      MXResponse += servoParam[i*4]; //ID
      MXResponse += " ";
      MXResponse += servoParam[i*4+1]; //P
      MXResponse += " ";
      MXResponse += servoParam[i*4+2]; //I
      MXResponse += " ";
      MXResponse += servoParam[i*4+3]; //D
    }
  }
  
  XLResponse += "#";
  MXResponse += "#";

  if(anyXLResponse) {
    Serial5.print(XLResponse); //Debug
    Serial.print("XLResponse : ");Serial.println(XLResponse); //Debug
    if (isFromBT) {
      Serial3.print("XLResponse : ");Serial3.println(XLResponse); //Debug
    }
  }

  if(anyMXResponse) {
    Serial4.print(MXResponse); //Debug
    Serial.print("MXResponse : ");Serial.println(MXResponse); //Debug
    if (isFromBT) {
      Serial3.print("MXResponse : ");Serial3.println(MXResponse); //Debug
    }
  }
}

/* ----- Fungsi untuk mengeset posisi servo dengan waktu -----*/
// Program menerima teensyParam kemudian membagi ID-nya sesuai XL dan MX
void SetTime(std::vector<int> servoParam) {
  String XLResponse = "!sT ";
  String MXResponse = "!sT ";

  boolean anyXLResponse = false;
  boolean anyMXResponse = false;
  
  for(byte i = 0; i < servoParam.size(); i++) {
    if(i == 0) {
      /* ---- Set parameter waktu ----- */
      XLResponse += servoParam[i];
      MXResponse += servoParam[i];
      i++;
    }

    if(servoParam[i] < 13) { //Servo XL
      anyXLResponse = true;
      XLResponse += " ";
      XLResponse += servoParam[i];
      XLResponse += " ";
      XLResponse += servoParam[i+1];
      i++;
    } else { //Servo MX
      anyMXResponse = true;
      MXResponse += " ";
      MXResponse += servoParam[i];
      MXResponse += " ";
      MXResponse += servoParam[i+1];
      i++;
    }

    MotorServo[servoParam[i-1]].Set = false;
  }
  
  XLResponse += "#";
  MXResponse += "#";
  
  if(anyXLResponse) {
    Serial5.print(XLResponse); //Debug
    Serial.print("XLResponse : ");Serial.println(XLResponse); //Debug
    if (isFromBT) {
      Serial3.print("XLResponse : ");Serial3.println(XLResponse); //Debug
    }
  }

  if(anyMXResponse) {
    Serial4.print(MXResponse); //Debug
    Serial.print("MXResponse : ");Serial.println(MXResponse); //Debug
    if (isFromBT) {
      Serial.print("MXResponse : ");Serial.println(MXResponse); //Debug
    }
  }
}

/* ----- Fungsi untuk menggerakan semua servo dengan waktu ----- */
void Play(short startPage, boolean isMusicDetect) {
//  delay(10000);
  short timer;
  short pause;
  short repeat;
  MPU9250 IMU(Wire1,0x68);

  do {
    repeat = 0; //counter
    do {
      /* ----- Menjalankan dari step 1 hingga step terakhir ----- */
      for(byte i = 1; i <= Page[startPage].PlayedStep; i++) {
        timer = 0;
        pause = 0;
        
        /* ----- Waktu pause sebelum menggerakan servo ----- */
        while(pause <= Page[startPage].Step[i].Pause*8) {
          pause++;
          delay(1); //1 ms
        }
        
//        isMusicPlayed = true; //sementara buat bluetooth

        /* ----- Menggerakan servo ----- */
        Serial.print("Page ");Serial.print(startPage);Serial.print(" Go ");Serial.println(i);
        if (isFromBT) {
          Serial3.print("Page ");Serial3.print(startPage);Serial3.print(" Go ");Serial3.println(i);
        }
        while (isMusicPlayed == false && isMusicDetect == true) {
          //Ditahan
        }
        Go(startPage, i);

        // Masukkan bacaan sensor ke SD Card
        file = SD.open("sensor", O_WRITE | O_CREAT | O_TRUNC);
        
        // Rewind file so test data is not appended.
        // file.rewind();
        file.print(IMU.getAccelX_mss(),6);
        file.print("\t");
        file.print(IMU.getAccelY_mss(),6);
        file.print("\t");
        file.print(IMU.getAccelZ_mss(),6);
        file.print("\t");
        file.print(IMU.getGyroX_rads(),6);
        file.print("\t");
        file.print(IMU.getGyroY_rads(),6);
        file.print("\t");
        file.print(IMU.getGyroZ_rads(),6);
        file.print("\t");
        file.print(IMU.getMagX_uT(),6);
        file.print("\t");
        file.print(IMU.getMagY_uT(),6);
        file.print("\t");
        file.print(IMU.getMagZ_uT(),6);
        file.print("\t");
        file.println(IMU.getTemperature_C(),6);
        file.close();

        /* ----- Menunggu servo bergerak ----- */        
        while(timer <= Page[startPage].Step[i].Time*8) {
          timer++;
          delay(1); //1 ms
        }
      }
      repeat++;
      
    } while(repeat <= Page[startPage].Repeat);

    /* ----- Mengubah Step Selanjutnya ----- */
    startPage = Page[startPage].Next;
    
  } while(startPage != 0);
}

/* ----- Fungsi untuk menggerakan semua servo dengan waktu ----- */
void PlayPart(short startPage, boolean isMusicDetect, std::vector<int> SMParam) {
//  delay(10000);
  short timer;
  short pause;
  MPU9250 IMU(Wire1,0x68);

  if (SMParam[0]>SMParam[1]){
    Serial.println("Step awal lebih besar dari step akhir.");
  } else {
  
    if (SMParam[1]>Page[startPage].PlayedStep){
      Serial.println("Lebih besar dari step page. Akan diplay sampai step page.");
      SMParam[1] = Page[startPage].PlayedStep;
    }
    
        /* ----- Menjalankan dari step 1 hingga step terakhir ----- */
        for(byte i = SMParam[0]; i <= SMParam[1]; i++) {
          timer = 0;
          pause = 0;
          
          /* ----- Waktu pause sebelum menggerakan servo ----- */
          while(pause <= Page[startPage].Step[i].Pause*8) {
            pause++;
            delay(1); //1 ms
          }
          
  //        isMusicPlayed = true; //sementara buat bluetooth
  
          /* ----- Menggerakan servo ----- */
          Serial.print("Page ");Serial.print(startPage);Serial.print(" Go ");Serial.println(i);
          if (isFromBT) {
            Serial3.print("Page ");Serial3.print(startPage);Serial3.print(" Go ");Serial3.println(i);
          }
          while (isMusicPlayed == false && isMusicDetect == true) {
            //Ditahan
          }
          Go(startPage, i);
          
          /* ----- Menunggu servo bergerak ----- */        
          while(timer <= Page[startPage].Step[i].Time*8) {
            timer++;
            delay(1); //1 ms
          }
        }    
  }
}

void List(){
  for (byte i = 1; i <= MAXNUM_PAGE; i++) {
    if ((i % 5) == 0) {
      Serial.print(i);
      if (isFromBT) {
        Serial3.print(i);
      }
    } else {
      Serial.println(i);
      if (isFromBT) {
        Serial3.println(i);
      }
    }
  }
}

void CheckEachServo(std::vector<int> SMParam){
  std::vector<int> servoParam(2);
  if (SMParam.size() == 0) {
    for (byte i = 0; i<=MAXNUM_MOTORSERVO; i++) {
    servoParam[0]=i;
    //gerakin ke suatu sisi
    if (i<13){
      servoParam[1]=  DEFAULT_POSITION_XL - 100;
    }
    else {
      servoParam[1]=  DEFAULT_POSITION_MX - 100;
    }
    Set(servoParam);
    delay(250);

    //gerakin ke sisi lain
    if (i<13){
      servoParam[1]=  DEFAULT_POSITION_XL + 100;
    }
    else {
      servoParam[1]=  DEFAULT_POSITION_MX + 100;
    }
    Set(servoParam);
    delay(250);

    //gerakin ke tengah;
    if (i<13){
      servoParam[1]=  DEFAULT_POSITION_XL;
    }
    else {
      servoParam[1]=  DEFAULT_POSITION_MX;
    }
    Set(servoParam);
    delay(500);
    
    }
  }
// Kasus each servo (gak semua dicek)
  else {
    for(byte i = 0; i < SMParam.size(); i++) {
      servoParam[0]=SMParam[i];
      //gerakin ke suatu sisi
      if (SMParam[i]<13){
        servoParam[1]=  DEFAULT_POSITION_XL - 100;
      }
      else {
        servoParam[1]=  DEFAULT_POSITION_MX - 100;
      }
      Set(servoParam);
      delay(250);

      //gerakin ke sisi lain
      if (SMParam[i]<13){
        servoParam[1]=  DEFAULT_POSITION_XL + 100;
      }
      else {
        servoParam[1]=  DEFAULT_POSITION_MX + 100;
      }
      Set(servoParam);
      delay(250);

      //gerakin ke tengah;
      if (SMParam[i]<13){
        servoParam[1]=  DEFAULT_POSITION_XL;
      }
      else {
        servoParam[1]=  DEFAULT_POSITION_MX;
      }
      Set(servoParam);
      delay(500);
    }
  }
}

void CheckBodyPart(short *servoPrm, byte numParam) {
  std::vector<int> servoParam(2);
 
  // Kasus body part (gak semua dicek)
   for(byte i = 0; i < numParam; i++) {
     servoParam[0]=servoPrm[i];
     //gerakin ke suatu sisi
     if (servoPrm[i]<13){
       servoParam[1]=  DEFAULT_POSITION_XL - 100;
     }
     else {
       servoParam[1]=  DEFAULT_POSITION_MX - 100;
     }
     Set(servoParam);
     delay(250);
     //gerakin ke sisi lain
     if (servoPrm[i]<13){
       servoParam[1]=  DEFAULT_POSITION_XL + 100;
     }
     else {
       servoParam[1]=  DEFAULT_POSITION_MX + 100;
     }
     Set(servoParam);
     delay(250);
     //gerakin ke tengah;
     if (servoPrm[i]<13){
       servoParam[1]=  DEFAULT_POSITION_XL;
     }
     else {
       servoParam[1]=  DEFAULT_POSITION_MX;
     }
     Set(servoParam);
     delay(500);
  }
}
