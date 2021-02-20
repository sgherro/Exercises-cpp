# Matrix Differences Example (ITA version)

## Esercizio 1
Si scriva un programma in linguaggio C++, a linea di comando, che accetti la seguente sintassi:
freq_img <file PGM> <output TXT>
Il programma prende in input un file in formato PGM e per ogni possibile valore dei pixel ne conta le
occorrenze. In output produce un file di testo composto da una riga per ogni valore da 0 a 255 con il
seguente formato:

<byte><tab><occorrenze><a capo>

Il byte e le occorrenze sono salvati in base dieci. Le righe sono ordinate da 0 a 255. Infine invia su
standard output l’entropia dell’insieme di simboli secondo la distribuzione appena calcolata, secondo il

formato:

Entropia:<spazio><valore><a capo>

Il valore è espresso come numero decimale.

## Esercizio 2

Si scriva un programma in linguaggio C++, a linea di comando, che accetti la seguente sintassi:
diff_img <file PGM> <prefisso di output>
Il programma prende in input un file in formato PGM e costruisce in memoria una matrice differenza.

Con matrice differenza (D) si intende la matrice costruita dall’immagine originale (I), secondo le regole
seguenti:
<pre>
 l’elemento 𝐷(0,0) vale quanto il pixel 𝐼(0,0)
 gli elementi 𝐷(0, 𝑦) valgono 𝐼(0, 𝑦) − 𝐼(0, 𝑦 − 1), per 𝑦 > 0
 gli elementi 𝐷(𝑥, 𝑦) valgono 𝐼(𝑥, 𝑦) − 𝐼(𝑥 − 1, 𝑦), per 𝑥 > 0
</pre>

Ad esempio:
<pre>
I           ->            D
a b c d           a   b-a c-b d-c
e f g h           e-a f-e g-f h-g
i j k l           i-e j-i k-j l-k
m n o p           m-i n-m o-n p-o
</pre>

Ovviamente i valori della matrice differenza non possono essere memorizzati in un uint8_t.

Dalla matrice differenza si costruisca l’immagine differenza, ovvero una versione visualizzabile della
matrice delle differenze. Per farlo basta trasformare ogni differenza dividendo per due e aggiungendo
128. Il risultato è ora compreso tra 0 e 255, quindi può essere salvato in formato PGM, nel file che ha
come nome il prefisso di output e l’estensione .PGM.
Successivamente, per ogni possibile valore della matrice differenza (-255…255) ne conta le occorrenze.

In output produce un file di testo composto da una riga per ogni valore da -255 a 255 con il seguente
formato:

<valore><tab><occorrenze><a capo>

Il valore e le occorrenze sono salvati in base dieci. Le righe sono ordinate da -255 a 255. Infine invia su
standard output l’entropia dell’insieme di simboli secondo la distribuzione appena calcolata, secondo il
formato:

Entropia:<spazio><valore><a capo>

Il valore è espresso come numero decimale.
