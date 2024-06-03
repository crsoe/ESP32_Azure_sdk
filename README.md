# **Projekt candela**
 
I det här projektet samlar jag in data från en fototransistor för ljusmätning, http-protokoll och sensor-kod är skriven i Arduino IDE. Data skickas till Azure hub som dirigerar data vidare till Power BI för enklare visualisering.
Jag använder mig av en ESP 32 WROOM, en fototransistor och en resistor på 10 kOhm.
 
![ESP32 wroom & phototransistor on breadboard](/ESP32Breadboard.jpg)
 
##⦁	Http-protokoll
I http-protokollet definierar jag mina Azure resurser samt min WiFi uppkoppling. I IOT explorer genererar jag SAS Shared Access Signature som nyckel till min Azure Hub.
Med hjälp av openssl uppkoppling genereras ett certifikat för identifikation av min IOT-sak.
 
Här talar jag om vilken URL och certifikat som ska användas för att  skapa en uppkoppling och identifiera min IOT-sak mot Azure.
Varje meddelande jag skickar har med information som ger mig rättigheter att publicera till hubben samt den json- sträng som innehåller mitt insamlade data från sensorn.
 
###⦁	Azure
I Azure har jag satt upp en prenumeration, en resursgrupp och en IOT Hubb.
Prenumerationen skapar en uppkoppling. 
Hubben kontrollerar att min sak har rätt att publicera. Fångar upp mitt meddelande och kan dirigera meddelandet vidare till Azure Stream Analytics. 
 
![Azure Slide](/Slide.jpg)
 
####⦁	Visualisering av data
Det är från Stream Analytics som Power BI läser in meddelanden och kan bearbeta data till den graf jag valt.
Grafen visar hur ljusstyrkan i rummet ändras med tiden. Högsta mätvärdet representerar den ljusaste tiden och lägsta när det är mörkast.