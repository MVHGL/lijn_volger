# Sturingsalgoritme lijnvolger

## Index
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

We willen ook kijken of we de lijnvolger zelf kunnen laten bepalen wat wit
of zwart is door hem een meting uit te laten voeren, waarbij er een grenswaarde
is die aangeeft of de datgene wat hij ziet zwart of wit is, of geen van beide.
