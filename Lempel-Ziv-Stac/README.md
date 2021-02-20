# Lempel Ziv Stac Example (ITA version)

L’algoritmo Lempel–Ziv–Stac (LZS, Stac compression, Stacker LZS) è un algoritmo di compressione
lossless che usa un classico LZSS e una codifica di posizione (offset) e lunghezza (length) a lunghezza
variabile. L’algoritmo era stato sviluppato in origine dalla Stac Electronics per la compressione di nastri,
poi diventato parte integrante del software Stacker, venduto per tenere i dati compressi su hard disk,
raddoppiandone circa la capacità. È poi stato adottato in diversi protocolli di rete.
Trovate una descrizione dell’algoritmo nel file “LZS Wikipedia.pdf” e lo standard ufficiale e rigoroso, ma
meno chiaro, nel file “ANSI X3.241-1994.pdf”.

## Esercizio

Il programma si dovrà chiamare lzs.exe, accetta 1 solo parametro, ovvero il nome del file di input. Il
programma deve aprire il file di input e creare il file di output compresso con lo stesso nome e “.lzs”
aggiunto in fondo. Eseguendo “lzs test.txt”, verrà creato il file “test.txt.lzs” compresso.

La compressione LZS deve cercare la più lunga occorrenza del lookahead buffer nel dizionario, utilizzando
anche l’opzione di avere lunghezze maggiori dell’offset. Facendo riferimento al file “test.txt”, che
contiene:

abacababaaaaaaxca

dovrebbe codificare
<pre>
a – letterale – 0 01100001
b – letterale – 0 01100010
a – letterale (perché l’occorrenza più lunga è lunga un solo byte) – 0 01100001
c – letterale – 0 01100011
aba – offset: 4 – length: 3 – 1 10000100 01
ba – offset: 2 – length: 2 – 1 10000010 00
aaaaa – offset: 1 – length: 5 – 1 10000001 1100
x – letterale – 0 01111000
ca – offset: 12 – length: 2 – 1 10001100 00
[end marker] – 1 10000000
[padding] – 0000
</pre>

in esadecimale si ottiene quindi

30 98 8C 26 3C 23 82 30 38 78 C6 18 00
