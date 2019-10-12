// List fungsi feedback yang diperlukan pada bluetooth:
// help, savepage, drawpage, set, settime, go, list, play
// offXXX, ngangkangXXX, dll

// Yang udah : HelpBT, SavePageBT, DrawPageBT, SetBT, SetTimeBT, GoBT, PlayBT, OffXXXBT, MirrorBT, WriteBT UDAH SEMUA

// Catatan : jangan lupa ganti nama di program utama
// Kalau mau cepet : copy, ganti nama function(+BT), ganti Serial jadi Serial3
// Jangan lupa kalau di dalem fungsi ada fungsi yang disebutkan diatas, kasi BT juga

void HelpBT() {
  Serial3.println("----- Fungsi dasar action editor -----");
  Serial3.println("Gunakan ! di awal dan # di akhir");
  Serial3.println("get/page     : membuka halaman sekarang");
  Serial3.println("get/page 100 : membuka halaman 100");
  Serial3.println("next/n 101   : mengganti halaman selanjutnya menjadi 101");
  Serial3.println("repeat/r 3   : mengganti repeat halaman sekarang menjadi 3");
  Serial3.println("step 7       : mengganti step maksimal menjadi 7");
  Serial3.println("del 5        : menghapus step 5");
  Serial3.println("go/g 7       : pergi ke step 7");
  Serial3.println("");
  Serial3.println("play         : memainkan page sekarang sesuai sampai step page");
  Serial3.println("play 3 5     : memainkan page sekarang dari step 3 ke step 5");
  Serial3.println("Pastikan step dan time sudah terdefinisi");
  Serial3.println("");
  Serial3.println("on        : membuat semua servo kaku");
  Serial3.println("on 10 11  : membuat kaku servo 10 11 saja");
  Serial3.println("off       : membuat semua servo lemas");
  Serial3.println("off 12 13 : membuat lemas servo 12 13 saja");
  Serial3.println("");
  Serial3.println("oh   : off head");
  Serial3.println("ora  : off right arm");
  Serial3.println("ola  : off left arm");
  Serial3.println("ohip : off hip");
  Serial3.println("of   : off foot");
  Serial3.println("orf  : off right foot");
  Serial3.println("olf  : off left foot");
  Serial3.println("");
  Serial3.println("set 1 100 3 400           : mengeset servo 1 ke posisi 100, servo 3 ke posisi 400");
  Serial3.println("setTime/sT 50 2 100 3 300 : mengeset servo 2 ke posisi 100, servo 3 ke posisi 300, dengan waktu 50*8 ms");
  Serial3.println("");
  Serial3.println("setPID 1 8 9 10           : mengeset PID servo 1 berturut-turut --> P=8, I=9, D=10");
  Serial3.println("");
  Serial3.println("pause 1 50 2 250 : mengubah pause step 1 menjadi 50 dan step 2 menjadi 250");
  Serial3.println("time 1 40 3 400  : mengubah time step 1 menjadi 40 dan step 3 menjadi 400");
  Serial3.println("");
  Serial3.println("w 1         : menuliskan step 0 ke step 1");
  Serial3.println("w 1 2       : menuliskan step 1 ke step 2");
  Serial3.println("w 3 4 5 6 7 : menuliskan step 3 ke step 4 5 6 7");
  Serial3.println("");
  Serial3.println("wh 1 2 3    : menuliskan parameter head dari step 1 ke step 2 3");
  Serial3.println("wra, wla, whip, wf, wrf, wlf dst menuliskan parameter sesuai fungsi off");
  Serial3.println("");
  Serial3.println("wBP 1           : menuliskan seluruh step page sekarang ke page 1 (copy keseluruhan page)");
  Serial3.println("wBP 1 2 3 4     : menuliskan step 1 page 2 ke step 3 page 4 (berlaku parameter kelipatan 4)");
  Serial3.println("wBPra, wBPla, wBPhip, wBPf, wBPrf, wBPlf dst menuliskan parameter sesuai fungsi off");
  Serial3.println("");
  Serial3.println("cek             : cek semua servo dari posisi default dengan offset 100");
  Serial3.println("cek 5 9         : cek servo 5 dan 9 dari posisi default dengan offset 100");
  Serial3.println("ch, ca, cla, cra, dst mengecek semua servo sesuai fungsi off");
  Serial3.println("");
  Serial3.println("shift 1         : menggeser seluruh step sebanyak 1 step");
  Serial3.println("shift 2 1       : menggeser mulai dari step ke-2 sebanyak 1 step ke kanan");
  Serial3.println("shift 2 -1       : menggeser mulai dari step ke-2 sebanyak 1 step ke kiri");
  Serial3.println("");
  Serial3.println("split 1 2       : menambahkan 2 step setelah step 1");
  Serial3.println("");
  Serial3.println("----- Fungsi dasar Kinematics -----");
  Serial3.println("default/def 50 : Mengeset semua servo ke posisi default dalam 50*8 ms");
  Serial3.println("");
  Serial3.println("jongkok 100 : menekuk kedua kaki posisi jongkok 100 poin");
  Serial3.println("jKa 200     : jongkok kanan 200 poin");
  Serial3.println("jKi 300     : jongkok kiri 300 poin");
  Serial3.println("");
  Serial3.println("bungkuk 100 : posisi bungkuk/ruku 100 poin");
  Serial3.println("bKa 200     : posisi bungkuk/ruku kanan 200 poin");
  Serial3.println("bKi 300     : posisi bungkuk/ruku kiri 300 poin");
  Serial3.println("");
  Serial3.println("ngangkang/ng 100 : membuka selangkangan 100 poin");
  Serial3.println("ngKa 200         : membuka selangkangan kanan 200 poin");
  Serial3.println("ngKi 300         : membuka selangkangan kiri 300 poin");
  Serial3.println("");
  Serial3.println("goyang/gy 100  : memiringkan/menggoyangkan badan robot 200 poin");
  Serial3.println("gyKa gyKi -250 : memiringkan/menggoyangkan kaki kanan/kiri saja -250 poin");
  Serial3.println("");
  Serial3.println("walk 10 : berjalan 10 langkah");
}

void SavePageBT(PAGE Page[]) {
  Serial3.println("Beda Pandangan...");
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
      
//        Serial3.print(line);
      file.print(line);
    }
    file.close();
  }
  Serial3.println("Semua prueeeeeengpreungprengprengpreng berhasil disimpan."); 
}

void PrintLeadingZeroBT(short val, short num_zero){
  for (byte i = 1; i < num_zero; i++) {
    if (val < pow(10,i))
      Serial3.print(0);
  }
  Serial3.print(val);
}

void DrawPageBT(short numPage) {
  byte i, j;
  currentPage = numPage;

  for(byte i = 0; i <= MAXNUM_MOTORSERVO; i++) {
    Serial3.print("ID "); Serial3.print(i); Serial3.print((char) 9);

    // ---- Menampilkan posisi servo ---- //
    for (j = 0; j <= MAXNUM_STEP; j++) {
      if (j == 0) {
        Serial3.print("[");
        if (MotorServo[i].Position == 8888) {
          Serial3.print("????");
        } else if (MotorServo[i].Position == 9999) {
          Serial3.print("----");
        } else {
          Page[currentPage].Step[j].Position[i] = MotorServo[i].Position;
          PrintLeadingZeroBT(Page[currentPage].Step[j].Position[i],4);
        }
        Serial3.print("]");
      } else {
        PrintLeadingZeroBT(Page[currentPage].Step[j].Position[i],4);
      }
      
      if (Page[currentPage].PlayedStep == j) {
        Serial3.print("|");
      } else {
        Serial3.print(" ");
      }
    }

    // ---- Menampilkan Volt dan Load ---- ///
    PrintLeadingZeroBT(MotorServo[i].Volt,4); Serial3.print(" ");
    PrintLeadingZeroBT(MotorServo[i].Load,4); Serial3.print(" ");
    PrintLeadingZeroBT(MotorServo[i].Temp,4); Serial3.print(" ");
    
    //----- Menampilkan keterangan Page -----//
    Serial3.print(" ");
    if (i == 1) {
      Serial3.print(Page[currentPage].Name);
    } else if (i == 2) {
      Serial3.print("Page Number : ");
       PrintLeadingZeroBT(currentPage,3); // Tes
    } else if (i == 3) {
      Serial3.print("Repeat : ");
      PrintLeadingZeroBT(Page[currentPage].Repeat,3);
    } else if (i == 4) {
      Serial3.print("Page Step : ");
      PrintLeadingZeroBT(Page[currentPage].PlayedStep,3);
    } else if (i == 5) {
      Serial3.print("Link to Next : ");
      PrintLeadingZeroBT(Page[currentPage].Next,3);
    }
    Serial3.println();
  }

  // ---- Menampilkan nilai Pause ---- //
  Serial3.print("Pause "); Serial3.print((char) 9);
  for (j = 0; j <= MAXNUM_STEP; j++) {
    if (j == 0){
      Serial3.print("[ ");
      PrintLeadingZeroBT(Page[currentPage].Step[j].Pause,3);
      Serial3.print("]");
    } else {
      Serial3.print(" ");
      PrintLeadingZeroBT(Page[currentPage].Step[j].Pause,3);
    }

    if (Page[currentPage].PlayedStep == j) {
      Serial3.print("|");
    } else {
      Serial3.print(" ");
    }
  }
  Serial3.println("---- ---- ----");

  // ---- Menampilkan nilai Time ---- //
  Serial3.print("Time "); Serial3.print((char) 9);
  for (j = 0; j <= MAXNUM_STEP; j++) {
    if (j == 0) {
      Serial3.print("[ ");
      PrintLeadingZeroBT(Page[currentPage].Step[j].Time,3);
      Serial3.print("]");
    } else {
      Serial3.print(" ");
      PrintLeadingZeroBT(Page[currentPage].Step[j].Time,3);
    }

    if (Page[currentPage].PlayedStep == j) {
      Serial3.print("|");
    } else {
      Serial3.print(" ");
    }
  }
  Serial3.println("---- ---- ----");

  // ---- Menampilkan tulisan bagian bawah ---- //
  Serial3.print((char) 9);
  for (j = 0; j <= MAXNUM_STEP; j++) {
    if (j == 0) {
      Serial3.print(" ");
    }

    if (j < 10){
      Serial3.print("STP"); Serial3.print(j);
    }
    else{
      Serial3.print("ST"); Serial3.print(j);
    }
  
    if (j == 0) {
      Serial3.print(" ");
    }
    
    Serial3.print(" ");
  }
  
  Serial3.print("Volt");Serial3.print(" ");
  Serial3.print("Load");Serial3.print(" ");
  Serial3.print("Temp");Serial3.print(" ");
  Serial3.println();
}

void ChangeTimeBT(std::vector<int> servoParam) {
  for(byte i = 0; i < (servoParam.size()/2)*2; i++) {
    Page[currentPage].Step[servoParam[i]].Time = servoParam[i+1];
    Serial3.print("Time step ");Serial3.print(servoParam[i]);Serial3.print(" telah diubah menjadi ");Serial3.println(servoParam[i+1]);
    i++;
  }
}

void ChangePauseBT(std::vector<int> servoParam) {
  for(byte i = 0; i < (servoParam.size()/2)*2; i++) {
    Page[currentPage].Step[servoParam[i]].Pause = servoParam[i+1];
    Serial3.print("Pause step ");Serial3.print(servoParam[i]);Serial3.print(" telah diubah menjadi ");Serial3.println(servoParam[i+1]);
    i++;
  }
}

void WriteBT(std::vector<int> writeParam, short *servoParam, byte servoNumParam) {
  if (writeParam.size() == 1) { // w 1
    if(writeParam[0] >= 1 && writeParam[0] <= MAXNUM_STEP) {
      for (byte i = 0; i < servoNumParam; i++) {
        Page[currentPage].Step[writeParam[0]].Position[servoParam[i]] = MotorServo[servoParam[i]].Position;
      }
    } else {
      Serial3.print("Step harus bernilai 1 - 20");
    }
  } else if (writeParam.size() > 1) { // w 2 3 4
    for(byte j = 1; j < writeParam.size(); j++) {
      if(writeParam[j] >= 1 && writeParam[j] <= MAXNUM_STEP) {
        for (byte i = 0; i < servoNumParam; i++) {
          Page[currentPage].Step[writeParam[j]].Position[servoParam[i]] = Page[currentPage].Step[writeParam[0]].Position[servoParam[i]];
        }
      } else {
          Serial3.print("Step harus bernilai 1 - 20");
        }
    }
  }
}

void DrawGUI(short numPage) {
  byte i, j;
  currentPage = numPage;
  Serial3.flush();
  Serial3.println('!');
  //delay(10);

  for(byte i = 0; i <= MAXNUM_MOTORSERVO; i++) {
    // ---- Menampilkan posisi servo ---- //
    for (j = 0; j <= 7; j++) {
      if (j == 0) {
        Serial3.println(MotorServo[i].Position);
        //delay(10); 
      } 
      else {
        Serial3.println(Page[currentPage].Step[j].Position[i]);
        //delay(10);
      }
    }
  }

  // ---- Menampilkan nilai Pause ---- //
  for (j = 0; j <= 7; j++) {
    Serial3.println(Page[currentPage].Step[j].Pause);
    //delay(10);
  }


  // ---- Menampilkan nilai Time ---- //
  for (j = 0; j <= 7; j++) {
    Serial3.println(Page[currentPage].Step[j].Time);
    //delay(10); 
  }
  
  for (j = 0; j <= 3; j++) {
    if (j == 0){
      Serial3.println(currentPage);
    }
    else if (j== 1){
      Serial3.println(Page[currentPage].Repeat);
    }
    else if (j == 2){
      Serial3.println(Page[currentPage].PlayedStep);
    }
    else {
      Serial3.println(Page[currentPage].Next);
    }
  }

  Serial3.println('#');

}
