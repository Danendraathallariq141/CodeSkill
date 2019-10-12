#include <stdarg.h>
#include <vector>

/* ----- PAGE ----- */
#define FIRST_PAGE_WALKING
#define LAST_PAGE_WALKING
#define PROCESS_PAGE_WALKING 100

// ----- Kamus Servo.h -----//
#include "MotorServo.h"
MOTORSERVO MotorServo[MAXNUM_MOTORSERVO+1];

// ---- SD Card ----- //
#include <SPI.h>
#include <SdFat.h>
#define CS_PIN SS
#define ROW_DIM 32
#define COL_DIM 20
SdFatSdioEX SD;
File file;
#define errorHalt(msg) {Serial.println(F(msg)); SysCall::halt();}

#define MAXNUM_PAGE   75 //Jumlah page maksimal
#define MAXNUM_STEP   20   //Jumlah step maksimal per page

typedef struct {
  short Position[MAXNUM_MOTORSERVO+1];
  byte Pause;
  byte Time;
} STEP;
  
typedef struct {
  STEP Step[MAXNUM_STEP+1];
  String Name;
  byte Repeat; //Play Count
  byte PlayedStep; //Page Step
  byte Next;
} PAGE;

// ---- Variabel Page.h ----- //
PAGE Page[MAXNUM_PAGE+1]; // bisa aja nambah, hati-hati bikin ERROR
short currentPage = 1;
boolean adaNext;

void PrintLeadingZero(short val, short num_zero){
  for (byte i = 1; i < num_zero; i++) {
    if (val < pow(10,i))
      Serial.print(0);
  }
  Serial.print(val);
}//

size_t readField(File* file, char* str, size_t size, const char* delim) {
  char ch;
  size_t n = 0;
  while ((n + 1) < size && file->read(&ch, 1) == 1) {
    // Delete CR.
    if (ch == '\r') {
      continue;
    }
    str[n++] = ch;
    if (strchr(delim, ch)) {
        break;
    }
  }
  str[n] = '\0';
  return n;
}

void FillPage(PAGE &Page, short filename, SdFatSdioEX SD){
  file = SD.open((String) filename, FILE_READ);
  // Rewind file so test data is not appended.
  file.rewind();

  size_t n;      // Length of returned field with delimiter.
  char str[50];  // Must hold longest field with delimiter and zero byte.
  char *ptr;     // Test for valid field.
  for (byte i = 0; i <= ROW_DIM; i++) { 
    for (byte j = 1; j <= COL_DIM; j++) {
      n = readField(&file, str, sizeof(str), ",\n");
      if (i <= MAXNUM_MOTORSERVO) {
        Page.Step[j].Position[i] = strtol(str, &ptr, 10);
      } else if (i == MAXNUM_MOTORSERVO+1) {
        Page.Step[j].Pause = strtol(str, &ptr, 10);
      } else if (i == MAXNUM_MOTORSERVO+2) {
        Page.Step[j].Time = strtol(str, &ptr, 10);
      } else if (i == MAXNUM_MOTORSERVO+3) {
        char *token;
        char *temp = new char[sizeof(str) + 1];
        strcpy(temp, str);
        token = strtok (temp,",\n");
        Page.Name = token;
      } else if (i == MAXNUM_MOTORSERVO+4) {
        Page.Repeat = strtol(str, &ptr, 10);
      } else if (i == MAXNUM_MOTORSERVO+5) {
        Page.PlayedStep = strtol(str, &ptr, 10);
      } else if (i == MAXNUM_MOTORSERVO+6) {
        Page.Next = strtol(str, &ptr, 10);
      }
    }
  }
  file.close();
}

void ReloadSD() {
  for (short filename = 1; filename <= MAXNUM_PAGE; filename++) {
    FillPage(Page[filename], filename, SD);
    delay(25);
  }
  for(byte i = 0; i <= MAXNUM_MOTORSERVO; i++) {
    MotorServo[i].Set = false;
  }
  Serial.println("Page telah diloading dari SDCard!"); //Untuk serial monitor Teensy
  Serial3.println("Page telah diloading dari SDCard!"); //Untuk serial monitor Arduino (BT)
}

void SavePage(PAGE Page[]) {
  Serial.println("Beda Pandangan...");
  for (short i = 1; i <= MAXNUM_PAGE; i++) {
    file = SD.open((String) i, O_WRITE | O_CREAT | O_TRUNC);
    
    // Rewind file so test data is not appended.
    file.rewind();
    String line;
    
    for (byte k = 0; k <= ROW_DIM; k++) {
      line = "";
      for (byte j = 1; j < COL_DIM; j++) {
        if (k <= MAXNUM_MOTORSERVO) {
          line = line + (String) Page[i].Step[j].Position[k] + ","; 
        } else if (k == MAXNUM_MOTORSERVO+1) {
          line = line + (String) Page[i].Step[j].Pause + ","; 
        } else if (k == MAXNUM_MOTORSERVO+2) {
          line = line + (String) Page[i].Step[j].Time + ","; 
        } else if (k == MAXNUM_MOTORSERVO+3) {
          line = line + (String) Page[i].Name + ",";
        } else if (k == MAXNUM_MOTORSERVO+4) {
          line = line + (String) Page[i].Repeat + ",";
        } else if (k == MAXNUM_MOTORSERVO+5) {
          line = line + (String) Page[i].PlayedStep + ",";
        } else if (k == MAXNUM_MOTORSERVO+6) {
          line = line + (String) Page[i].Next + ",";
        }
      }

      if (k <= MAXNUM_MOTORSERVO) {
        line = line + (String) Page[i].Step[COL_DIM].Position[k] + "\r\n";
      } else if (k == MAXNUM_MOTORSERVO+1) {
        line = line + (String) Page[i].Step[COL_DIM].Pause + "\r\n";
      } else if (k == MAXNUM_MOTORSERVO+2) {
        line = line + (String) Page[i].Step[COL_DIM].Time + "\r\n";
      } else if (k == MAXNUM_MOTORSERVO+3) {
        line = line + (String) Page[i].Name + "\r\n";
      } else if (k == MAXNUM_MOTORSERVO+4) {
        line = line + (String) Page[i].Repeat + "\r\n";
      } else if (k == MAXNUM_MOTORSERVO+5) {
        line = line + (String) Page[i].PlayedStep + "\r\n";
      } else if (k == MAXNUM_MOTORSERVO+6) {
        line = line + (String) Page[i].Next + "\r\n";
      }
      
//        Serial.print(line);
      file.print(line);
    }
    file.close();
  }
  Serial.println("Semua prueeeeeengpreungprengprengpreng berhasil disimpan."); 
}

void DrawPage(short numPage) {
  byte i, j;
  currentPage = numPage;

  for(byte i = 0; i <= MAXNUM_MOTORSERVO; i++) {
    Serial.print("ID "); Serial.print(i); Serial.print((char) 9);

    // ---- Menampilkan posisi servo ---- //
    for (j = 0; j <= MAXNUM_STEP; j++) {
      if (j == 0) {
        Serial.print("[");
        if (MotorServo[i].Position == 8888) {
          Serial.print("????");
        } else if (MotorServo[i].Position == 9999) {
          Serial.print("----");
        } else {
          Page[currentPage].Step[j].Position[i] = MotorServo[i].Position;
          PrintLeadingZero(Page[currentPage].Step[j].Position[i],4);
        }
        Serial.print("]");
      } else {
        PrintLeadingZero(Page[currentPage].Step[j].Position[i],4);
      }
      
      if (Page[currentPage].PlayedStep == j) {
        Serial.print("|");
      } else {
        Serial.print(" ");
      }
    }

    // ---- Menampilkan Volt dan Load ---- ///
    PrintLeadingZero(MotorServo[i].Volt,4); Serial.print(" ");
    PrintLeadingZero(MotorServo[i].Load,4); Serial.print(" ");
    PrintLeadingZero(MotorServo[i].Temp,4); Serial.print(" ");
    
    //----- Menampilkan keterangan Page -----//
    Serial.print(" ");
    if (i == 1) {
      Serial.print(Page[currentPage].Name);
    } else if (i == 2) {
      Serial.print("Page Number : ");
       PrintLeadingZero(currentPage,3); // Tes
    } else if (i == 3) {
      Serial.print("Repeat : ");
      PrintLeadingZero(Page[currentPage].Repeat,3);
    } else if (i == 4) {
      Serial.print("Page Step : ");
      PrintLeadingZero(Page[currentPage].PlayedStep,3);
    } else if (i == 5) {
      Serial.print("Link to Next : ");
      PrintLeadingZero(Page[currentPage].Next,3);
    }
    Serial.println();
  }

  // ---- Menampilkan nilai Pause ---- //
  Serial.print("Pause "); Serial.print((char) 9);
  for (j = 0; j <= MAXNUM_STEP; j++) {
    if (j == 0){
      Serial.print("[ ");
      PrintLeadingZero(Page[currentPage].Step[j].Pause,3);
      Serial.print("]");
    } else {
      Serial.print(" ");
      PrintLeadingZero(Page[currentPage].Step[j].Pause,3);
    }

    if (Page[currentPage].PlayedStep == j) {
      Serial.print("|");
    } else {
      Serial.print(" ");
    }
  }
  Serial.println("---- ---- ----");

  // ---- Menampilkan nilai Time ---- //
  Serial.print("Time "); Serial.print((char) 9);
  for (j = 0; j <= MAXNUM_STEP; j++) {
    if (j == 0) {
      Serial.print("[ ");
      PrintLeadingZero(Page[currentPage].Step[j].Time,3);
      Serial.print("]");
    } else {
      Serial.print(" ");
      PrintLeadingZero(Page[currentPage].Step[j].Time,3);
    }

    if (Page[currentPage].PlayedStep == j) {
      Serial.print("|");
    } else {
      Serial.print(" ");
    }
  }
  Serial.println("---- ---- ----");

  // ---- Menampilkan tulisan bagian bawah ---- //
  Serial.print((char) 9);
  for (j = 0; j <= MAXNUM_STEP; j++) {
    if (j == 0) {
      Serial.print(" ");
    }

    if (j < 10){
      Serial.print("STP"); Serial.print(j);
    }
    else{
      Serial.print("ST"); Serial.print(j);
    }
  
    if (j == 0) {
      Serial.print(" ");
    }
    
    Serial.print(" ");
  }
  
  Serial.print("Volt");Serial.print(" ");
  Serial.print("Load");Serial.print(" ");
  Serial.print("Temp");Serial.print(" ");
  Serial.println();
}

void ChangeTime(std::vector<int> servoParam) {
  for(byte i = 0; i < (servoParam.size()/2)*2; i++) {
    Page[currentPage].Step[servoParam[i]].Time = servoParam[i+1];
    Serial.print("Time step ");Serial.print(servoParam[i]);Serial.print(" telah diubah menjadi ");Serial.println(servoParam[i+1]);
    i++;
  }
}

void ChangePause(std::vector<int> servoParam) {
  for(byte i = 0; i < (servoParam.size()/2)*2; i++) {
    Page[currentPage].Step[servoParam[i]].Pause = servoParam[i+1];
    Serial.print("Pause step ");Serial.print(servoParam[i]);Serial.print(" telah diubah menjadi ");Serial.println(servoParam[i+1]);
    i++;
  }
}

void ChangeName(String newName) {
  char *token;
  char *temp = new char[newName.length() + 1];
  strcpy(temp, newName.c_str());
  Page[currentPage].Name = temp;
}

void ChangeRepeat(byte newRepeat){
  Page[currentPage].Repeat = newRepeat;
}

void ChangeNext(byte newNext){
  Page[currentPage].Next = newNext;
}

void ChangePlayedStep(byte newPlayedStep){
  Page[currentPage].PlayedStep = newPlayedStep;
}

void DeleteStep(std::vector<int> deleteParam){
  /* ---- Mengeset semua nilai pada deleteParam menjadi 0 ----- */
  for(byte j = 0; j < deleteParam.size(); j++) {
    for(byte i = 0; i <= MAXNUM_MOTORSERVO; i++) {
        Page[currentPage].Step[deleteParam[j]].Position[i] = 0;
    }
  }
}

void Write(std::vector<int> writeParam, short *servoParam, byte servoNumParam) {
  if (writeParam.size() == 1) { // w 1
    if(writeParam[0] >= 1 && writeParam[0] <= MAXNUM_STEP) {
      for (byte i = 0; i < servoNumParam; i++) {
        Page[currentPage].Step[writeParam[0]].Position[servoParam[i]] = MotorServo[servoParam[i]].Position;
      }
    } else {
      Serial.print("Step harus bernilai 1 - 20");
    }
  } else if (writeParam.size() > 1) { // w 2 3 4
    for(byte j = 1; j < writeParam.size(); j++) {
      if(writeParam[j] >= 1 && writeParam[j] <= MAXNUM_STEP) {
        for (byte i = 0; i < servoNumParam; i++) {
          Page[currentPage].Step[writeParam[j]].Position[servoParam[i]] = Page[currentPage].Step[writeParam[0]].Position[servoParam[i]];
        }
      } else {
          Serial.println("Step harus bernilai 1 - 20");
        }
    }
  }
}

void WriteBedaPage(std::vector<int> writeParam, short *servoParam, byte servoNumParam) {
  // wBP 1           : menuliskan seluruh step page sekarang ke page 1 (copy keseluruhan page)
  // wBP 1 2 3 4     : menuliskan step 1 page 2 ke step 3 page 4 (berlaku parameter kelipatan 4) --> isi vektor writeParam[] = {1, 2, 3, 4}
  if (writeParam.size() == 1) {   
    // Kasus page bernilai di luar range
    if ((writeParam[0]<1)||(writeParam[0]>MAXNUM_PAGE)) {
      //Serial.println("Step harus bernilai 1 - 20");
      Serial.println("Page harus bernilai 1 - 75");
      if (isFromBT){
        Serial3.println("Page harus bernilai 1 - 75");
      }
    } else {
      for (byte i = 1; i<=MAXNUM_STEP; i++) {
        for (byte j = 0; j < servoNumParam; j++) {
          Page[writeParam[0]].Step[i].Position[servoParam[j]] = Page[currentPage].Step[i].Position[servoParam[j]];
        }
      }
    }
  } else {
    byte n = (writeParam.size()/4);
    for (byte i = 0; i<n; i++) {
      // Kasus step dan page bernilai di luar range
      if ((writeParam[i*4]<1)||(writeParam[i*4+1]<1)||(writeParam[i*4+2]<1)||(writeParam[i*4+3]<1)||(writeParam[i*4]>MAXNUM_STEP)||(writeParam[i*4+2]>MAXNUM_STEP)||(writeParam[i*4+1]>MAXNUM_PAGE)||(writeParam[i*4+3]>MAXNUM_PAGE)) {
        Serial.println("Step harus bernilai 1 - 20");
        Serial.println("Page harus bernilai 1 - 75");
        if (isFromBT){
          Serial3.println("Step harus bernilai 1 - 20");
          Serial3.println("Page harus bernilai 1 - 75");
        }
      } else {
        for (byte j = 0; j < servoNumParam; j++) {
          Page[writeParam[i*4+3]].Step[writeParam[i*4+2]].Position[servoParam[j]] = Page[writeParam[i*4+1]].Step[writeParam[i*4]].Position[servoParam[j]];
        }
      }
    } 
  }  
}

//new added
void ShiftStep(std::vector<int> shiftParam, short *servoParam, byte servoNumParam) {
  if (shiftParam.size() == 1) { // shift 1
    if(shiftParam[0] >= 1 && shiftParam[0] <= MAXNUM_STEP) {
      for (byte j = MAXNUM_STEP; j> shiftParam[0]; j-- ){
        for (byte i = 0; i < servoNumParam; i++) {
          Page[currentPage].Step[j].Position[servoParam[i]] = Page[currentPage].Step[j-shiftParam[0]].Position[servoParam[i]];
        }
      }
      for (byte j = shiftParam[0]; j>= 1; j-- ){
        for (byte i = 0; i < servoNumParam; i++) {
          Page[currentPage].Step[j].Position[servoParam[i]] = 0;
        }
      }  
    } else {
      Serial.println("Step harus bernilai 1 - 20");
      if (isFromBT){
        Serial3.println("Step harus bernilai 1 - 20");
      }
    }
  } else if (shiftParam.size() == 2) { // shift 2 3 (start step, how many step shifted)
    if ( ((shiftParam[0] + shiftParam[1]) < 1) || ((Page[currentPage].PlayedStep + shiftParam[1]) > MAXNUM_STEP) ) {
      Serial.println("Kelebihan gesernya");
      if (isFromBT){
        Serial3.println("Kelebihan gesernya");
      }
      return;
    }
    if (shiftParam[1] >= 1 && shiftParam[1] <= MAXNUM_STEP) {
      for (byte j = MAXNUM_STEP; j >= (shiftParam[0] + shiftParam[1]); j-- ){
        for (byte i = 0; i < servoNumParam; i++) {
          Page[currentPage].Step[j].Position[servoParam[i]] = Page[currentPage].Step[j-shiftParam[1]].Position[servoParam[i]];
        }
      }
      for (byte j = (shiftParam[0] + shiftParam[1] - 1); j>= shiftParam[0]; j-- ){
        for (byte i = 0; i < servoNumParam; i++) {
          Page[currentPage].Step[j].Position[servoParam[i]] = 0;
        }
      } 
    } else if (shiftParam[1] < 0) {
      for (byte j = (shiftParam[0] + shiftParam[1]); j <= (MAXNUM_STEP + shiftParam[1]); j++ ){
        for (byte i = 0; i < servoNumParam; i++) {
          Page[currentPage].Step[j].Position[servoParam[i]] = Page[currentPage].Step[j-shiftParam[1]].Position[servoParam[i]];
        }
      }
      for (byte j = (MAXNUM_STEP + shiftParam[1] + 1); j <= MAXNUM_STEP; j++ ){
        for (byte i = 0; i < servoNumParam; i++) {
          Page[currentPage].Step[j].Position[servoParam[i]] = 0;
        }
      }
    }
  }
}

void SplitStep(std::vector<int> splitParam, short *servoParam, byte servoNumParam){
  std::vector<int> shiftParam(2);
  shiftParam[0] = splitParam[0]+1; //1 step setelah yang displit
  //Serial.println(shiftParam[0]); //Debug
  shiftParam[1] = splitParam[1]; //jumlah split
  //Serial.println(shiftParam[1]); //Debug
  ShiftStep(shiftParam, idAll, sizeof(idAll)/2);
  for (byte j = 1; j<=splitParam[1]; j++) {
    for (byte i = 0; i < servoNumParam; i++) {
      int dx = (Page[currentPage].Step[shiftParam[0]+shiftParam[1]].Position[servoParam[i]] - Page[currentPage].Step[splitParam[0]].Position[servoParam[i]]) / (splitParam[1]+1);
      Page[currentPage].Step[splitParam[0]+j].Position[servoParam[i]] = Page[currentPage].Step[splitParam[0]].Position[servoParam[i]] + (dx * j);
    }
  }
}

void CopyPage(std::vector<int> servoParam) {
  for (byte k = 0; k <= ROW_DIM; k++) { 
      for (byte j = 1; j <= COL_DIM; j++) { 
      if (k <= MAXNUM_MOTORSERVO) { 
        Page[servoParam[1]].Step[j].Position[k] = Page[servoParam[0]].Step[j].Position[k];
      } else if (k == MAXNUM_MOTORSERVO+1) { 
        Page[servoParam[1]].Step[j].Pause = Page[servoParam[0]].Step[j].Pause;  
      } else if (k == MAXNUM_MOTORSERVO+2) { 
        Page[servoParam[1]].Step[j].Time = Page[servoParam[0]].Step[j].Time; 
      } else if (k == MAXNUM_MOTORSERVO+3) { 
        Page[servoParam[1]].Name = Page[servoParam[0]].Name;
      } else if (k == MAXNUM_MOTORSERVO+4) { 
        Page[servoParam[1]].Repeat = Page[servoParam[0]].Repeat; 
      } else if (k == MAXNUM_MOTORSERVO+5) { 
        Page[servoParam[1]].PlayedStep = Page[servoParam[0]].PlayedStep; 
      } else if (k == MAXNUM_MOTORSERVO+6) { 
        Page[servoParam[1]].Next = Page[servoParam[0]].Next;
      }
    } 
  } 
}

void Help() {
  Serial.println("----- Fungsi dasar action editor -----");
  Serial.println("Gunakan ! di awal dan # di akhir");
  Serial.println("get/page     : membuka halaman sekarang");
  Serial.println("get/page 100 : membuka halaman 100");
  Serial.println("next/n 101   : mengganti halaman selanjutnya menjadi 101");
  Serial.println("repeat/r 3   : mengganti repeat halaman sekarang menjadi 3");
  Serial.println("step 7       : mengganti step maksimal menjadi 7");
  Serial.println("del 5        : menghapus step 5");
  Serial.println("go/g 7       : pergi ke step 7");
  Serial.println("");
  Serial.println("play         : memainkan page sekarang sesuai sampai step page");
  Serial.println("play 3 5     : memainkan page sekarang dari step 3 ke step 5");
  Serial.println("Pastikan step dan time sudah terdefinisi");
  Serial.println("");
  Serial.println("on        : membuat semua servo kaku");
  Serial.println("on 10 11  : membuat kaku servo 10 11 saja");
  Serial.println("off       : membuat semua servo lemas");
  Serial.println("off 12 13 : membuat lemas servo 12 13 saja");
  Serial.println("");
  Serial.println("oh   : off head");
  Serial.println("ora  : off right arm");
  Serial.println("ola  : off left arm");
  Serial.println("ohip : off hip");
  Serial.println("of   : off foot");
  Serial.println("orf  : off right foot");
  Serial.println("olf  : off left foot");
  Serial.println("");
  Serial.println("set 1 100 3 400           : mengeset servo 1 ke posisi 100, servo 3 ke posisi 400");
  Serial.println("setTime/sT 50 2 100 3 300 : mengeset servo 2 ke posisi 100, servo 3 ke posisi 300, dengan waktu 50*8 ms");
  Serial.println("");
  Serial.println("setPID 1 8 9 10           : mengeset PID servo 1 berturut-turut --> P=8, I=9, D=10");
  Serial.println("");
  Serial.println("pause 1 50 2 250 : mengubah pause step 1 menjadi 50 dan step 2 menjadi 250");
  Serial.println("time 1 40 3 400  : mengubah time step 1 menjadi 40 dan step 3 menjadi 400");
  Serial.println("");
  Serial.println("w 1         : menuliskan step 0 ke step 1");
  Serial.println("w 1 2       : menuliskan step 1 ke step 2");
  Serial.println("w 3 4 5 6 7 : menuliskan step 3 ke step 4 5 6 7");
  Serial.println("");
  Serial.println("wh 1 2 3    : menuliskan parameter head dari step 1 ke step 2 3");
  Serial.println("wra, wla, whip, wf, wrf, wlf dst menuliskan parameter sesuai fungsi off");
  Serial.println("");
  Serial.println("wBP 1           : menuliskan seluruh step page sekarang ke page 1 (copy keseluruhan page)");
  Serial.println("wBP 1 2 3 4     : menuliskan step 1 page 2 ke step 3 page 4 (berlaku parameter kelipatan 4)");
  Serial.println("wBPra, wBPla, wBPhip, wBPf, wBPrf, wBPlf dst menuliskan parameter sesuai fungsi off");
  Serial.println("");
  Serial.println("cek             : cek semua servo dari posisi default dengan offset 100");
  Serial.println("cek 5 9         : cek servo 5 dan 9 dari posisi default dengan offset 100");
  Serial.println("ch, ca, cla, cra, dst mengecek semua servo sesuai fungsi off");
  Serial.println("");
  Serial.println("shift 1         : menggeser seluruh step sebanyak 1 step");
  Serial.println("shift 2 1       : menggeser mulai dari step ke-2 sebanyak 1 step ke kanan");
  Serial.println("shift 2 -1       : menggeser mulai dari step ke-2 sebanyak 1 step ke kiri");
  Serial.println("");
  Serial.println("split 1 2       : menambahkan 2 step setelah step 1");
  Serial.println("");
  Serial.println("----- Fungsi dasar Kinematics -----");
  Serial.println("default/def 50 : Mengeset semua servo ke posisi default dalam 50*8 ms");
  Serial.println("");
  Serial.println("jongkok 100 : menekuk kedua kaki posisi jongkok 100 poin");
  Serial.println("jKa 200     : jongkok kanan 200 poin");
  Serial.println("jKi 300     : jongkok kiri 300 poin");
  Serial.println("");
  Serial.println("bungkuk 100 : posisi bungkuk/ruku 100 poin");
  Serial.println("bKa 200     : posisi bungkuk/ruku kanan 200 poin");
  Serial.println("bKi 300     : posisi bungkuk/ruku kiri 300 poin");
  Serial.println("");
  Serial.println("ngangkang/ng 100 : membuka selangkangan 100 poin");
  Serial.println("ngKa 200         : membuka selangkangan kanan 200 poin");
  Serial.println("ngKi 300         : membuka selangkangan kiri 300 poin");
  Serial.println("");
  Serial.println("goyang/gy 100  : memiringkan/menggoyangkan badan robot 200 poin");
  Serial.println("gyKa gyKi -250 : memiringkan/menggoyangkan kaki kanan/kiri saja -250 poin");
  Serial.println("");
  Serial.println("walk 10 : berjalan 10 langkah");
}

int convertMXtoXL (int MXValue) {
  //Nilai hard-coded, penjelasan akan ditambahkan di kemudian hari
  MXValue -= 341.3;
  
  if (MXValue < 0){
    return (0);
  } else if (MXValue > (3754.7 - 341.3)){
    return (1024);
  } else {
    return ((int)((((float)MXValue) / (3754.7 - 341.3)) * 1024));
  }
}

void ConvertMX(){
  for (int i = 1; i < MAXNUM_PAGE; i++) {
    for (int j = 1; j < MAXNUM_STEP; j++) {
      for (int k = 1; k < sizeof(idConvertedMX); k++) {
        Page[i].Step[j].Position[k] = convertMXtoXL(Page[i].Step[j].Position[k]);
      }
    }
  }
}

void ConvertArm(byte pageNum){
  for (byte i =1; i<MAXNUM_STEP; i++) {
    Page[pageNum].Step[i].Position[5] -= 300; 
  }
}
