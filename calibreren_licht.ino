const unsigned int DCP[] = {13, 2, 3, 4, 5, 6, 7, 8, 9};
const unsigned int ACP[] = {13, A0, A1, A2, A3, A4, A5};

int licht_l_p = ACP[5];
int licht_r_p = ACP[4];

void setup(){
  Serial.begin(9600);
}

void loop(){
    long totaal_links = 0;
    int gemiddelde_links = 0;
  
    long totaal_rechts = 0;
    int gemiddelde_rechts = 0;
  
  for(int count = 0;count < 100;count++){
    totaal_links += analogRead(licht_l_p);
    totaal_rechts += analogRead(licht_r_p);
    delay(5);
  }
  
    gemiddelde_links = totaal_links / 100;
    gemiddelde_rechts = totaal_rechts / 100;
  
    Serial.print("Links: ");
    Serial.print(gemiddelde_links);
    Serial.println();
    
    Serial.print("Rechts: ");
    Serial.print(gemiddelde_rechts);
    Serial.println();
}
