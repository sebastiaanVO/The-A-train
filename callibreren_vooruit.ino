
const unsigned int DCP[] = {13, 2, 3, 4, 5, 6, 7, 8, 9};
const unsigned int ACP[] = {13, A0, A1, A2, A3, A4, A5};

int motor_l_p = DCP[4];
int motor_r_p = DCP[2];
int relais_l_p = DCP[1];
int relais_r_p = DCP[3];

int rijtijd_ms = 5000;
int pauzetijd_ms = 10000;

//snelheid motoren, tussen 0 en 255
int snelheid_motor_l = 100;
int snelheid_motor_r = 100;


void setup(){
  Serial.begin(9600);
  pinMode(motor_l_p, OUTPUT);
  pinMode(motor_r_p, OUTPUT);
  pinMode(relais_l_p, OUTPUT);
  pinMode(relais_r_p, OUTPUT);
  
}



void loop(){
//Relais instellen

digitalWrite(relais_l_p, HIGH);
digitalWrite(relais_r_p, HIGH);

	
//Schrijven motoren
 analogWrite(motor_l_p, snelheid_motor_l);
 analogWrite(motor_r_p, snelheid_motor_r);
 
 delay(rijtijd_ms);
 
 analogWrite(motor_l_p, 0);
 analogWrite(motor_r_p, 0);
 
 delay(pauzetijd_ms);
}
