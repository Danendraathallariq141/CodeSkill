/* ----- Tata Cara Penulisan v1.0 ----- */
// #define    : huruf besar, underscore, jelas, misal #define MAX_SM_CHAR 64
// singkatan  : huruf besar, harus digabung dengan kata lain, misal SMCommand
// variabel   : camelCase/camelCaseThree, jelas, misal String SMCommand, String XLCommand, String MXCommand
// fungsi     : CamelCase/CamelCaseThree, jelas, misal SendOpenCMCommand();

/* ----- SOP program OpenCM9.04 ----- */
// PENTING! Gunakan Sublime Text 3 untuk mengedit, kemudian setelah diedit copy-paste ke OpenCM IDE baru untuk di-compile
// Kalau koding di OpenCM IDE-nya langsung nanti jadi berantakan
// Jangan menggunakan vector, C++ di OpenCM9.04 agak aneh
// Boleh menggunakan library string.h untuk strtok
// Jangan menggunakan tipe data string, gunakan array of char, library boleh karena digunakan untuk strtok

/* ----- SOP Elektrik OpenCM9.04 ----- */
// Colok Baterai minimal 11.5 V
// Pastikan Kabel Tertancap dengan baik

/* ----- SOP Mekanik OpenCM9.04 ---- */
// Pastikan titik nol servo berada di titik yang gerakannya tidak mungkin dijangkau

/* ----- Library ----- */
#include "OpenCMCommand.h"

/* ---- MAXIMUM OFFSET ----- */
#define MAX_OFFSET_JONGKOK 700
#define MAX_OFFSET_BUNGKUK 800
#define MAX_OFFSET_NGANGKANG 500
#define MAX_OFFSET_GOYANG 1200

/* ----- ID Kinematics ----- */
// Jongkok
short idJongkok[]                 = {16,17,20,21,22,23};
float multiplierJongkok[]         = {1,-1,-2.08,+2.08,-1.08,1.08};
short idJongkokKanan[]            = {16,20,22};
float multiplierJongkokKanan[]    = {1,-2.08,-1.08};
short idJongkokKiri[]             = {17,21,23};
float multiplierJongkokKiri[]     = {-1,2.08,1.08};

//Bungkuk
short idBungkuk[]                 = {16,17};
float multiplierBungkuk[]         = {1,-1};
short idBungkukKanan[]            = {16};
float multiplierBungkukKanan[]    = {1};
short idBungkukKiri[]             = {17};
float multiplierBungkukKiri[]     = {-1};

//Ngangkang
short idNgangkang[]               = {14,15};
float multiplierNgangkang[]       = {1,-1};
short idNgangkangKanan[]          = {14};
float multiplierNgangkangKanan[]  = {1};
short idNgangkangKiri[]           = {15};
float multiplierNgangkangKiri[]  = {-1};

//Goyang
short idGoyang[]                  = {18,19,24,25};
float multiplierGoyang[]          = {1,1,1,1};
short idGoyangKanan[]             = {18,24};
float multiplierGoyangKanan[]     = {1,1};
short idGoyangKiri[]              = {19,25};
float multiplierGoyangKiri[]      = {1,1};

//Angkat Kaki
short idAngkatKakiKanan[]         = {16,17,18,19,20,21,22,23,24,25};
float multiplierAngkatKakiKanan[] = {1.25, -0.5, 0.2, 0.354, -2.08, 0.832, -1.08, 0.432, 1.242, 0.354};

void Default(std::vector<int> servoParam) {
  String XLResponse = "!d";
  String MXResponse = "!d";

  boolean anyXLResponse = false;
  boolean anyMXResponse = false;

  /* ----- Jika tidak ada parameter waktu ----- */
  if(servoParam.size() == 0) {
    servoParam.push_back(200);
  }
  
  for(byte i = 0; i < servoParam.size(); i++) {
    if(i == 0) {
      /* ---- Set parameter waktu ----- */
      XLResponse += " ";
      MXResponse += " ";

      if(servoParam[i] <= 0) {
        servoParam[i] = 50;
      }

      XLResponse += servoParam[i];
      MXResponse += servoParam[i];
      i++;
    }

    /* ----- Set semua servo posisi default ----- */
    if(servoParam.size() == 1) {
      anyXLResponse = true;
      anyMXResponse = true;

      for(byte j = 0; j < sizeof(idXL)/2; j++) {
        MotorServo[idXL[j]].Set = true;
        if (idXL[j] == 5) {
          MotorServo[idXL[j]].Position = 192;
        } else if (idXL[j] == 6) {
          MotorServo[idXL[j]].Position = 816;
        } else {
          MotorServo[idXL[j]].Position = DEFAULT_POSITION_XL;
        }
      }

      for(byte j = 0; j < sizeof(idMX)/2; j++) {
        MotorServo[idMX[j]].Set = true;
        MotorServo[idMX[j]].Position = DEFAULT_POSITION_MX;
      }
    }

    if(i < servoParam.size()) {
      if(servoParam[i] <= 13) { //Servo XL
        MotorServo[servoParam[i]].Set = true;
        if (servoParam[i] == 5) {
          MotorServo[servoParam[i]].Position = 192;
        } else if (servoParam[i] == 6) {
          MotorServo[servoParam[i]].Position = 816;
        } else {
          MotorServo[servoParam[i]].Position = DEFAULT_POSITION_XL;
        }
        anyXLResponse = true;
        XLResponse += " ";
        XLResponse += servoParam[i];
      } else {
        MotorServo[servoParam[i]].Set = true;
        MotorServo[servoParam[i]].Position = DEFAULT_POSITION_MX;
        anyMXResponse = true;
        MXResponse += " ";
        MXResponse += servoParam[i];
      }
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
      Serial3.print("XLResponse : ");Serial3.println(XLResponse); //Debug
    }
  }
}

void Jongkok(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!j ";

  if(offset >= MAX_OFFSET_JONGKOK) {
    offset = MAX_OFFSET_JONGKOK;
  }

  teensyResponse += offset;
  teensyResponse += "#";

  for(byte i = 0; i < sizeof(idJongkok)/2; i++) {
    MotorServo[idJongkok[i]].Set = true;
    MotorServo[idJongkok[i]].Position = (short) ((float) DEFAULT_POSITION_MX + (float)offset*multiplierJongkok[i]);
  }

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void JongkokKanan(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!jKa ";

  if(offset >= MAX_OFFSET_JONGKOK) {
    offset = MAX_OFFSET_JONGKOK;
  }

  teensyResponse += offset;
  teensyResponse += "#";

  for(byte i = 0; i < sizeof(idJongkokKanan)/2; i++) {
    MotorServo[idJongkokKanan[i]].Set = true;
    MotorServo[idJongkokKanan[i]].Position = (short) ((float) DEFAULT_POSITION_MX + (float)offset*multiplierJongkokKanan[i]);
  }

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void JongkokKiri(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!jKi ";

  if(offset >= MAX_OFFSET_JONGKOK) {
    offset = MAX_OFFSET_JONGKOK;
  }

  teensyResponse += offset;
  teensyResponse += "#";

  for(byte i = 0; i < sizeof(idJongkokKiri)/2; i++) {
    MotorServo[idJongkokKiri[i]].Set = true;
    MotorServo[idJongkokKiri[i]].Position = (short) ((float) DEFAULT_POSITION_MX + (float)offset*multiplierJongkokKiri[i]);
  }

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void Bungkuk(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!b ";

  if(offset >= MAX_OFFSET_BUNGKUK) {
    offset = MAX_OFFSET_BUNGKUK;
  }

  teensyResponse += offset;
  teensyResponse += "#";

  for(byte i = 0; i < sizeof(idBungkuk)/2; i++) {
    MotorServo[idBungkuk[i]].Set = true;
    MotorServo[idBungkuk[i]].Position = (short) ((float) DEFAULT_POSITION_MX + (float)offset*multiplierBungkuk[i]);
  }

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void BungkukKanan(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!bKa ";

  if(offset >= MAX_OFFSET_BUNGKUK) {
    offset = MAX_OFFSET_BUNGKUK;
  }

  teensyResponse += offset;
  teensyResponse += "#";

  for(byte i = 0; i < sizeof(idBungkukKanan)/2; i++) {
    MotorServo[idBungkukKanan[i]].Set = true;
    MotorServo[idBungkukKanan[i]].Position = (short) ((float) DEFAULT_POSITION_MX + (float)offset*multiplierBungkukKanan[i]);
  }

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void BungkukKiri(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!bKi ";

  if(offset >= MAX_OFFSET_BUNGKUK) {
    offset = MAX_OFFSET_BUNGKUK;
  }

  teensyResponse += offset;
  teensyResponse += "#";

  for(byte i = 0; i < sizeof(idBungkukKiri)/2; i++) {
    MotorServo[idBungkukKiri[i]].Set = true;
    MotorServo[idBungkukKiri[i]].Position = (short) ((float) DEFAULT_POSITION_MX + (float)offset*multiplierBungkukKiri[i]);
  }

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void Ngangkang(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!ng ";

  if(offset >= MAX_OFFSET_NGANGKANG) {
    offset = MAX_OFFSET_NGANGKANG;
  }

  teensyResponse += offset;
  teensyResponse += "#";

  for(byte i = 0; i < sizeof(idNgangkang)/2; i++) {
    MotorServo[idNgangkang[i]].Set = true;
    MotorServo[idNgangkang[i]].Position = (short) ((float) DEFAULT_POSITION_MX + (float)offset*multiplierNgangkang[i]);
  }

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void NgangkangKanan(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!ngKa ";

  if(offset >= MAX_OFFSET_NGANGKANG) {
    offset = MAX_OFFSET_NGANGKANG;
  }

  teensyResponse += offset;
  teensyResponse += "#";

  for(byte i = 0; i < sizeof(idNgangkangKanan)/2; i++) {
    MotorServo[idNgangkangKanan[i]].Set = true;
    MotorServo[idNgangkangKanan[i]].Position = (short) ((float) DEFAULT_POSITION_MX + (float)offset*multiplierNgangkangKanan[i]);
  }

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void NgangkangKiri(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!ngKi ";

  if(offset >= MAX_OFFSET_NGANGKANG) {
    offset = MAX_OFFSET_NGANGKANG;
  }

  teensyResponse += offset;
  teensyResponse += "#";

  for(byte i = 0; i < sizeof(idNgangkangKiri)/2; i++) {
    MotorServo[idNgangkangKiri[i]].Set = true;
    MotorServo[idNgangkangKiri[i]].Position = (short) ((float) DEFAULT_POSITION_MX + (float)offset*multiplierNgangkangKiri[i]);
  }

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void Goyang(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!gy ";

  if(offset >= MAX_OFFSET_GOYANG) {
    offset = MAX_OFFSET_GOYANG;
  }

  teensyResponse += offset;
  teensyResponse += "#";

  for(byte i = 0; i < sizeof(idGoyang)/2; i++) {
    MotorServo[idGoyang[i]].Set = true;
    MotorServo[idGoyang[i]].Position = (short) ((float) DEFAULT_POSITION_MX + (float)offset*multiplierGoyang[i]);
  }

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void GoyangKanan(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!gyKa ";

  if(offset >= MAX_OFFSET_GOYANG) {
    offset = MAX_OFFSET_GOYANG;
  }

  teensyResponse += offset;
  teensyResponse += "#";

  for(byte i = 0; i < sizeof(idGoyangKanan)/2; i++) {
    MotorServo[idGoyangKanan[i]].Set = true;
    MotorServo[idGoyangKanan[i]].Position = (short) ((float) DEFAULT_POSITION_MX + (float)offset*multiplierGoyangKanan[i]);
  }

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void GoyangKiri(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!gyKi ";

  if(offset >= MAX_OFFSET_GOYANG) {
    offset = MAX_OFFSET_GOYANG;
  }

  teensyResponse += offset;
  teensyResponse += "#";

  for(byte i = 0; i < sizeof(idGoyangKiri)/2; i++) {
    MotorServo[idGoyangKiri[i]].Set = true;
    MotorServo[idGoyangKiri[i]].Position = (short) ((float) DEFAULT_POSITION_MX + (float)offset*multiplierGoyangKiri[i]);
  }

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void Walk(short offset) {
  currentPage = PROCESS_PAGE_WALKING;
  Go(currentPage, 1);
  
  /* ----- Fungsi untuk menggerakan semua servo dengan waktu ----- */
  short iWalk = 0;
  short timer;
  short pause;
  short repeat;
  short tempCurrentPage = currentPage; //agar currentPage tidak berubah
  
  do {
    repeat = 0; //counter
    do {
      /* ----- Menjalankan dari step 1 hingga step 7 ----- */
      for(byte i = 1; i <= Page[currentPage].PlayedStep; i++) { //Masih step 1 - 7 doang
        timer = 0;
        pause = 0;
    
        /* ----- Waktu pause sebelum menggerakan servo ----- */
        while(pause <= Page[currentPage].Step[i].Pause*8) {
          pause++;
          delay(1); //1 ms
        }
    
        /* ----- Menggerakan servo ----- */
        Serial.print("Page ");Serial.print(currentPage);Serial.print(" Go ");Serial.println(i);
        if (isFromBT) {
          Serial3.print("Page ");Serial3.print(currentPage);Serial3.print(" Go ");Serial3.println(i);
        }
        Go(currentPage, i); //Ditambah 1 karena mulainya dari 0

        /* ----- Menunggu servo bergerak ----- */
        while(timer <= Page[currentPage].Step[i].Time*8) {
          timer++;
          delay(1); //1 ms
        }
      }
      repeat++;
      
    } while(repeat <= Page[currentPage].Repeat);

    /* ----- Mengubah Step Selanjutnya ----- */
    currentPage = Page[currentPage].Next;
    iWalk++;
    if(iWalk == offset*2) {
      currentPage = 0;
    }
  } while(currentPage != 0);
  
  currentPage = tempCurrentPage;
}

void AngkatKakiKanan(short offset) {
  /* ----- Variabel Lokal ----- */
  String teensyResponse = "!aKKa ";

  teensyResponse += offset;
  teensyResponse += "#";

  Serial.println(teensyResponse); //Debug
  if (isFromBT) {
    Serial3.println(teensyResponse); //Debug
  }
  Serial4.print(teensyResponse); //MX
}

void Mirror(std::vector<int> writeParam, short *servoParam, byte servoNumParam) {
  short temp;

  if (writeParam.size() == 0) {
    //    mirror saat itu juga
    for (byte i = 0; i < servoNumParam; i++) {
      if (servoParam[i] == 1 || servoParam[i] == 2 || servoParam[i] == 0) { // Servo XL kepala
        MotorServo[servoParam[i]].Position = DEFAULT_POSITION_XL - (MotorServo[servoParam[i]].Position - DEFAULT_POSITION_XL);
      } else if (servoParam[i] == 13 || servoParam[i] == 26){ // Servo MX Paha
        MotorServo[servoParam[i]].Position = DEFAULT_POSITION_MX - (MotorServo[servoParam[i]].Position - DEFAULT_POSITION_MX);
      } else if (servoParam[i] >= 5 && servoParam[i] <= 12) { // Servo XL tangan
        temp = DEFAULT_POSITION_XL - (MotorServo[servoParam[i]].Position - DEFAULT_POSITION_XL); // id pertama
        MotorServo[servoParam[i]].Position = DEFAULT_POSITION_XL - (MotorServo[servoParam[i+1]].Position - DEFAULT_POSITION_XL);
        MotorServo[servoParam[i+1]].Position = temp;
      } else { // Servo MX
        temp = DEFAULT_POSITION_MX - (MotorServo[servoParam[i]].Position - DEFAULT_POSITION_MX); // id pertama
        MotorServo[servoParam[i]].Position = DEFAULT_POSITION_MX - (MotorServo[servoParam[i+1]].Position - DEFAULT_POSITION_MX);
        MotorServo[servoParam[i+1]].Position = temp;
      }
      i++;
    }
  } else if (writeParam.size() == 1) { // m 1
    for (byte i = 0; i <= MAXNUM_MOTORSERVO; i++) {
      MotorServo[i].Position = Page[currentPage].Step[writeParam[0]].Position[i];
    }
    
    for (byte i = 0; i < servoNumParam; i++) {
      if (servoParam[i] == 1 || servoParam[i] == 2) { // Servo XL Kepala
        MotorServo[servoParam[i]].Position = DEFAULT_POSITION_XL - (MotorServo[servoParam[i]].Position - DEFAULT_POSITION_XL);
      } else if (servoParam[i] == 13){
        MotorServo[servoParam[i]].Position = DEFAULT_POSITION_MX - (MotorServo[servoParam[i]].Position - DEFAULT_POSITION_MX);
      } else if (servoParam[i] >= 5 && servoParam[i] <= 12) { // Servo XL tangan
        temp = DEFAULT_POSITION_XL - (MotorServo[servoParam[i]].Position - DEFAULT_POSITION_XL); // id pertama
        MotorServo[servoParam[i]].Position = DEFAULT_POSITION_XL - (MotorServo[servoParam[i+1]].Position - DEFAULT_POSITION_XL);
        MotorServo[servoParam[i+1]].Position = temp;
      } else { // Servo MX
        temp = DEFAULT_POSITION_MX - (MotorServo[servoParam[i]].Position - DEFAULT_POSITION_MX); // id pertama
        MotorServo[servoParam[i]].Position = DEFAULT_POSITION_MX - (MotorServo[servoParam[i+1]].Position - DEFAULT_POSITION_MX);
        MotorServo[servoParam[i+1]].Position = temp;
      }
      i++;
    }
  }

  // Pindahkan motorServo ke Step 0, kemudian go
  for(byte i = 0; i <= MAXNUM_MOTORSERVO; i++) {
    Page[currentPage].Step[0].Position[i] = MotorServo[i].Position;
  }
  Page[currentPage].Step[0].Time = 250;
  Go(currentPage, 0);
}
