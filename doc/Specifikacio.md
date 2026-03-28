# <center> genMatrix - Specifikáció </center>
## Feladat
A feladat a következő (<a href=https://infocpp.iit.bme.hu/hf_otlet>forrás</a>):\
\
<i>
Készítsen generikus intelligens mátrixot, melynek mérete definiálható! Valósítsa meg az összes értelmes műveletet operátor átdefiniálással (overload), de nem kell ragaszkodni az összes operátor átdefiniálásához! Használjon iterátort is!
Specifikáljon egy egyszerű tesztfeladatot, amiben fel tudja használni az elkészített adatszerkezetet! A tesztprogramot külön modulként fordított programmal oldja meg! A programmal mutassa be a generikus szerkezet használatát több egyszerű adathalmazon, amit fájlból olvas be, és egy olyan saját osztályon, amely dinamikus adatot tartalmaz.
</i>
\
\
A feladat továbbá <b>tiltja</b> az STL tárolók használatát (ennek ellénére a tesztek során előkerülhetnek).
## Feladat specifikáció
A feladat (a fentieket elemezve) tehát egy generikus `(n x m)` mátrix elkészítése, amelyen elvégezhető az összes értelmes mátrix művelet. A mátrixnak ezen felül rendelkeznie kell saját `std::iterator`-ral is.
### A mátrixon elvégezhető műveletek
A feladatban szereplő <i>összes értelmes művelet</i> a következőképpen definiált:

* Összeadás
* Kivonás
* Szorzás
* Transzponálás
* Sor- és oszlopcsere
* Invertálás
* Determináns kiszámítása

Ezek segítségével lefuttatható az összes (gyakorlatban is használt) létező mátrixon futó algoritmus, továbbá komplex műveletek is megvalósíthatók (pl. forgatás), de ezen problémák már a használat körébe esnek.

<i>Megjegyzés: A műveletek csak akkor elvégezhetőek, <b>ha azoknak van értelme</b> és a típus rendelkezik a megfelelő túlterhelt operetátorokal. (Pl. nem négyzetes mátrix determinánsa nincs értelmezve, `MyClass`-ból készült mátrixot nem lehet összeadni `MyClass` túlterhelt `operator+`-a nélkül.)</i> 

A fenti műveleteken kívül a mátrixot lehet másolni, új értéket adni neki, az egyes elemekhez hozzáférni az `operator()` túlterhelésével. A mátrixot a későbbi felhasználás érdekében ki lehet írni fájlba majd visszaolvasni azt.
### Szükséges feltételek egy típus példányosításához
<b>A generikus mátrix egy sablonnal lesz megvalósítva</b> és csak olyan típusra példányosítható, amely legalább az értékadás művelete értelmezett (`operator=`), továbbá bemeneti (`std::istream& operator>>`) és kimeneti (`std::ostream& operator<<`) adatfolyamról/adatfolyamra olvasható/írható, máskülönben nem lehetne fájkezelést folytatni velük. 

A többi művelet csak akkor példányosul az adott típusra, ha a típus rendelkezik a szükséges operátorokkal.

A dinamikus erőforrásokkal rendelkező típusok esetében szükséges, hogy a típus destruktora felszabadítsa azokat. A mátrix nem ellenőrzi ezt, csak a destruktort fogja meghívni, a helyes felszabadítás (típusszinten) a típus hatásköre és feladata.

### Hibakezelés és tesztelés

A hibák kezelésére a mátrix saját hibaosztályt dob, ami a `std::exception` absztrakt osztály gyermeke, így egyszerűen kezelhető más kivételekkel. Ilyen hibák pl. a túlindexelés, nem értelmezhető műveletek, fájlkezelés során fellépő hibák, (...). 

A tesztelés a `gtest_lite` keretrendszerrel fog történni. A tesztesetek típusonként fognak történni, tehát a sablont a teszt példányosítja egy adott típusra majd végigfuttatja az összes (értelmezhető) műveletet rajta és teszteli 1-2 szélsőséges eseten is. A memóriaszivárgást a `memtrace` fogja ellenőrizni. Az előbbi könyvtárak <a href=https://git.iit.bme.hu/prog2/seged/memtrace_gtest>itt</a> letölthetőek.

A tesztelés során használt saját osztály egy egyszerű dinamikus string osztály, mely rendelkezik a mátrix műveletek egy részével, ezen kívül pár alaptípus is tesztelt.

#### Gyakorlati teszt
Az egyszerű gyakorlati tesztfeladat a következő egyenletrendszer megoldása:
```
2x +  y -  z + 3w +  v = 7
-x + 4y + 2z -  w + 5v = 3
3x - 2y +  z + 2w -  v = 10
 x +  y +  z +  w +  v = 8
5x - 3y + 4z - 2w + 2v = 1
```
<i> Megjegyzés: a fenti egyenletrendszert az OpenAI ChatGPT "GPT-5.3 (adaptive)" modellje generálta 2026. 03. 28.-án 21:08-kor. Prompt: "Generate me a linear system of equations."</i>

### Felhasználói felület
A programot a parancssorból lehet futtatni és menüvezérelt. A programon kívül a futtatáshoz egyéb fájl nem szükséges. A be- és kimenet a szabvályos bemenet (`stdin`) és kimenet (`stdout`). A hibakimenet a szabványos hibakimenet (`stderr`). A program felhasználói felülete egyszerű, mert a genMatrix elsődlegesen egy generikus adattípus nagyobb problémák megoldásához, önmagában legfeljebb egy "mátrix számológép"-ként lehet használni. 

## Fejlesztés
A fejlesztés során a `git` verziókezelő szoftver használt. A teljes projekt elérhető a GitHubon <a href=https://github.com/feeblescholar/genMatrix>itt</a> (kezdve ezzel a fájlal). A csak szöveges fájlok Markdown (`.md`) formátumban lesznek elérhetőek. A projekt programozói dokumentációjának elkészítése doxygennel történik.