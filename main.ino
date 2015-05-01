
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

int motor_r_p = DCP[2];
int motor_l_p = DCP[4];

int drukknop_p = DCP[5];
int buzzer_p = DCP[6];

int led_l_p = DCP[8];
int led_r_p = DCP[7];

//Analoge poorten
int afstand_l_p = ACP[5];
int afstand_r_p = ACP[4];
int afstand_v_p = ACP[2];
int licht_l_p = ACP[1];
int licht_r_p = ACP[3];

// Variabelen voor sensorwaarden.
long totaal_afstand_l = 0;
int gemiddelde_afstand_l = 0;
  
long totaal_afstand_r = 0;
int gemiddelde_afstand_r = 0;
    
long totaal_afstand_v = 0;
int gemiddelde_afstand_v = 0;
    	        
long totaal_licht_l = 0;
int gemiddelde_licht_l = 0;
        
long totaal_licht_r = 0;
int gemiddelde_licht_r = 0;
        
int totaal_drukknop = 100;

int afstand_l_cm;
int afstand_r_cm;
int afstand_v_cm;
float verschil_afstand_l_r;
int som_afstand_l_r;

//lijst met gemeten waarden sensorwaarden
//Afstand, lijst[cm] = meetwaarde_sensor
//sensor 2
int afstand_l_raw_lijst[] = {1000,1000,1000,643,550,478,417,355,319,292,261,241,219,200,188,178,166,156,148,142,135,210,220,230,240,250,260,270,280,290,0};
//sensor 1
int afstand_r_raw_lijst[] = {1000,1000,1000,640,564,469,408,342,315,284,258,236,220,203,189,175,163,156,144,136,130,210,220,230,240,250,260,270,280,290,0};
//sensor 3
int afstand_v_raw_lijst[] = {1000,1000,1000,592,483,418,363,320,285,257,233,210,196,184,169,157,150,139,130,124,120,210,220,230,240,250,260,270,280,290,0};
//Licht, lijst[0] = volledig donker, lijst[10] = superfel licht, lijst[3] = gewoon licht
int licht_l_raw_lijst[] = {0,1,2,3,4,5,6,7,8,9,10};
int licht_r_raw_lijst[] = {0,1,2,3,4,5,6,7,8,9,10};

//********RIJDEN*********
//Lijst met verband snelheden tussen motoren links en rechts, lijst[0] = uit, lijst[10] = max_snelheid
int motor_l_raw_snelheid[] = {0,150,180,200,250};
int motor_r_raw_snelheid[] = {0,135,160,190,255};

//Uiteindelijke snelheid motor, schaal 0-255
int motor_l;
int motor_r;

//Snelheiden motor op schaal 0 - 4
int standaardsnelheid = 3;
int remsnelheid = 10;

//Percentage waarvoor snelheid motoren moet worden gecorrigeerd bij rechtdoor
float max_aanpassing_procent = 10; //SNELHEID NOOIT HOGER ALS 30%

//Tot deze afstand moet de snelheid dynamisch gecorrigeerd worden
float max_correctie_afstand = 6;


//******DRAAIEN*******
//afstand tussen sensor voor en muur waarop auto moet draaien
int draaiafstand_voor = 15;

//indien afstand tussen links en muur en rechts en muur (apart) groter is als deze waarde bij een bocht, hebben we een T-punt
int t_punt_afstand = 40;

//som afstand links en rechts, indien groter dan deze afstand zitten we in bocht
int draaiafstand_zij = 50;

//Draaien, snelheid per motor en duur
//Links
int draaitijd_l = 930;
int draaisnelheid_links_motor_l = 150;
int draaisnelheid_links_motor_r = 150;

//Rechts
int draaitijd_r = 950;
int draaisnelheid_rechts_motor_l = 150;
int draaisnelheid_rechts_motor_r = 150;

//******REMMEN/STOPPEN******
//afstand muur/wagen voor beginnen af te remmen
int remafstand = 15;

//afstand muur/wagen waarbij wagen moet stoppen
int stopafstand = 5;

//******ANDERE******
//Tijdverschillen
int tijd_slagboom;
int laatste_update = 0;
boolean gestart = false;


//******************SETUP*************
void setup()
{
  //Declaren dat de pinnen bij motoren en relay 'OUTPUT' zijn
  pinMode(motor_l_p, OUTPUT);
  pinMode(motor_r_p, OUTPUT);
  pinMode(relais_l_p, OUTPUT);
  pinMode(relais_r_p, OUTPUT);
  pinMode(drukknop_p, INPUT);
  
  //Communicatie met PC
  Serial.begin(9600);

}

//***************MAIN LOOP******************
void loop()
{
  Serial.println(totaal_drukknop);
meetsensoren(); //Updaten sensorwaarden

if (not gestart){
	if (totaal_drukknop == 0){
		tone(buzzer_p, 1000, 3000);
                gestart = true;
	}
	
}
else{
	meetsensoren(); //Updaten sensorwaarden
	relais(1,1); //Beide motoren vooruit

	//3 OPTIES: 1 Rechtdoor, 2 Stoppen/afremmen , 3 Bocht


	//1
	//We moeten gewoon rechtdoor
	//Afstand vanvoor voldoende groot, en afstand links,rechts voldoende klein
	if ((afstand_v_cm > remafstand) && (som_afstand_l_r < draaiafstand_zij)){
		rechtdoor(standaardsnelheid, true);
		}


	//2
    	//We moeten stoppen (slagboom of einde)
    	//Afstand vanvoor,links,rechts voldoende klein
    	else if ((afstand_v_cm <= remafstand) && (som_afstand_l_r < draaiafstand_zij)){
		stoppen_obstakel();

	}

	//3
    	//We moeten een bocht maken
    	//Afstand links + rechts voldoende groot
    	else if (som_afstand_l_r >= draaiafstand_zij){


		//Naderen tot draaiafstand_voor
		while (afstand_v_cm > draaiafstand_voor){
			meetsensoren();
			remmen_draai();
		}

		//Stop motoren
		rechtdoor(0, false);

                delay(1000);

		//draaien
		draaien();

		//Update sensoren
		meetsensoren();

		//Rijd verder tot terug op recht stuk
		//!!!!!! NOG MAX TIJD TOEVOEGEN
		while (som_afstand_l_r > draaiafstand_zij){
			rechtdoor(2, false);
			meetsensoren();
		}
	}
}
}


//**************HULPFUNCTIES**************

//Functie voor het updaten van de sensorwaarden
void meetsensoren(){

	int tijdverschil = millis() - laatste_update;
	laatste_update = millis();

    totaal_afstand_l = 0;
    gemiddelde_afstand_l = 0;
  
    totaal_afstand_r = 0;
    gemiddelde_afstand_r = 0;
    
    totaal_afstand_v = 0;
    gemiddelde_afstand_v = 0;
    	        
    totaal_licht_l = 0;
    gemiddelde_licht_l = 0;
        
    totaal_licht_r = 0;
    gemiddelde_licht_r = 0;
        
    totaal_drukknop = 0;
        
  	for(int count = 0;count < 100;count++){
  		totaal_afstand_l += analogRead(afstand_l_p);
  		totaal_afstand_r += analogRead(afstand_r_p);
  		totaal_afstand_v += analogRead(afstand_v_p);
  		totaal_licht_l += analogRead(licht_l_p);
		totaal_licht_r += analogRead(licht_r_p);
		totaal_drukknop += digitalRead(drukknop_p);
    	delay(5);
     	}
  
  		gemiddelde_afstand_l = totaal_afstand_l / 100;
  		gemiddelde_afstand_r = totaal_afstand_r / 100;
  		gemiddelde_afstand_v = totaal_afstand_v / 100;
  		
  		gemiddelde_licht_l = totaal_licht_l / 100;
  		gemiddelde_licht_r = totaal_licht_r / 100;
  		
	//reset vorige meting
	afstand_l_cm = 0;
	afstand_r_cm = 0;
	afstand_v_cm = 0;
	
	// Omzetten naar cm
	for(int index = 0;index < 31; index++){
		//sensor links
		if (afstand_l_cm == 0 && afstand_l_raw_lijst[index] <= gemiddelde_afstand_l){
			afstand_l_cm = index;
		}
		//sensor rechts
		if (afstand_r_cm == 0 && afstand_r_raw_lijst[index] <= gemiddelde_afstand_r){
			afstand_r_cm = index;
		}
		//sensor boven
		if (afstand_v_cm == 0 && afstand_v_raw_lijst[index] <= gemiddelde_afstand_v){
			afstand_v_cm = index;
		}
		
	}


	// Verschil tussen afstand links en rechts, positief meer plaats links
	verschil_afstand_l_r = afstand_l_cm - afstand_r_cm;

	// Som afstand links en rechts
	som_afstand_l_r = afstand_l_cm + afstand_r_cm;

	//Meting lichtsensor


 }

//Regel de relais, input 1 of 0 voor respectievelijk voor of achteruit
void relais(int links, int rechts){

	//Relais links
	if (links == 0){
		digitalWrite(relais_l_p, HIGH);
	}

	else if (links == 1){
		digitalWrite(relais_l_p, LOW);
	}

	//Relais rechts
	if (rechts == 0){
		digitalWrite(relais_r_p, HIGH);
	}

	else if (rechts == 1){
		digitalWrite(relais_r_p, LOW);
	}
}

//Functie voor gewoon rechtdoor te rijden, en richting te corrigeren
//Input standaardsnelheid, snelheidscorrectie true/false
void rechtdoor(int snelheid_standaard, boolean snelheid_corrigeren){
        relais(1,1);
	float motor_l_standaard = motor_l_raw_snelheid[snelheid_standaard];
	float motor_r_standaard = motor_r_raw_snelheid[snelheid_standaard];					 
	
        float correctie = 0;
        
	if (snelheid_corrigeren == true){
		//lokale variabele voor verschil afstand links/rechts
		//Wordt gelijk aan verschil_afstand_l_r, zolang dit kleiner is als max_correctie_afstand
		int verschil_afstand_l_r_lokaal;
	
	
		//Correctiewaarde moet dynamisch aangepast worden tot max waarde
		//Indien verschil_afstand groter dan deze waarde => Correctie maximaal
		if (abs(verschil_afstand_l_r) > max_correctie_afstand){
                       if (verschil_afstand_l_r > 0){
                 
			verschil_afstand_l_r_lokaal = max_correctie_afstand;
                       }
                       else{
                         verschil_afstand_l_r_lokaal = max_correctie_afstand * -1;
                       }
		}
	
	
		else{
			verschil_afstand_l_r_lokaal = verschil_afstand_l_r;
		}
		//Correctiewaarde berekenen
		correctie = ((max_aanpassing_procent * verschil_afstand_l_r_lokaal) / max_correctie_afstand) / 100;
	}
	else{
		correctie = 0;
	}
	//!!!!!!!!NOG CHECKEN INT/FLOAT BIJ CORRIGEREN, WORDT NIET TE GROF AFGEROND?
	//Uiteindelijke waarden motoren
	motor_l = motor_l_standaard * (1.0 + correctie);
	motor_r = motor_r_standaard * (1.0 - correctie);
        Serial.println(correctie);

	//Motoren daadwerkelijk aanpassen
	analogWrite(motor_l_p,motor_l);
	analogWrite(motor_r_p,motor_r);
}


//Vertragen en stoppen voor een slagboom/het einde
void stoppen_obstakel(){
	//Doorrijden tot bepaald afstand
	if (afstand_v_cm > remafstand){
		rechtdoor(standaardsnelheid, true);
	}
	//Afremmen
	else if ((afstand_v_cm <= remafstand) && (afstand_v_cm > stopafstand)){
		rechtdoor(remsnelheid, true);
	}
	//Stoppen
	else if (afstand_v_cm <= stopafstand){
		rechtdoor(0, true);
	}
}


//Vertragen voor het draaien
void remmen_draai(){
	//Doorrijden tot bepaald afstand
	if (afstand_v_cm > 20){
		rechtdoor(standaardsnelheid, false);
	}
	//Afremmen
	else if ((afstand_v_cm <= 20) && (afstand_v_cm > draaiafstand_voor)){
		rechtdoor(2, false);
	}
	//Stoppen
	else if (afstand_v_cm <= draaiafstand_voor){
		rechtdoor(0, false);
	}
}

//Draaien
void draaien(){

	//draairichting bepalen

	//0 voor links, 1 voor rechts
	int richting;

	//Bepalen T-punt of gewone bocht
	if (afstand_l_cm > t_punt_afstand && afstand_r_cm > t_punt_afstand){ //T-punt
		
		//richting bepalen ahv lichtsensor
		richting = t_punt_richting();
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
	//links
	if (richting == 0){
		relais(0,1);
		analogWrite(motor_l_p, draaisnelheid_links_motor_l);
		analogWrite(motor_r_p, draaisnelheid_links_motor_r);
		delay(draaitijd_l);
	}
	//rechts
	else{
		relais(1,0);
		analogWrite(motor_l_p, draaisnelheid_rechts_motor_l);
		analogWrite(motor_r_p, draaisnelheid_rechts_motor_r);
		delay(draaitijd_r);		
	}
	//stop motoren
	rechtdoor(0, false);	
}

//Bepaalt richting bij t-punt, 0 voor links, 1 voor rechts
int t_punt_richting(){
	return 0;
}

void buzzer(int tijd_per_toon, int aantal_tonen){
	int tijd = tijd_per_toon /2;
	for(int teller = 0; teller < aantal_tonen;teller++){
		digitalWrite(buzzer_p, HIGH);
		delay(tijd);
		digitalWrite(buzzer_p, LOW);
		delay(tijd);
	}
}
