//   _______        __          _______                 _______
//  (  ____ )      /__\        (  ___  )               / ___   )
//  | (    )|     ( \/ )       | (   ) |               \/   )  |
//  | (____)|      \  /        | |   | |     _____         /   )
//  |  _____)      /  \/\      | |   | |    (_____)      _/   /
//  | (           / /\  /      | |   | |                /   _/
//  | )          (  \/  \      | (___) |               (   (__/\
//  |/            \___/\/      (_______)               \_______/
//
//   _______  _______  _______  _______  _______        ______   _______  _______
//  (  ____ \(  ____ )(  ___  )(  ____ \(  ____ )      / ___  \ (  __   )/ ___   )
//  | (    \/| (    )|| (   ) || (    \/| (    )|      \/   \  \| (  )  |\/   )  |
//  | |      | (____)|| |   | || (__    | (____)|         ___) /| | /   |    /   )
//  | | ____ |     __)| |   | ||  __)   |  _____)        (___ ( | (/ /) |  _/   /
//  | | \_  )| (\ (   | |   | || (      | (                  ) \|   / | | /   _/
//  | (___) || ) \ \__| (___) || (____/\| )            /\___/  /|  (__) |(   (__/\
//  (_______)|/   \__/(_______)(_______/|/             \______/ (_______)\_______/
//


// Initialiseren en declareren van enkele variabelen.

// Herbenoemen van de default Arduino in- of uitgangspinnen naar de
// in- of uitgangspinnen verbonden met Connector 1 en Connector 2
// DCP staat voor Digitale Connector Pin
// ACP staat voor Analoge Connector Pin
const unsigned int DCP[] = {13, 2, 3, 4, 5, 6, 7, 8, 9};
const unsigned int ACP[] = {13, A0, A1, A2, A3, A4, A5};


//**************************************
//**********NIET AANPASSEN**************
//**************************************

//******TOEKENNEN VAN POORTEN************

//Digitale poorten
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
int licht_l_p = ACP[3];
int licht_r_p = ACP[1];



// ******Variabelen voor sensorwaarden.*************
//afstand
long totaal_afstand_l = 0;
int gemiddelde_afstand_l = 0;
long totaal_afstand_r = 0;
int gemiddelde_afstand_r = 0;
long totaal_afstand_v = 0;
int gemiddelde_afstand_v = 0;

int afstand_l_cm;
int afstand_r_cm;
int afstand_v_cm;
float verschil_afstand_l_r;
int som_afstand_l_r;

//licht
long totaal_licht_l = 0;
int gemiddelde_licht_l = 0;
long totaal_licht_r = 0;
int gemiddelde_licht_r = 0;

//knop
int totaal_drukknop = 100;

//*******Andere*************

//Meting lichtsensor, 0 links, 1 rechts
int richting_licht = 1;

//startknop
boolean gestart = false;

//Tijdstip van vorige correctie
long tijd_vorige_correctie_l = 0;
long tijd_vorige_correctie_r = 0;
long tijd_vorige_correctie_l_zwak = 0;
long tijd_vorige_correctie_r_zwak = 0;

long tijd_start_correctie = 0;


//**************************************
//**********AAN TE PASSEN**************
//**************************************

//********GEMETEN WAARDEN******************
//Afstand, lijst[cm] = meetwaarde_sensor
int afstand_l_raw_lijst[] = {1000,1000,1000,642,567,473,411,357,319,284,256,239,221,206,191,179,167,159,147,142,133,129,125,120,113,109,105,101,98,95,93,3100,3200,3300,3400,80,3600,3700,3800,3900,0};
int afstand_r_raw_lijst[] = {1000,1000,1000,640,566,465,402,353,316,280,253,233,212,200,184,173,161,152,144,136,128,120,116,112,106,101,96,93,91,88,84,3100,3200,3300,3400,72,3600,3700,3800,3900,0};
int afstand_v_raw_lijst[] = {1000,1000,1000,611,546,454,398,344,307,269,243,221,204,188,176,167,156,144,136,127,122,114,110,105,99,95,91,87,84,82,78,3100,3200,3300,3400,65,3600,3700,3800,3900,0};

//Indien sensor vlak tegen muur zit, geeft hij een zeer kleine waarde terug (alsof afstand muur zeer ver is).
//Indien sensorwaarde kleiner is als dit getal, zitten we vlak tegen muur, ipv muur ver weg.
int afstand_muur_raw = 0;



//Motoren, lijst[0] = uit, lijst[10] = max_snelheid
int motor_l_raw_snelheid[] = {0,150,180,205,252};
int motor_r_raw_snelheid[] = {0,135,160,179,255};

//Licht
//Indien lichtsterkte hoger als deze waarde, stoplicht
int licht_drempel_l = 470;
int licht_drempel_r = 470;

//Draaien, snelheid per motor en duur
//Links
int draaitijd_l = 985;
int draaisnelheid_links_motor_l = 150;
int draaisnelheid_links_motor_r = 150;

//Rechts
int draaitijd_r = 1000;
int draaisnelheid_rechts_motor_l = 150;
int draaisnelheid_rechts_motor_r = 150;


//***********************
//Aantal metingen van sensoren per uitvoeren van meetsensoren(). 5ms per meting
int aantal_metingen = 15;


//********RIJDEN*********

//Snelheiden motor op schaal 0 - 4
int standaardsnelheid = 3;

//Indien, bij rechtdoor rijden, links/rechts dichter dan deze afstand bij muur => correctie 
int correctie_afstand_achteruit = 6;
int correctie_afstand_zwak = 10;

//Indien bij rechtdoor richting moet aangepast worden => éne wiel draait gedurende deze tijd op max afstand. Milliseconde
int correctie_tijd_r = 630;
int correctie_tijd_l = 700;

int correctie_tijd_r_zwak = 300;
int correctie_tijd_l_zwak = 300;


//Minimum tijdsperiode tussen 2 correcties
int min_tijd_tussen_correctie = 1500;

int correctie_achteruit_tijd = 1500;


//******DRAAIEN*******

//afstand tussen sensor voor en muur waarop auto moet draaien
int draaiafstand_voor = 18;

//indien afstand tussen links en muur en rechts en muur (apart) groter is als deze waarde BIJ een bocht, hebben we een T-punt
int t_punt_afstand = 31;

//som afstand links en rechts, indien groter dan deze afstand zitten we in bocht
int draaiafstand_zij = 40;


//******REMMEN/STOPPEN******

//afstand muur/wagen voor beginnen af te remmen
int remafstand = 25;

//afstand muur/wagen waarbij wagen moet stoppen
int stopafstand = 9;

//snelheid bij remmen
int remsnelheid = 1;

//Tijd waarop auto moet stilstaan voor stopsignaal te geven
int stop_tijd = 8000;

//*******EINDLIED*******
byte names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};  
int tones[] = {1802, 1607, 1517, 1352, 1204, 1137, 1013, 903};
byte melody[] = "3g3e3g5C9a3a1p3f3d3f5b7g3f8e1p3e3c6f6c3p3f3e6g9f";
int count = 0;
int count2 = 0;
int count3 = 0;
int MAX_COUNT = 24;



//************************************
//******************SETUP*************
//************************************
void setup()
{
  //Declaren van INPUT en OUTPUT pinnen
  pinMode(motor_l_p, OUTPUT);
  pinMode(motor_r_p, OUTPUT);
  pinMode(relais_l_p, OUTPUT);
  pinMode(relais_r_p, OUTPUT);
  pinMode(drukknop_p, INPUT);
  pinMode(led_l_p, OUTPUT);
  pinMode(led_r_p, OUTPUT);
  pinMode(buzzer_p, OUTPUT);
  //LED's aan
  digitalWrite(led_l_p, HIGH);
  digitalWrite(led_r_p, HIGH);

  //Wachten tot startknop is ingedrukt
  while (gestart == false){
        meetsensoren();
        digitalWrite(led_l_p, HIGH);
        digitalWrite(led_r_p, HIGH);
	if (totaal_drukknop == 0){
                digitalWrite(led_l_p, LOW);
                digitalWrite(led_r_p, LOW);
                tone(buzzer_p, 1000, 350);
                delay(500);
                tone(buzzer_p, 1000, 350);
                delay(500);
                tone(buzzer_p, 1000, 350);
                delay(500);
                tone(buzzer_p, 3000, 450);
                delay(500);
                noTone(buzzer_p);
                gestart = true;
	}
        
  }

}


//******************************************
//***************MAIN LOOP******************
//******************************************
void loop()
{
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
		while ((afstand_v_cm >= draaiafstand_voor) && (som_afstand_l_r >= draaiafstand_zij)){
			meetsensoren();
			remmen_draai();
		}
	
		//Stop motoren
		rechtdoor(0, false);
			
		//Checken of Afstand links + rechts nog altijd voldoende groot is
		if (som_afstand_l_r >= draaiafstand_zij){
				
			
			//draaien
			draaien();
		
			//Update sensoren
			meetsensoren();
		
				
			while ((som_afstand_l_r >= draaiafstand_zij) && (afstand_v_cm > draaiafstand_voor) ){
				rechtdoor(2, false);
				meetsensoren();
			}
		}
	}
}

//****************************************
//**************HULPFUNCTIES**************
//****************************************

//Functie voor het updaten van de sensorwaarden
void meetsensoren(){

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

    //aantal_metingen uitvoeren op sensoren
  	for(int count = 0;count < aantal_metingen;count++){
  		totaal_afstand_l += analogRead(afstand_l_p);
  		totaal_afstand_r += analogRead(afstand_r_p);
  		totaal_afstand_v += analogRead(afstand_v_p);
  		totaal_licht_l += analogRead(licht_l_p);
		totaal_licht_r += analogRead(licht_r_p);
		totaal_drukknop += digitalRead(drukknop_p);
    	        delay(5); //wachttijd tussen elke meting
     	}

    //gemiddelde sensorwaarden berekenen
  	gemiddelde_afstand_l = totaal_afstand_l / aantal_metingen;
  	gemiddelde_afstand_r = totaal_afstand_r / aantal_metingen;
  	gemiddelde_afstand_v = totaal_afstand_v / aantal_metingen;
    
  	gemiddelde_licht_l = totaal_licht_l / aantal_metingen;
  	gemiddelde_licht_r = totaal_licht_r / aantal_metingen;
        

	//reset vorige meting
	afstand_l_cm = 0;
	afstand_r_cm = 0;
	afstand_v_cm = 0;

	// Omzetten naar cm
	for(int index = 0;index < 42; index++){
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
	
	// Verschil tussen afstand links en rechts, positief meer PLAATS links, naar rechts AFGEWEKEN
	verschil_afstand_l_r = afstand_l_cm - afstand_r_cm;

	// Som afstand links en rechts
	som_afstand_l_r = afstand_l_cm + afstand_r_cm;

	//Meting lichtsensor
    if ((gemiddelde_licht_l >= licht_drempel_l) && (gemiddelde_licht_r < licht_drempel_r)){ //LAMP LINKS
    	digitalWrite(led_l_p, LOW);
    	digitalWrite(led_r_p, HIGH);
    	richting_licht = 1;
    }
    else if ((gemiddelde_licht_r >= licht_drempel_r) && (gemiddelde_licht_l < licht_drempel_l)){ //LAMP RECHTS
    	digitalWrite(led_l_p, HIGH);
    	digitalWrite(led_r_p, LOW);
    	richting_licht = 0;

        }
    else if ((gemiddelde_licht_l >= licht_drempel_l) && (gemiddelde_licht_r >= licht_drempel_r)){ //LAMP OVERAL, beide sensoren over drempelwaarde
    	digitalWrite(led_l_p, LOW);
    	digitalWrite(led_r_p, LOW);
    	//AANPASSEN-------------------------------------------------------------------

        }

    else{	//Beide onder grenswaarde, led uit
    	digitalWrite(led_l_p, LOW);
    	digitalWrite(led_r_p, LOW);
    
    }

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

//Functie voor gewoon rechtdoor te rijden, en desgewenst richting te corrigeren
//Input standaardsnelheid, snelheidscorrectie true/false (aan/uit)
void rechtdoor(int snelheid_standaard, boolean snelheid_corrigeren){
        
	relais(1,1);
	float motor_l_standaard = motor_l_raw_snelheid[snelheid_standaard];
	float motor_r_standaard = motor_r_raw_snelheid[snelheid_standaard];

    float correctie = 0;

                  analogWrite(motor_l_p,motor_l_standaard);
	          analogWrite(motor_r_p,motor_r_standaard);
                  //Correctie richting
	

        if ((afstand_l_cm <= correctie_afstand_achteruit) && ((millis() - tijd_vorige_correctie_l) > min_tijd_tussen_correctie)){
                        relais(0,0);
                        delay(correctie_achteruit_tijd);
                        relais(1,1);
                        tijd_start_correctie = millis();
                        while (((millis() - tijd_start_correctie) < correctie_tijd_r) && (afstand_v_cm > draaiafstand_voor)){
                          analogWrite(motor_l_p, 255);
                          meetsensoren();
                        }
                        tijd_vorige_correctie_l = millis();

                    
		}

        if ((afstand_r_cm <= correctie_afstand_achteruit) && ((millis() - tijd_vorige_correctie_r) > min_tijd_tussen_correctie)){
			relais(0,0);
                        delay(correctie_achteruit_tijd);
                        relais(1,1);
                        tijd_start_correctie = millis();
                        while (((millis() - tijd_start_correctie) < correctie_tijd_l) && (afstand_v_cm > draaiafstand_voor)){
                          analogWrite(motor_r_p, 230);
                          meetsensoren();
                        }
                        tijd_vorige_correctie_r = millis();
		}

        
        if (snelheid_corrigeren == true){
		
		//Indien afstand links of rechts kleiner dan correctie afstand, desbetreffende motor op max snelheid laten draaien voor correctie_tijd

             
		
                
		

               if ((afstand_l_cm <= correctie_afstand_zwak) && ((millis() - tijd_vorige_correctie_l_zwak) > min_tijd_tussen_correctie)){

                        tijd_start_correctie = millis();
                        while (((millis() - tijd_start_correctie) < correctie_tijd_r_zwak) && (afstand_v_cm > draaiafstand_voor)){
                          analogWrite(motor_l_p, 255);
                          meetsensoren();
                        }
                        tijd_vorige_correctie_l_zwak = millis();

                    
		}
		
		

                   if ((afstand_r_cm <= correctie_afstand_zwak) && ((millis() - tijd_vorige_correctie_r_zwak) > min_tijd_tussen_correctie)){

                        tijd_start_correctie = millis();
                        while (((millis() - tijd_start_correctie) < correctie_tijd_l_zwak) && (afstand_v_cm > draaiafstand_voor)){
                          analogWrite(motor_r_p, 255);
                          meetsensoren();
                        }
                        tijd_vorige_correctie_r_zwak = millis();

                    
		}


                

	}
	//Motoren daadwerkelijk aanpassen

        analogWrite(motor_l_p,motor_l_standaard);
	analogWrite(motor_r_p,motor_r_standaard);

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
                long tijd_bij_stop = millis();
                boolean gestopt = false;
                while (afstand_v_cm <= stopafstand){
                    meetsensoren();
                    if (((millis() - tijd_bij_stop) > stop_tijd) && (gestopt == false)){
                      barbie();
                      gestopt = true;
                    }
           delay(500);       
                       
                }
                 
                        
	}
}


//Vertragen voor het draaien
void remmen_draai(){
	//Doorrijden tot bepaald afstand
	if (afstand_v_cm > 20){
		rechtdoor(standaardsnelheid, true);
	}
	//Afremmen
	else if ((afstand_v_cm <= 20) && (afstand_v_cm > draaiafstand_voor)){
		rechtdoor(2, true);
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
		richting = richting_licht;

	}
	else{ //Gewone bocht
		if (verschil_afstand_l_r > 0){ 	//links
			richting = 0;
			digitalWrite(led_l_p, HIGH);
			digitalWrite(led_r_p, LOW);
		}
		else{		//rechts
			richting = 1;
			digitalWrite(led_l_p, LOW);
			digitalWrite(led_r_p, HIGH);
		}
	}
	
	//wacht
	delay(0);

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
	//dim led
	digitalWrite(led_l_p, LOW);
	digitalWrite(led_r_p, LOW);

	//wacht
	delay(0);
}

//Eindlied
void barbie() {  
  digitalWrite(buzzer_p, LOW);     
  for (count = 0; count < MAX_COUNT; count++) {
    for (count3 = 0; count3 <= (melody[count*2] - 48) * 30; count3++) {
      for (count2=0;count2<8;count2++) {
        if (names[count2] == melody[count*2 + 1]) {       
          digitalWrite(buzzer_p,HIGH);
          delayMicroseconds(tones[count2]);
          digitalWrite(buzzer_p, LOW);
          delayMicroseconds(tones[count2]);
        } 
        if (melody[count*2 + 1] == 'p') {
          // make a pause of a certain size
          digitalWrite(buzzer_p, LOW);
          delayMicroseconds(500);
        }
      }
    }
  }
}
