# Stuuralgorithm brickpi lijnvolger

## Index
* [Inleiding](https://github.com/ramonvbemmel/lijn_volger/PAPER.md#inleiding)

## Inleiding
In dit document wordt ingegaan op het door ons (Team 1) onwtikkelde
algoritme. Onze robot (gebouwd op LEGO mindstorms) wordt
bestuurd door een programma dat draait opeen Raspberry Pi en input
geeft aan de robot via een BrickPi shield.

De bedoeling van de robot is dat hij volledig autonoom een zwarte lijn
volgt op een wit veld. Hij moet dus ook bochten kunnen draaien, objecten ontwijken
en de lijn terug kunnen vinden.

De BrickPi dient voor ons als interface met sensoren en actuatoren op
de robot. De robot zelf is een lego mindstorms robot, maar i.p.v. de
meegeleverde computer van mindstorms gebruiken we dus bovengenoemde
configuratie van Raspberry Pi plus een BrickPi shield. Op het Brickpi
shield zitten MMJ aansluitingen om via een serieel protocol met de 
motoren en sensoren te kunnen communiceren.

