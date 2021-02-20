# JSON Parser Example (ITA version)

Si deve realizzare un programma in grado di convertire immagini descritte da un formato proprietario
basato su JSON in formato PPM. Un esempio di file è il seguente:

<pre>
{
  "width" : 500,
  "height" : 500,
  "background" : "FFFFFF",
  "elements" : [
    {
      "width" : 50,
      "height" : 50,
      "x" : 100,
      "y" : 200,
      "color" : "808080"
    },
    {
      "width" : 150,
      "height" : 50,
      "x" : 100,
      "y" : 20,
      "color" : "123456"
    }
    ]
}
</pre>

Il file è composto di un oggetto JSON con quattro proprietà. Queste sono, facendo riferimento all’ordine
dell’esempio: larghezza dell’immagine, altezza dell’immagine, colore dello sfondo e un array di rettangoli
colorati (pieni). Ogni rettangolo ha larghezza, altezza, posizione orizzontale, posizione verticale e un colore.

Creare un programma che apra un file in formato JSON indicato a linea di comando come primo parametro
e lo salvi in formato PPM col nome indicato come secondo parametro:
json2ppm <input.json> <output.ppm>

I colori sono impostati con valori casuali, giusto per rendere l'idea della visualizzazione.

### Note

Per la scrittura del parser JSON sono sufficienti le competenze di C++ fornite durante il corso, ma scrivere
una libreria completa ed esaustiva è abbastanza complesso. Pertanto il suggerimento per la risoluzione è
di non scrivere un parser in grado di digerire ogni possibile file JSON, ma di farlo innanzitutto per gestire
il caso specifico.
Partite con l’esempio proposto. Migliorate il parser in modo che non richieda che i parametri vengano
forniti nell’ordine indicato. Aggiungete flessibilità in modo che se incontra un elemento non noto non abbia
problemi, e che poi sia robusto alla presenza di diverse combinazioni di whitespace attorno agli elementi.
Inizialmente si assuma che i rettangoli indicati nell’array elements siano sempre interamente contenuti nel
rettangolo iniziale. Volendo complicare l’esercizio, fare in modo che se una parte non è contenuta, questa
semplicemente non venga utilizzata.
