
// Initialiseren en declareren van enkele variabelen. 

// Herbenoemen van de default Arduino in- of uitgangspinnen naar de 
// in- of uitgangspinnen verbonden met Connector 1 en Connector 2
// DCP staat voor Digitale Connector Pin
// ACP staat voor Analoge Connector Pin
const unsigned int DCP[] = {13, 2, 3, 4, 5, 6, 7, 8, 9};
const unsigned int ACP[] = {13, A0, A1, A2, A3, A4, A5};


//Digitale poorten !MOTOREN OP PWM!

int relais_l_p = DCP[1];
int relais_r_p = DCP[3];

int borstel_l_p = DCP[5];
int borstel_r_p = DCP[6];

int motor_r_p = DCP[2];
int motor_l_p = DCP[4];

//Analoge poorten  !!!!!!!!!!!!!!!!!!!! KUNNEN GEEN INTEGERS ZIJN
int afstand_l_p = ACP[1];
int afstand_r_p = ACP[2];
int afstand_v_p = ACP[3];
int licht_l_p = ACP[4];
int licht_r_p = ACP[5];

// Variabelen van sensorwaarden.
int afstand_l_raw;
int afstand_l_cm;
int afstand_r_raw;
int afstand_r_cm;
int afstand_v_raw;
int afstand_v_cm;
int verschil_afstand_l_r;
int som_afstand_l_r;

int licht_l_raw;
int licht_l_gem;
int licht_r_raw;
int licht_r_gem;

int borstel_l;
int borstel_r;

//Tijdverschillen
int tijd_slagboom;
int laatste_update = 0;

//Snelheiden motor op schaal 0 - 255
int standaardsnelheid = 200;
int remsnelheid = 100;

//Percentage waarvoor snelheid motoren moet worden gecorrigeerd bij rechtdoor
int max_aanpassing_procent = 30; //SNELHEID NOOIT HOGER ALS 30%

//Tot deze afstand moet de snelheid dynamisch gecorrigeerd worden
int max_correctie_afstand = 4;

//Snelheid motor, in basis 255
int motor_l;
int motor_r;

//afstand tussen sensor voor en muur waarop auto moet draaien
int draaiafstand = 5;

//afstand muur/wagen voor beginnen af te remmen
int remafstand = 15;

//afstand muur/wagen waarbij wagen moet stoppen
int stopafstand = 5;

//Setup
void setup()
{
  //Declaren dat de pinnen bij motoren en relay 'OUTPUT' zijn
  pinMode(motor_l_p, OUTPUT);
  pinMode(motor_r_p, OUTPUT);
  pinMode(relais_l_p, OUTPUT);
  pinMode(relais_r_p, OUTPUT);
  
  //Pin voor metaalborstel is input
  pinMode(borstel_l_p, INPUT);
  pinMode(borstel_r_p, INPUT);
  
  //Fancy
  Serial.begin(9600);
  
}



//Main loop
void loop()
{
	meetsensoren(); //Updaten sensorwaarden
	relais(1,1); //Beide motoren vooruit
	
	//3 OPTIES: 1 Rechtdoor, 2 Stoppen/afremmen , 3 Bocht
	
	
	//1
	//We moeten gewoon rechtdoor
	//Afstand vanvoor voldoende groot, en afstand links,rechts voldoende klein
	if ((afstand_v_cm > remafstand) && (som_afstand_l_r < 25)){
		rechtdoor(standaardsnelheid);	
		}
	 
	
	//2
    //We moeten stoppen (slagboom of einde)
    //Afstand vanvoor,links,rechts voldoende klein
    else if ((afstand_v_cm <= remafstand) && (som_afstand_l_r < 25)){
		stoppen_obstakel();
    		
	}
	
	//3
    //We moeten een bocht maken
    //Afstand links + rechts voldoende groot 
    else if (som_afstand_l_r >= 25){
			
			
		//Naderen tot draaiafstand
		while (afstand_v_cm > draaiafstand){
		meetsensoren();
		remmen_draai();
		}
		
		//Stop motoren
		rechtdoor(0);
		
		//draaien
		draaien();
		
		//Update sensoren
		meetsensoren();
		
		//Rijd verder tot terug op recht stuk
		while (afstand_v_cm > draaiafstand){
		meetsensoren();
		}	   
	}		
	
}



//HULPFUNCTIES


//Functie voor het updaten van de sensorwaarden
void meetsensoren(){
	
	int tijdverschil = millis() - laatste_update;
	laatste_update = millis();
	
	// Metingen afstandsensoren, schaal 0 tot 1023
	afstand_l_raw = (analogRead(afstand_l_p));
	afstand_r_raw = (analogRead(afstand_r_p));
	afstand_v_raw = (analogRead(afstand_v_p));
  
	// Omzetten naar cm
	
	
	// Verschil tussen afstand links en rechts, positief meer plaats links
	int verschil_l_r = afstand_l_cm - afstand_r_cm; 
	
	// Som afstand links en rechts
	int som_afstand_l_r = afstand_l_cm + afstand_r_cm;
  
	//Meting lichtsensor
  
	//Meting borstels
	borstel_l = digitalRead(borstel_l_p);
	borstel_r = digitalRead(borstel_r_p);
	
	
 }
 
 
 
//Regel de relais, input 1 of 0 voor respectievelijk voor of achteruit
void relais(int links, int rechts){
	
	
	//Relais links
	if (links == 1){
	digitalWrite(relais_l_p, HIGH);
	}
	
	else if (links == 0){
		digitalWrite(relais_l_p, LOW);
	}
		
	//Relais rechts
	if (rechts == 1){
		digitalWrite(relais_r_p, HIGH);
	}
	
	else if (rechts == 0){
	digitalWrite(relais_r_p, LOW);
	}
}



//Functie voor gewoon rechtdoor te rijden, en richting te corrigeren
//Input standaardsnelheid
void rechtdoor(int standaard){
	
	//lokale variabele voor verschil afstand links/rechts
	int verschil_afstand_l_r_lokaal;
	
	//Correctiewaarde moet dynamisch aangepast worden tot max waarde
	//Indien verschil_afstand groter dan deze waarde =>aanpassen
	if (verschil_afstand_l_r > max_correctie_afstand){
 		verschil_afstand_l_r_lokaal = max_correctie_afstand;
 	 }
 	 
 	 
	else{
		verschil_afstand_l_r_lokaal = verschil_afstand_l_r;
	}
	//Correctiewaarde berekenen
	int correctie = ((standaard * max_aanpassing_procent * verschil_afstand_l_r_lokaal) / max_correctie_afstand) / 100;

	//Uiteindelijke waarden motoren
	 motor_l = standaard - correctie;
	 motor_r = standaard + correctie;
		  
	//Motoren daadwerkelijk aanpassen
	analogWrite(motor_l_p,motor_l);
	analogWrite(motor_r_p,motor_r);	
}


//Vertragen en stoppen voor een slagboom/het einde
void stoppen_obstakel(){
	//Doorrijden tot bepaald afstand
	if (afstand_v_cm > remafstand){
		rechtdoor(standaardsnelheid);
	}
	//Afremmen
	else if ((afstand_v_cm <= remafstand) && (afstand_v_cm > stopafstand)){
		rechtdoor(remsnelheid);
	}
	//Stoppen
	else if (afstand_v_cm <= stopafstand){
		rechtdoor(0);
	}
} 


//Draaien
void draaien(){
	
	//draairichting bepalen
	
	//0 voor links, 1 voor rechts
	int richting;
	
	//Bepalen T-punt of gewone bocht
	if (afstand_l_cm > 25 && afstand_r_cm > 25){ //T-punt
	
		if (licht_l_gem - licht_r_gem > 0){ //CHECKEN AAN WELKE KANT VAN DRAAI LICHT STAAT
			richting = 0;
		}
		else{
			richting = 1;
		}
	}
	
	else{ //Gewone bocht
		if (verschil_afstand_l_r > 0){
			richting = 0;
		}
		else{
			richting = 1;
		} 
	}
	
	//draaien zelf
	
	
	
}

//Vertragen voor het draaien
void remmen_draai(){
	//Doorrijden tot bepaald afstand
	if (afstand_v_cm > 20){
		rechtdoor(standaardsnelheid);
	}
	//Afremmen
	else if ((afstand_v_cm <= 20) && (afstand_v_cm > draaiafstand)){
		rechtdoor(remsnelheid);
	}
	//Stoppen
	else if (afstand_v_cm <= draaiafstand){
		rechtdoor(0);
	}
}
