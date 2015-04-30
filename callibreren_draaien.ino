
const unsigned int DCP[] = {13, 2, 3, 4, 5, 6, 7, 8, 9};
const unsigned int ACP[] = {13, A0, A1, A2, A3, A4, A5};

int motor_l_p = DCP[1];
int motor_r_p = DCP[2];
int relais_l_p = DCP[3];
int relais_r_p = DCP[4];

//richting, 0 links, 1 rechts
int richting = 0;

int draaitijd_ms = 200;

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
//Relais links
if (richting == 1){
	digitalWrite(relais_l_p, HIGH);
	digitalWrite(relais_r_p, LOW);
}

else if (richting == 0){
	digitalWrite(relais_l_p, LOW);
	digitalWrite(relais_r_p, HIGH);
}
	
//Schrijven motoren
 analogWrite(motor_l_p, snelheid_motor_l);
 analogWrite(motor_r_p, snelheid_motor_r);
 
 delay(draaitijd_ms);
 
 analogWrite(motor_l_p, 0);
 analogWrite(motor_r_p, 0);
 
 delay(10000);
}
