== homework.h ==

Am declarat o structura de tip pixel in care sunt cele 3 culori red, green, blue.
Am declarat o structura de tip image care sa contina type, width, height, maxval 
si in funtie de tipul imaginii o matrice de unsigned char pentru gray si o matrice
de pixeli pentu color.

== homework.c ==

readInput:
Am citit din fisier type, width, height si maxval
In functie de type, am alocat memorie si am citit matricea pe linii.

writeData:
Am scris in fisier type, width, height si maxval
In functie de type, am scris in fisie linie cu linie

threadFunction - resize:
Am initializat parametrii type, width, height si maxval pentru imaginea de out
In functie de type, am alocat memorie si apoi am apelat functie threadFunction
pentru a paraleliza operatiile din cadrul matricii.
In threadFunction am impartit pentru fiecare thread ce interval din cadrul matricii
este rezolvat de acesta. M-am asigurat ca start-ul si end-ul sunt divizibile
pentru resize_factor.
Pentru type-ul imaginii de gray am verificat resize_factor. Daca resize_factor 
este divizibil cu 2, atunci merg din bloc de resize_factor x resize_factor
in bloc de resize_factor x resize_factor. In cadrul blocului, calculez suma elementelor
din bloc pe care o impart in resize_factor ^ 2 si o pun in imaginea de out.
Daca resize_factor este 3, repet aceeasi parcurgere doar ca de aceasta data,
suma o calculez inmultind fiecare element din cadrul blocului cu elementul
respectiv din GaussianKernel si le sumez(produsele). La final, suma o impart 
la 16 si o pun in imaginea de out.
Daca type-ul indica o imagine color, atunci parcurgerea din  bloc de 
resize_factor x resize_factor in bloc de resize_factor x resize_factor
se pastreaza, doar ca pentru fiecare camp din cadrul structurii pixel, adica red,
green, blue) se calculeaza o suma. Sum_red, sum_green, sum_green le calculez
insumand fiecare element din cadrul blocului curent si apoi il impart la
resize_factor ^ 2. Sumele le pun in imaginea de out
Aceeasi parcurgere este si pentru cazul in care resize_factor este 3.
Doar ca de aceasta data, sum_red, sum_green si sum_blue sunt calculate
inmultind fiecare element corespunzator culorii cu elementul specific din
GaussianKernel si insumand toate aceste produse din cadrul blocului.
La final, sumele le impart la 16 si le pun in imaginea de out.

 == homework1.h ==

Am declarat o structura de tip image care sa contina type, width, height, maxval
si o matrice de unsigned char.

== homework1.c ==

initialize:
Initializez campurile type(cu P5 deoarece imaginea este gray), width, gray
maxval si apoi aloc memorie.

writeData:
Scriu in fisier type, width, height si maxval, iar pentru campul matrix
scriu linie cu linie.

threadFunction - render:
In render am declarat thread-urile si creat thread-urile.
In threadFunction am calculat fiecare bucata pe care thread-ul trebuie sa o rezolve
Am parcurs imaginea de dimensiunea resolution x resolution "pixel" cu "pixel".
Pentru fiecare am calculat distanta de la centrul acestuia(100 / resolution *  0.5)
la dreapta. Daca aceasta distanta este mai mica sau egala cu 3(grosimea), atunci
pixelul are culoarea negru(valoare 0). Altfel, pixelul are culoarea
alb(valoarea 255).
Pentru y-ul din cadrul fiecarui pixel, am tinut cont ca aceasta
merge de jos in sus(e.g. Pentru liniile 0, 1, 2 eu trebuie sa scriu in (3 - 1 - 0), 
(3 - 1 - 1), (3 - 1 - 2).

== Scalabilitate ==
Timpii au fost obtinuti prin rularea pe cluster pe masinile din coada ibm-nehalem.q

RESIZE :
Gray:
resize_factor = 2
	num_threads = 1 : 0.004281
	num_threads = 2 : 0.001983
	num_threads = 4 : 0.001413
	num_threads = 8 : 0.001068

resize_factor = 3
	num_threads = 1 : 0.004660
	num_threads = 2 : 0.002663
	num_threads = 4 : 0.001522
	num_threads = 8 : 0.000857

resize_factor = 8
	num_threads = 1 : 0.002697
	num_threads = 2 : 0.001654
	num_threads = 4 : 0.000911
	num_threads = 8 : 0.000712

Color:

resize_factor = 2
	num_threads = 1 : 0.010098
	num_threads = 2 : 0.004759
	num_threads = 4 : 0.003335
	num_threads = 8 : 0.001684

resize_factor = 3
	num_threads = 1 : 0.013157
	num_threads = 2 : 0.006812
	num_threads = 4 : 0.003754
	num_threads = 8 : 0.002472

resize_factor = 8
	num_threads = 1 : 0.005316
	num_threads = 2 : 0.002984
	num_threads = 4 : 0.002378
	num_threads = 8 : 0.001364

RENDER:
resolution = 1000
	num_threads = 1 : 0.021576
	num_threads = 2 : 0.011244
	num_threads = 4 : 0.005881
	num_threads = 8 : 0.004316	

resolution = 8000
	num_threads = 1 : 0.809362
	num_threads = 2 : 0.415088
	num_threads = 4 : 0.239357
	num_threads = 8 : 0.141774
