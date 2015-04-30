void setup(){
  Serial.begin(9600);
}

void loop(){
long totaal_links = 0;
int gemiddelde_links = 0;

long totaal_rechts = 0;
int gemiddelde_rechts = 0;

for(int count = 0;count < 100;count++)
{totaal_links += analogRead(0);
totaal_rechts += analogRead(1);
delay(5);
}

gemiddelde_links = totaal_links / 100;
gemiddelde_rechts = totaal_rechts / 100;


Serial.print(gemiddelde_links);
Serial.print("                ");
Serial.print(gemiddelde_rechts);
Serial.println();

}
