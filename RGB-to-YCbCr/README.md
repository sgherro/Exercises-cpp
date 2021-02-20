# RGB to YCbCr conversion example (ITA version)

Le formule per il passaggio da RGB a YCbCr e viceversa sono molte e non sempre chiarissime. Gli
standard sono piuttosto esigenti e hanno molti dettagli di non facile comprensione. Per la maggior parte
dei casi le formule seguenti sono pronte da applicare a valori RGB a 8 bit per canale, già 𝛾-corretti. Il
punto è il punto decimale.
<pre>
𝑌 = 16 + 1/256 (65.738 ⋅ 𝑅 + 129.057 ⋅ 𝐺 + 25.064 ⋅ 𝐵)
𝐶𝑏 = 128 + 1/256 (−37.945 ⋅ 𝑅 − 74.494 ⋅ 𝐺 + 112.439 ⋅ 𝐵)
𝐶𝑟 = 128 + 1/256 (112.439 ⋅ 𝑅 − 94.154 ⋅ 𝐺 − 18.285 ⋅ 𝐵)
</pre>

Anche i valori YCbCr devono essere memorizzati a 8 bit per canale. Le corrispondenti trasformazioni
inverse sono:
<pre>
𝑅 = 1/256 (298.082 ⋅ 𝑌 + 408.583 ⋅ 𝐶𝑟) − 222.921
𝐺 = 1/256 (298.082 ⋅ 𝑌 − 100.291 ⋅ 𝐶𝑏 − 208.120 ⋅ 𝐶𝑟) + 135.576
𝐵 = 1/256 (298.082 ⋅ 𝑌 + 516.412 ⋅ 𝐶𝑏) − 276.836
</pre>
Bisogna fare attenzione ai valori ottenuti come risultato delle trasformazioni. Se per caso il risultato di
una di queste equazioni è minore di 0 o maggiore di 255 bisogna saturare a 0 e a 255 rispettivamente.

## Esercizio 1

Si scriva un programma in linguaggio C++, a linea di comando, che accetti la seguente sintassi:
separa <scale> <input file> <output name stub>
Il programma accetta come secondo parametro il nome di un file in formato PPM e produce in output 3
file in formato PGM il cui nome è costituito dal terzo parametro più “_Y.pgm”, “_Cb.pgm”, “_Cr.pgm”.
Il programma deve leggere l’immagine in ingresso e salvare in output il canale Y con le dimensioni
originali e i canali Cb e Cr con larghezza e altezza divise per il valore di <scale> (arrotondando all’intero
superiore).
Per la riduzione utilizzate la media di ogni blocco <scale>×<scale>.
###Inizialmente, per semplificare il lavoro, eseguite l’esercizio supportando solo il caso <scale>=1. Poi con <scale>=2.

## Esercizio 2
Si scriva un programma in linguaggio C++, a linea di comando, che accetti la seguente sintassi:
unisci <scale> <input name stub> <output file>
Il programma accetta come input 3 file in formato PGM il cui nome è costituito dal primo parametro più
“_Y.pgm”, “_Cb.pgm”, “_Cr.pgm” e produce in output un’immagine in formato PPM con il nome fornito
come secondo parametro.
  
Il programma deve leggere le 3 immagini in ingresso, ingrandire i canali Cb e Cr in base al parametro
<scale> ed eventualmente tagliarli (scartando a destra e in basso) alla dimensione della Y e unirle assieme
nel file di output in RGB.
