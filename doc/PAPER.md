# Besturingsalgoritme lijnvolger

## Inhoudsopgave
* [Inleiding](https://github.com/ramonvbemmel/lijn_volger/PAPER.md#inleiding)
* [Doelen](https://github.com/ramonvbemmel/lijn_volger/PAPER.md#doelen)

## Inleiding
In dit document wordt ingegaan op het door ons ontwikkelde
algoritme. Onze robot (gebouwd op LEGO mindstorms) wordt
bestuurd door een programma dat draait op een Raspberry Pi en input
geeft aan de robot via een BrickPi shield.

De bedoeling van de robot is dat hij volledig autonoom een zwarte lijn
volgt op een wit veld. Hij moet dus ook bochten kunnen draaien, objecten ontwijken
en de lijn terug kunnen vinden.

De BrickPi dient voor ons als interface met sensoren en actuatoren op
de robot. De robot zelf is een lego mindstorms robot, maar i.p.v. de
meegeleverde computer van mindstorms gebruiken we dus bovengenoemde
configuratie van Raspberry Pi plus een BrickPi shield. Op het BrickPi
shield zitten MMJ aansluitingen om via een serieel protocol met de 
motoren en sensoren te kunnen communiceren.

## Doelen
Het algoritme moet zo werken dat de lijnvolger:

* Een lijn kan zien door middel van een zwart-wit contrast.
* Op een lijn kan blijven door zijn rijrichting aan te passen.
* Bij een bocht kan afslaan.
* Obstakels op de lijn kan ontwijken.
* Zijn weg terug kan vinden naar de lijn.

## Algorithme
Het maken van bochten is het makkelijkst als we dit proportioneel
doen. We kunnen de lichtwaardes meten en kijken wat wit, zwart en
gecombineerd wit/zwart is. De bevindingen zijn als volgt;

|Sensor type | Dark | Light|
|---|---|---|
Light sensor | `> 2000`| `< 2000` | 
RGB sensor |`< 450` | `> 450`|

De sensoren, vooral de zwart/wit sensor, zijn wel heel erg gevoelig.
Om de grote fluctuaties in de uitlezingen te verminderen delen wij deze door 
een getal om een kleinere schaal te bereiken. We gebruiken deze waardes om proportioneel
bij te kunnen sturen waardoor de robot veel soepeler over de lijn rijdt.

De formule die we gebruiken is als volgt:


