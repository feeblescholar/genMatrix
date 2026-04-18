# <center>genMatrix - Terv</center>
A feladat egy mátrix osztály sablon és egy tesztprogram megtervezését igényli. 
A sablon paraméterként a mátrix típusát kapja.
Az osztály sablonhoz tartozik egy hibaosztály (`Matrix_Error`) is, ami a `std::exception` gyereke (ld. később).
## A mátrixsablon
A mátrix méretét a konstruktorban megadott két `size_t` típusú paraméter határozza meg. 
A feladat nem szabja meg, hogy a mátrix statikus vagy dinamikus legyen, ezért mind a kettőt tudja, ehhez a "kulcs" a `(0,0)`-ás méret.
Az osztály sablon tagfüggvényei (ahol lehet) konstans objektumokra is meghívhatók (`const` correctness), így ezt a továbbiakban nem fogom külön kiemelni. A mátrix sorainak és oszlopainak száma, valamint a mérete példányosítás után is természetesen lekérhető.
### Mátrix tagjainak elérése, megadása és mozgatása
A tagok <b>egyetlen 1 dimenziós dinamikus tömbben</b> tároltak sorfolytonosan.
A mátrix `(i,j)`-dik tagját a túlterhelt `operator()(size_t, size_t)`-vel lehet elérni. A mátrixot fel lehet tölteni egyesével az előbbi módon vagy statikus esetben az <a href="https://libeigen.gitlab.io/">Eigen</a> mátrixkönyvtár stílusa szerint is, ami a következő:
``` 
Matrix<típus> A(2,2);
a << 1, 2, 3, 4;
```
A fenti kódrészlet matematikailag:
$$A = \begin{pmatrix}
1 & 2\\
3 & 4
\end{pmatrix}$$
Ezen "szintaktikai cukorka" segíti a használatot és a tesztelést is (<i>továbbá egy `CommaInit` nevű segédosztályt is, kizárólag erre a célra</i>). Fontos megjegyezni, hogy a fenti esetben a teljes mátrixot fel kell tölteni, különben a program kivételt fog dobni. 
A mátrix sorait és oszlopait a `swapRow(size_t, size_t)` és `swapCol(size_t, size_t)` függvényekel lehet cserélni. Túlindexelés esetén mindegyik fenti függvény kivételt dob.
### Összeadás, kivonás
Az összeadást, kivonást, szorzást a matematikailag definiált módon hajtjuk végre. Az előbbi 2 feltétele, hogy a mátrixok mérete azonos legyen.

$$A \pm B =
\begin{pmatrix}
a_{1,1} & \dots & a_{1,n} \\
\vdots & \ddots & \vdots \\
a_{m,1} & \dots & a_{m,n}
\end{pmatrix}
\pm
\begin{pmatrix}
b_{1,1} & \dots & b_{1,n} \\
\vdots & \ddots & \vdots \\
b_{m,1} & \dots & b_{m,n}
\end{pmatrix} =
\begin{pmatrix}
a_{1,1} \pm b_{1,1} & \dots & a_{1,n} \pm b_{1,n} \\
\vdots & \ddots & \vdots \\
a_{m,1} \pm b_{m,1} & \dots & a_{m,n} \pm b_{m,n}
\end{pmatrix}$$

Skalárral is ugyanez a definicíó (elemenként $\pm$). A fenti műveletekhez az algoritmus triviális, ezért nem kerül külön leírásra.

### Szorzás
A mátrixok szorzása szintén történhet skalárral vagy egy másik mátrixal. Skalárral való szorzás esetén minden elemet megszorzunk az adott értékkel:
$$cA = c
\begin{pmatrix}
a_{1,1} & \dots & a_{1,n} \\
\vdots & \ddots & \vdots \\
a_{m,1} & \dots & a_{m,n}
\end{pmatrix} =
\begin{pmatrix}
c\cdot a_{1,1} & \dots & c\cdot a_{1,n} \\
\vdots & \ddots & \vdots \\
c\cdot a_{m,1} & \dots & c\cdot a_{m,n}
\end{pmatrix}$$
Két mátrix szorzásának feltétele, hogy a bal tényezőnek annyi sora legyen, ahogy oszlopa a jobbnak. Az eredmény a matematikai formulát követi:
$$(AB)_{i,j} = \sum_{k=1}^{n} a_{i,k}b_{k,j}$$
Az implementáció a sebesség érdekében viszont cache-barát eljárással történik, ami lényegében csak a sorrendben tér el a megszokottól. Először vesszük A `i`.-dik sorának `k`.-dik elemét majd megszorozzunk B teljes `j`.-dik sorával, amit a megfelelő helyen hozzáadunk C-hez.

Az <i>"ikj"</i> mátrixszorzás ($A\cdot B$) algoritmusa pszeudokóddal:
```
szoroz(Matrix<típus> A, Matrix <típus> B):
    if (A.n != B.m) throw Matrix_Error(...);
    Matrix C<típus>(A.n, B.m);
    for (i = 0; i < A.n; i++)
        for (k = 0; k < A.n; k++)
            for (j = 0; j < A.n; j++)
                C(i,j) += A(i, k) * B(k, j);
    return C
```
Lépésszáma: $O(n^3)$. 

<i> Megjegyzés: A mátrixszorzás nem kommutatív. </i>

Ezen műveletek végrehajthatók a balértéken (`+=`, `-=`, `*=`) vagy kerülhetnek egy teljesen új mátrixba (`+`, `-`, `*`).

### Transzponálás
A transzponálás során a mátrix `(i,j)`.-dik elemét felcseréljük a `(j,i)`.-el. Négyzetes mátrix esetén optimalizálhatjuk az algoritmust, azzal hogy a főátlóra tükrözünk. A művélet elvégezhető helyben vagy egy új mátrixon (<i>a példában itt helyben szerepel</i>):
```
Transzponál(Matrix<típus> A)
    if (n == m)
        for (i = 0; i < A.n; i++)
            for (j = i + 1; j < m; j++)
                csere(a(i,j), a(j,i));
    else
        Matrix<típus> tmp(A.m, A.n);
        for (i = 0; i < A.n; i++)
            for (j = 0; j < A.m; j++)
                tmp(i, j) = a(j, i);
    A = tmp;
```
Lépésszáma: $O(n^2)$.
### Determináns kiszámítása
A determináns egy <b>kizárólag négyzetes</b> mátrix összes lehetséges "bástyaelhelyezés" előjeles szorzatának összege (a Leibnitz-formula szerint):
$$\det(A) = \sum_{\sigma \in S_n} \text{sgn}(\sigma) \prod_{i=1}^{n} a_{i, \sigma(i)}$$
A fenti formula viszont nem túlzottan számítógép-barát (megoldható lenne, de $O(n\cdot n!)$ időben).
Viszont egy felső háromszög mátrix determinánsa "csak" a főátló szorzata, azt pedig előállítjuk az ennél jóval "olcsóbb" Gauss-eliminációval:
```
Gauss(Matrix<típus> A):
    if (A.n != A.m) throw Matrix_Error(...);

    Matrix<típus> B = A;
    det = 1;

    for (i = 0; i < B.n; i++)
        pivot = i;
        for (j = i + 1; j < B.n; j++)
            if (abs(B(j, i)) > abs(B(pivot, i)))
                pivot = j;
        
        if (abs(B(pivot, i)) < EPSILON) return 0;
        
        if (pivot != i) {
            swapRow(B(pivot), B(i));
            det *= -1;
        }
        
        for (j = i + 1; j < B.n; j++)
            div = B(j, i) / B(i, i);
            for (k = i; k < B.n; k++)
                B(j,k) -= div * B(i, k);
    
    for (i = 0; i < n; i++)
        det *= B(i, i)

    return det;
```
Itt alkalmazzuk a partial pivoting-et, magyarán megpróbáljuk az adott oszlop legnagyobb elemével osztani. Ezt azért tesszük, mert ha `B(k, k)` túl kicsi, az eredmény pontatlan lesz. Így (viszonylag) stabil a számítás és a lehető legpontosabb (ennél már csak az LU felbontás pontosabb, de jóval trükkösebb kezelni).
Lépésszáma: $O(n^3)$.

## Invertálás
Az invertálás (a determinánshoz hasonlóan) szintén csak négyzetes mátrixon értelmezet, melynek determinánsa nem nulla. A megvalósító algoritmus csak annyiban tér a deteminánshoz képes, hogy az előjeleket nem vizsgáljuk és az eredeti mátrix jobb oldalát kibővítjük egy $E_{n}$ egységmátrixszal, Gauss-elimináljuk (úgy, hogy a bővítésen is elvégezzük a műveleteket, de nem tekintjük a mátrix részeként) és a bővített módosított egységmátrix, az inverz mátrix.

## Műveletek típusok között
A típusok közti áthidalást segíti, ha különböző típusú mátrixokra is értelmezhetnénk a műveleteket. Erre vannak különböző megoldások (specializáció, type erasure, ...) itt egy új sablonváltozót vezetünk be. A bináris operandusok egy speciális sablont kapnak, amely képes az adott műveletre példányosítani az adott műveletet, ehhez az `auto` és `decltype` párost használjuk:
```cpp
template <typename T> class Matrix {
    ...
public:
    ...
    template <typename U> auto operator+(const MyClass<U>& other) const {
        return Matrix<decltype(value + other.value)>(value + other.value);
    }
    ...
};
```
<i>Megjegyzés: Ezen problémák az implementációra vannak hagyva, mert a konzisztens eredményekhez széles körű tesztelés szükséges. 
Nem szerepel az osztályhiearchia grafikonon sem (természetesen a véglesen fog).</i>


## A mátrix iterátora
A specifikációban szereplő mátrix iterátor a `Matrix_Iterator`. Az öröklés itt mellőzve lett, mert a `std::iterator` a C++ 2017 óta elavultnak (`deprecated`) számít és a programak `-Wall –Werror` kapcsolókkal kell fordulnia. A helyét a modern type aliasok váltották fel, ez is így készült. A funkcionalitás tekintetétben az iterator egy szabványos `std::random_access_iterator`, tehát rendelkezik minden szükséges operátorral, amit a <a href=https://en.cppreference.com/cpp/iterator/random_access_iterator>szabvány</a> megkövetel. Az iterátor a mátrixsablon komponense, ugyanarra a típusra példányosul, mint maga a főosztály.

## A mátrix hibaosztálya
A mátrix hibaosztálya tárolja a függvényt ahol a hiba keletkezett, a hibaüzenetet és azt, hogy kritikus hiba-e (ki kell-e lépni). Az üzeneteket az objektum dinamikusan foglalja le, majd megszűnéskor szabadítja fel. 

A hibaosztály a `std::exception` absztrakt osztály gyereke. Ebből kifolyólag a hibaüzenetet a `what()` tagfüggvénnyel lehet elérni. 
A `src()` tagfüggvény a függvény nevét adja vissza, ahol a hiba keletkezett, ez debuggolás során hasznos információ, főleg, ha nem helyben kezeljük a kivételt.

A hibaosztály nem komponense a mátrix osztálynak, mert nem típusfüggő. 
Ezzel rengeteg felesleges kódtól megkíméljük a fordítót és csökkentjük a fordítási időt.

## Egyesített osztálydiagram
Az UML diagram PlantUML kódból készült. A PlantUML editor <a href=https://editor.plantuml.com>itt</a> érhető el. A forrásfájl, amiből generálták, a vektorgrafikás képpel együtt elérhető a projekt GitHubon <a href=https://github.com/feeblescholar/genMatrix>itt</a>. <center><img src="UML diagram rendered.svg"></img></center>

## Tesztelés
A tesztprogram a szabványos bemenetről fog várni egy karaktert (implementáció kérdése lesz, mi legyen), amire az összes tesztesetet le fogja futattni, majd kilépni. 
Ez a C(J)PORTA miatt lesz kialakítva. 

A tesztesetek a `int, float, double, Komplex` típusokra fognak lefutni, utána következnek a típusok közötti tesztek. A `Komplex` osztály egy saját osztály lesz dinamikus erőforrásokkal. Az egyes típusokon lefuttatott tesztek (jellege) ugyanazok lesznek. A tesztelés szükséges adatokat (annak egy részét) a program fájlból fogja betölteni, majd a végén ki is fogja írni, ezzel is tesztelve a fájlkezelést 

Ha a `CPORTA` makró nem definiált egy egyszerű felhasználói felület fog megjelenni, 1-2 hasznos funkcióval.