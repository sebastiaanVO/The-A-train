
// Initialiseren en declareren van enkele variabelen. 

//Digitale poorten !MOTOREN OP PWM!
int motor_r_p = 4;
int motor_l_p = 5;
int relay_l_p = 1;
int relay_r_p = 2;
int borstel_p = 3;

//Analoge poorten
int afstand_l_p = 1;
int afstand_r_p = 2;
int afstand_v_p = 3;
int licht_l_p = 4;
int licht_r_p = 5;

// Variabelen van sensorwaarden.
int afstand_l_raw;
int afstand_l_cm;
int afstand_r_raw;
int afstand_r_cm;
int afstand_v_raw;
int afstand_v_cm;

int licht_l;
int licht_r;
int verschil_afstand_l_r;
int som_afstand_l_r;

//Tijdverschil
int tijd_slagboom; 

//Snelheiden motor op schaal 0 - 255
int standaardsnelheid = 200;
int remsnelheid = 100;

//Percentage waarvoor snelheid motoren moet worden gecorrigeerd bij rechtdoor
int max_aanpassing_procent = 30; //SNELHEID NOOIT HOGER ALS 30%

//Tot deze afstand moet de snelheid dynamisch gecorrigeerd worden
int max_correctie_afstand = 4;

int motor_l;
int motor_r;


//Afstanden, schaal 1023, cm naar basis 1023 = maal 34

//afstand tussen sensor voor en muur waarop auto moet draaien
int draaiafstand = 5 * 34;
-
//afstand sensor voor en muur voor af te remmen
int remafstand = 10 * 34;

void setup()
{
  //Declaren dat de pinnen bij motoren en relay 'OUTPUT' zijn
  pinMode(motor_l_p, OUTPUT);
  pinMode(motor_r_p, OUTPUT);
  pinMode(relay_l_p, OUTPUT);
  pinMode(relay_r_p, OUTPUT);
  
  //Pin voor metaalborstel is input
  pinMode(borstel_p, INPUT);
  
  //Fancy
  Serial.begin(9600);
  
}



//Main loop
void loop()
{
	meetsensoren(); //Updaten sensorwaarden
	
	
	
	//We moeten gewoon rechtdoor
	//Afstand vanvoor voldoende groot, en afstand links,rechts voldoende klein
	if (afstand_v_cm > 20 ) && (som_afstand_l_r < 1000){
		rechtdoor(standaardsnelheid);	
		}
	 
	 	  
    	//We moeten een bocht maken
    	//Afstand vanvoor,links,rechts voldoende groot 
    	else if (som_afstand_l_r >= 1000){
			
			//Motoren op remsnelheid
			analogWrite(motor_l_p,remsnelheid)
			analogWrite(motor_r_p,remsnelheid)
			
			//Naderen tot voldoende grote afstand
			while afstand_v > draaiafstand{
			meetsensoren()
			}
			
			draaien(richting)
			
			
			//Start draai
			//2 Mogelijkheden: T-Punt of gewone draai
			if afstand_l_cm > 1000 && afstand_r_cm > 1000    
			
			//Beslissen 
	}
    	//We moeten stoppen (slagboom of einde)
    	else if (afstand_v_cm < 10) && (som_afstand_l_r < 25){
    		
    		
	}
}


//Functie voor het updaten van de sensorwaarden
int meetsensoren(){
	// Metingen afstandsensoren, schaal 0 tot 1023
  afstand_l = (analogRead(afstand_l_p));
  afstand_r = (analogRead(afstand_r_p));
  afstand_v = (analogRead(afstand_v_p));
  
  //Meting lichtsensor
  
  // Verschil tussen afstand links en rechts, positief meer naar rechts
  verschil_l_r = afstand_l - afstand_r; 
  // Som afstand links en rechts
  som_afstand_l_r = afstand_l + afstand_r;
 }
 
 
 
//Regel de relays, input 1 of 0 voor respectievelijk voor of achteruit
int relay(links,rechts){
	
	
	//Relay links
	if links == 1{
	digitalWrite(relay_l_p, HIGH)
	}
	
	else if links == 0{
		digitalWrite(relay_l_p, LOW)
	}
		
	//Relay rechts
	if rechts == 1{
		digitalWrite(relay_r_p, HIGH)
	}
	
	else if rechts == 0{
	digitalWrite(relay_r_p, LOW)
	}
}

//Functie voor te draaien, input 0 voor links, 1 voor rechts
int draaien(richting){
	
}

//Functie voor gewoon rechtdoor te rijden, en richting te corrigeren
//Input standaardsnelheid(geen correcties)
int rechtdoor(standaard){
	
	int verschil_afstand_l_r_lokaal;

	if verschil_afstand_l_r > max_correctie_afstand{
 		verschil_afstand_l_r_lokaal = max_correctie_afstand;
 	 }
 	 
	else{
		verschil_afstand_l_r_lokaal = verschil_afstand_l_r;
	}
	
	int correctie = ((standaard * max_aanpassing_procent*verschil_afstand_l_r_lokaal) / max_correctie_afstand) / 100

	//Snelheid motoren berekenen ahv afstand links/rechts
	 motor_l = standaard - correctie
	 motor_r = standaard + correctie
		  
	//Motoren daadwerkelijk aanpassen
	analogWrite(motor_l_p,motor_l);
	analogWrite(motor_r_p,motor_r);	
}

//
int stoppen{ 
	
	if afstand_v_cm > 20{
		rechtdoor(standaard)
	}
	else if (afstand_v_cm <= 20) %% (afstand_v_cm > 10){
		rechtdoor(remsnelheid)
	}
	else if afstand_v_cm <= 10{
		rechtdoor(0)
	}
}	
	
