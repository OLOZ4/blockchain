# V1 versija (be AI)

## Pseudo-kodas

+ Funcija gauna stringą s
+ Inicijuojame kintamąjį *var = 0*
+ Inicijuojame vektorių *seed* skaičiais nuo 0 iki 63
----------------
+ Kiekvienam stringo s elementą i (i priklauso nuo 0 iki s.length-1) pridedame į seed vektorų naudodami ```seed[i % 64] ^= returnVal(s[i]) + i```
+ Išrūšiuojame seed vektorių
+ Įvedame naują kintamąjį *sum = 0*
+ Iš seed vektoriaus pridedame skaičius prie kintamojo sum nuo mažiausio jei kintamasis sum < 1000
+ Kiekvienas seed vektoriaus elementas i (i priklauso nuo 0 iki 63) yra modifikuojamas pagal formulę ```seed[i] = abs(seed[i]–var2)``` čia ``` var2 = (var - sum * i) % 100```
+ Kiekvienam seed vektoriaus elementui naudojame šią formulę ```seed[i % 64] ^= returnVal(s[i]) + i;```
+ Nustatome stringą ```result = ""```
+ Galiausiai imame visus seed elementus (visi elementai yra skaičiai) ir tikriname, jei elementas yra tarp 65 ir 91 arba tarp 96 ir 123 - jei tiesa, tai verčiame į raidę ir pridedame pries stringo result, o jei ne, paliekame skaičių ir taip pat pridedame prie stringo result
+ String'ą result "nupjauname" ir paliekame tik pirmus 64 simbolius
+ Gražiname result 

2. Paimame kiekviena ivesties simboli ir 1. paverciame ji skaiciu ir 2. atimame is to skaiciaus 87. 

3. Gautus skaicius pridedame prie seed'o masyvo

4. Taip pat is gautu skaiciu suskaiciuojame kintamaji nr.1 naudojant formule: var += returnVal(s[i]) *  pow(10,i+1);
5. Isrusiuojame seed'o masyva didejimo tvarka

6. Skaiciuojame suma: sudedame visus skaicius seedo vektoriuje esancius skaicius sudedame iki kol suma nevirsyja 1000.

7. Skaiciuojame kintamaji nr. 2 : var2 = (var - sum*i)%100; cia i priklauso nuo 0 iki 63. t.y. gauname du skaicius ir juos atimame is i-tojo seed'o vektoriaus elemento ir idedami ji i moduli

8. Isivedame nauja kintamaji: rezultata. Paimame kiekviena seed'o vektoriaus elementa (skaiciu) ir jei jis yra tarp 65 ir 122 tai paverciame ji i simboli pagal ascii lentele ir pridedame i rezultato stringa, kitu atveju skaiciu paverciame i stringa ir pridedame i ta pati stringa. 

9. Isvedame pirmus 64 simbolius is rezultato stringo.
