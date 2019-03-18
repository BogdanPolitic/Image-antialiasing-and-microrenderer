


28-10-2018
POLITIC ANDREI-BOGDAN, 335CC
TEMA 1 APD


PART 1 - RESIZE

	Inaintea implementarii functiilor, am introdus cateva variabile globale:
		in_ref retine referinta la imaginea de input
		out_ref retine referinta la imaginea de output
		gK[3][3] este kernelul gaussian de ordin 3.
	Am avut nevoie de referintele in_ref si out_ref, pentru a putea accesa si modifica imaginile din functia de paralelizare, fara a fi
nevoie pasarea lor ca argumente.
	In functia readInput am citit mai intai cele patru variabile care in format PNM se afla inaintea matricei efective ce descrie
imaginea. Am alocat apoi memorie pentru matricea imaginii. Am luat in considerare doua cazuri: cel al imaginii PGM, ce are matricea de tip
unsigned char**, si cel al imaginii PPM, ce are matricea de tip **ppmCell (ppmCell este un tip definit in fisier, si reprezinta o celula RGB,
adica cele 3 canale unsigned char : B(blue), G(yellow), R(rosu)). Apoi, pentru fiecare din cele doua tipuri, am citit din fisier matricea
imaginii, pe care am stocat-o in campul pgmMatrix, sau ppmMatrix (dupa caz).
	In functia writeData, scrierea in fisierul de output am facut-o intr-un mod foarte similar.
	In functia resize, mai intai am initializat in_ref si out_ref cu referintele imaginilor in, respectiv out. Apoi am adaptat
dimensiunile imaginilor corespunzator pierderii de informatie din partea dreapta jos a imaginii, pentru cazul in care dimensiunile nu se
impart exact la factorul de resize, si am completat campurile structurii imaginii out. Luand in considerare cele doua tipuri (PGM si PNM), am
alocat memorie pentru imaginea out, in acelasi mod ca in functiile de citire si scriere. In final, am initializat thread-urile in functie de
num_threads dat ca input, si am dat drumul executiei thread-urilor aferente functiei resizeThread.
	Functia resizeThread este functia efectiva de resize, in care procesul de resize se realizeaza paralel pe num_threads core-uri.
Paralelizarea consta in impartirea task-ului pe linii. Fiecare core realizeaza operatiile pe linii diferite. Pentru a nu fi generate erori la
impartirea pe linii, pentru fiecare core, linia de start incepe de la cel mai mare index divizibil cu resize_factor, mai mic decat cel asignat
initial. Indexul liniei de final este dat de formula bine-stiuta: id-ul thread-ului curent inmultit cu numarul total de indecsi, impartit apoi
la numarul de thread-uri, iar pentru thread-ul de final, se realizeaza trunchierea, daca este necesar.
	Urmeaza algoritmul efectiv de resize si antialiasing, pentru toate cele 4 cazuri: imagine PGM sau PPM, resize_factor par sau impar. Am
iterat prin matricea de input cu indicii i si j, cu pas de resize_factor unitati. Astfel, fiecare i si j din iteratii reprezinta indecsi
pentru imaginea micsorata de output. Pentru fiecare i si j, am urmat algoritmul din enunt, insumand elementele din submatrice si apoi
impartind, asignand rezultatul celulei de pe linia i si coloana j din matricea de output.
	In aceasta functie, pentru fiecare citire/scriere din/in imagine, am folosit referintele in_ref si out_ref.



PART 2 - RENDER

	Inaintea implementarii functiilor, am introdus variabila globala in_ref care retine referinta la imaginea de input, pentru a putea
accesa si modifica din functia de paralelizare imaginea , fara a fi nevoie pasarea ei ca argument.
	Functia initialize primeste ca argument noul obiect de tip imagine, ale carui campuri le completez, in afara de matricea imaginii,
care se va completa in functia de threading. Campurile sunt: type (tipul imaginii, care automat este 5, adica PGM (grayscale)), linesNo(nr de
linii), columnsNo(nr de coloane), eqA, eqB, eqC (cei trei coeficienti ai ecuatiei dreptei).
	Functia render porneste direct procesul de paralelizare, pe num_thread core-uri, cu functia renderThread.
	In functia renderThread, accesul la imagine se face folosind referinta globala la imagine. Mai intai initializez punctele de start si
de end ale iterarii core-ului curent, fiecare core construind imaginea pe linii diferite. Apoi are loc algoritmul efectiv de render. Pentru
fiecare pixel din imagine, se verifica daca distanta de la centrul acestuia la linie este mai mica decat grosimea data, adaptata la rezolutia
(resolution) ceruta. Formula distantei am descris-o in functia distance, care primeste ca argumente indicii i si j, si masoara distanta din
centrul pixelului respectiv (punctul de coordonate i + 0.5, j + 0.5) la dreapta, conform formulei |eqA * x + eqB * y + c| / sqrt(a^2 + b^2).
