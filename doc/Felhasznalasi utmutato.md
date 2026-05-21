# <center>genMatrix - Felhasználási útmutató</center>

A genMatrix egy mátrixkönyvtár mely képes generikus mátrixokat és vektorokat kezelni. Ezen dokumentum a genMatrix használatát részletezi, a genMatrix működése a programozói dokumentációban részletesen megtalálható. A könyvtár forráskódja elérhető <a href=https://github.com/feeblescholar/genMatrix>itt</a>. A használatot a főprogramban található 3 tesztfeladat szemlélteti.

## Osztályok példányosítása
A genMatrixban jelenleg 3 db fő osztály példányosítható, a `Matrix`, `Vector` és `Complex` osztály. Az előbbi kettő (továbbiakban tárolók) sablon tehát a példányosításhoz meg kell adni az osztály típusát is, pl. `Matrix<int>` vagy `Vector<double>`. 
A teljesen támogatott osztályok (`T`) a következők: `int, long, float, double, Complex`. 
A méretekre az alábbi szabályok vonatkoznak:
* `Matrix<T> mtx(n, m)` létrehoz egy `n x m`-es statikus `T` típusú mátrixot.
* `Matrix<T> mtx` vagy `Matrix<T> mtx(0, 0)` létrehoz egy dinamikus `T` típusú mátrixot.
* `Vector<T> v(n)` létrehoz egy `n` kapacitású `T` típusú oszlopvektort.
* `Vector<T> v` létrehoz egy `1` kapacitású `T` típusú oszlopvektort.

Nem megfelelő méretezés esetén az osztály `genMatrix_Error` kivételt dob.

## Adatelérés
A mátrix osztály esetében az `M(i, j)` indexeléssel érhető el a mátrix i.-dik sorának j.-dik adata. Vektor esetében a `V[i]` indexeléssel érhető el a vektor i.-dik adata (ahol `V[0]` a legfelső elemnek felel meg a standard matematikai jelölésben).
Túlindexelés esetén az adott osztály `genMatrix_Error` kivételt dob.
A tárolók ezen kívül bejárhatóak range based `for` loopal (`for (auto p : mtx)`) valamint rendelkeznek iterátorokkal a `.begin()` és `.end()` formájában, melyek biztosítják az STL kompatibilitást. A `Matrix` osztály ezen kívül írható `<<` fájlba (tetszőleges stream) és vissza is lehet olvasni (`>>`) onnan (dinamikus mátrix kell hozzá).

## Értékadás
A tárolók esetében az értékadás történhet:
* Egy másik, azonos fajtájú tárolóval: pl. `Matrix<int> M = M1`
* Indexelés útján: pl. `V[0] = 2`
* Akár eltérő fajtájú tárolóval: `Vector<int> V(M, 2)`
* A `<<` operátorral: `M << 1, 2, 3, 4`
    * Mátrixok esetében itt pontosan annyi elemet kell, megadni      amekkora a méret (csak statikus mátrixon használható)
    * Vektorok esetében nincsen korlátozás, ha az új elem nem fér bele, akkor a vektor kapacitása nőni fog.

Helytelen értékadás esetén az adott osztály `genMatrix_Error` kivételt dob.

## Műveletek a tárolókon
A tárolókon megvalósított műveletek a matematikailag elvárt módon működnek. Az adott elemi művelet nem fog példányosulni, ha az tároló típusa nem rendelkezik az adott túlterhelt operátorral. A tárolók ezen kívül értékül adhatók, másolhatók és támogatják a move (`&&`) szemantikát is.

Mindkét tároló a képes eltérő típusok (de azonos fajtájú tárolók) közti elemi műveletvégzésre is (ha a konverzió lehetséges természetesen). Ha az eredmény új tárolóba kerül (`+, -, *`), az eredmény típusát a könyvtár újraszámítja és konvertálja. Ha az eredmény a balértékbe kerül (`+=, -=, *=`), akkor az eredmény típusa a balérték típusa.

A tárolók közti művelet definiált művelet az csak a szorzás (`*`). A többi operátor nem definiált explicit, de a tárolók közti konverzió (`Matrix <-> Vector`) elérhető.

Mindkét tároló esetében elérhető az 1-es (`norm_1(*tároló*)`), végtelen (`norm_inf(*tároló*)`) és Frobenius (`norm_frobenius(*tároló*)`) norma kiszámítása. 

### Mátrixspecifikus függvények
* `.getRows()` és `.getCols()` visszaadja a mátrix sorainak/oszlopainak számát
* `.getDataLocation()` segítségével lekérhető, hogy a mátrix statikus vagy dinamikus-e
* `.size()` visszaadja a mátrix méretét
* `.resize(n, m)` újraméretez egy <b>dinamikus</b> mátrixot
* `.swapRow(i, j)` és `.swapCol(i, j)` megcseréli a mátrix két sorát/oszlopát
* `.transpose()` és `.transposeInPlace()` transzponálja az adott mátrixot (új mátrixba vagy helyben)
* `.inverse()` kiszámítja a mátrix inverzét
* `det(mtx)` kiszámítja a mátrix determinánsát

### Vektorspecifikus műveletek
* `.shrink()` leszűkíti a vektor méretét a kapacitásra
* `.resize()` megadja a vektor új kapacitását
* `.push_back(*érték*)` hozzáadja az értéket a vektor végéhez
* `.pop_back(*érték*)` kitöröl egy értéket a vektor végéről
* `.remove(*index*)` kitöröl egy elemet az adott indexen

Nem definált (de meghívott) vagy érvénytelen művelet esetén a tároló `genMatrix_Error` kivételt dob.

## Kivételkezelés
A genMatrix tárolói rendelkeznek külön-külön hibaosztállyal (`Matrix_Error` és `Vector_Error`), de a (jövőbeli) kompatibilitás érdékében javasolt a bázisosztály elkapása. A `.src()` tagfüggvény megadja a függvény nevét ahol a kivétel keletkezett és a `.what()` tagfüggvény pedig visszaadja a hibaüzenetet (ahogy a C++ `std::exeption` specifikálja). A egyetlen kivétel az előbbiek alól a `Complex` osztály 0-val való osztása, ő `std::domain_error` kivételt dob.