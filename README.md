# **Projekt candela**
 
I det här projektet samlar jag in data från en fototransistor för ljusmätning, http-protokoll och sensor-kod är skriven i Arduino IDE. Data skickas till Azure hub som dirigerar data vidare till Power BI för enklare visualisering.
Jag använder mig av en ESP 32 WROOM, en fototransistor och en resistor på 10 kOhm.
 
![ESP32 wroom & phototransistor on breadboard](/Bilder/ESP32Breadboard.jpg)
 
⦁	Http-protokoll 
I http-protokollet definierar jag mina Azure resurser samt min WiFi uppkoppling. Här talar jag om vilken URL/Adress jag vill komma till.
Varje meddelande jag skickar har med information i json-format som innehåller mitt insamlade data från sensorn.
 
⦁	Azure 
I Azure har jag satt upp en prenumeration, en resursgrupp och en IOT Hubb.
Prenumerationen skapar en uppkoppling. 
Hubben kontrollerar att min sak har rätt att publicera. Fångar upp mitt meddelande och kan dirigera meddelandet vidare till Azure Stream Analytics. 
 
![Azure Slide](/Bilder/Slide.jpg)
 
⦁	Visualisering av data 
Det är från Stream Analytics som Power BI läser in meddelanden och kan bearbeta data till den graf jag valt.
Grafen visar hur ljusstyrkan i rummet ändras med tiden. Högsta mätvärdet representerar den ljusaste tiden och lägsta när det är mörkast.

![Azure Main](/Bilder/Azure_Main.png)
![Temp Hubb](/Bilder/TempHubb_OverView.png)
Temporary IOT hubb I Azure som är skapad för att ta emot meddelanden från min microkontroller och distrubiera vidare inom Azure. 
![StreamAnalytics](/Bilder/StreamAnalytics.png)
Stream Analytics verktyg skapat för att bearbeta data som kommer in i TempHubben med tex funktioner och sökningar, i detta fall har jag valt att välja all indata från hubben samt, genom utdata funktionen, länka datat till Power BI verktyget.
  
![Utdata](/Bilder/StreamAnalytics_UtData.png)
 
![GrafPowerBI](/Bilder/DataGraf.png)

Power BI är ett verktyg för att på ett enkelt sätt visualisera data. ilden ovan visar med en graf hur min sensor har uppfattat ljusstyrkan i mitt rum över tid. 
Det syns här att jag startade sensorn den 19e maj en kort stund. Att det var en medelhög ljusstyrka, dagsljus. Det går också att se att sensorn/microkontrollen har varit avsängd till den 20 maj då jag på nytt startade upp för ytterligare mätning. I början av mätningen är mörkt i rummet för att sedan bli mycket ljust då jag tände en lampa precis brevid sensorn, mätningen från transistorn visar ett värde på över 4000. Det går att urskilja en mätning i mitten av den högsta stapeln som visar att styrkan på lampan skuvades ner till hälften för att sedan släckas helt och det blir mörkt i rummet där värdet visar 0.

Säkerhet 
 I mitt http protokoll skickar jag med ett certifikat för identifiering och SAS (Shared Access Signature) token för autentisering. 
