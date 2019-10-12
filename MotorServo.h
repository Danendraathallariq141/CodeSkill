#define MAXNUM_MOTORSERVO 26 //Jumlah servo maksimal
#define DEFAULT_POSITION_MX 2048  // Nilai default posisi MX
#define DEFAULT_POSITION_XL 512 // Nilai default posisi XL

/* ----- Variabel Global ----- */
// Daftar ID Servo
short idAll[]         = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};
short idXL[]          = {0,1,2,3,4,5,6,7,8,9,10,11,12};
short idMX[]          = {13,14,15,16,17,18,19,20,21,22,23,24,25,26};
short idHead[]        = {0,1,2};
short idArm[]         = {3,4,5,6,7,8,9,10,11,12};
short idRightArm[]    = {4,6,8,10,12};
short idLeftArm[]     = {3,5,7,9,11};
short idHip[]         = {13,26};
short idFoot[]        = {14,15,16,17,18,19,20,21,22,23,24,25};
short idRightFoot[]   = {14,16,18,20,22,24};
short idLeftFoot[]    = {15,17,19,21,23,25};

//Daftar ID Servo -- tambahan
short idConvertedMX[] = {3,4,13};

/* ----- MotorServo Struct ----- */
typedef struct{
  word Position;
  word Speed;
  word Volt;
  word Load;
  word Temp;
  boolean Set;
} MOTORSERVO;
