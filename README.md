# V1 versija (be AI)

## Pseudo-kodas

+ Funcija gauna stringą s
+ Inicijuojame kintamąjį *var = 0*
+ Inicijuojame vektorių *seed* skaičiais nuo 0 iki 63
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

## Eksperimentinis tyrimas

### Pavyzdžiai
|Įvedimas|Hashas|
|--------|------|
|lietuva|```18445343w31497Q55293N53271K50241I47215E4317965391312623611155933```|
|Lietuva|```KG95jgd4354494439343026211611623813182328C6257524743383329241914```|
|Lietuva!|```D3524d62403232D28124910F30105011G3195111G31951112932852132733754```|

### Išvedimo dydis
Išvedimo dydis nėra priklausomas nuo įvesties dydžio. Išvedimo dydis visada bus 64 simboliai:
```
                                    a –> 1194939291ZYXWVVUTSRQPONMLKJIHGFEDCB6564636261605958575655545352
Labai Labai labai ilgas ilgas tekstas –> FFM58594D26172013l313551575458384752212311561405958425417111915X
```

### Deterministiškumas
Visada ta pati įvestis duos tą pačią išvestį:
```
Testas –> R65922236621975SH6048362412012L64524028164719E574533219214253751
Testas –> R65922236621975SH6048362412012L64524028164719E574533219214253751
```

### Efektyvumas
Matuosime failo konstitucija.txt efektyvumą su skirtingu kiekiu eilučių. 

|Eilučių kiekis|Laikas mikrosekundėmis|
|--------------|----------------------|
|2|50|
|4|52|
|8|55|
|16|70|
|32|90|
|64|130|
|128|250|
|256|520|
|512|1200|
|789|1750|

### Kolizijų paieška
Sugeneruota po 100 000 atsitiktinių string porų, kurių ilgis būtų: 10, 100, 500, 1000 simbolių.

|String'o ilgis|Kolizijų kiekis|
|--------------|---------------|
|10|0|
|100|0|
|500|0|
|1000|0|

### Lavinos efektas

### Negrįžtamumo operacija
+ Hiding: Labai silpnas
+ Puzzle-Friendliness: Geras

### Išvados
+ Padariau labai didelę klaidą kad naudojau statinį salt'ą nes teoriškai galima žinant tą salt'ą gauti pradinę reikšmę
+ Nepaisant to, vizualiai hashavimas atrodo gerai, turi lavinos efektą, neturi kolilizijų bei yra efektyvus, tačiau nepaisant to, jokiu būdu negalima pasitikėti šiuo hashavimo algoritmu
+ Taip pat pastebėjau, kad kuo įvestis mažesnė, tuo daugiau hash'as turi raidžių, kas praktiškai "išduoda" hashuotos informacijos ilgį.
