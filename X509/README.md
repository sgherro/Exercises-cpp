#X.509 Example (ITA version)

X.509 è uno standard proposto dall'Unione internazionale delle telecomunicazioni (ITU-T), usato per
definire il formato dei certificati a chiave pubblica (PKC) e dei certificati di attributo (CA). I certificati
vengono utilizzati per la validazione dell'identità e la trasmissione di dati criptati che solo il possessore
(persona, organizzazione o applicazione) di uno specifico certificato è in grado di decifrare e leggere. Questi
vengono rilasciati dalle Certificate Authority (CA), un soggetto terzo fidato che assicura la corrispondenza
di una chiave pubblica a una determinata identità. Uno degli usi più diffusi di X.509 è nell'ambito Internet,
il certificato SSL/TLS viene usato nell'omonimo protocollo per criptare le comunicazioni tra un sito web e
il nostro browser.

Questi certificati, quando vengono salvati su file vengono tipicamente memorizzati in forma testuale per
poter essere concatenati facilmente e inviati anche tramite e-mail. Per questa codifica si utilizzano diversi
standard e in particolare la definizione dei dati viene fatta con una sintassi Abstract Syntax Notation One
(ASN.1), un meccanismo di definizione di strutture dati generiche estremamente flessibile. Questa
definizione è poi seguita dalla codifica binaria chiamata Distinguished Encoding Rules (DER), e
dall’encoding in Base64.

I dati codificati in questo modo sono composti da una struttura ricorsiva in cui ogni valore è costituito da
tre parti:
<pre>
1. Identifier octets: 1 o più byte che permettono di capire il tipo e le caratteristiche del valore
2. Length octets: 1 o più byte che permettono di capire la lunghezza del contenuto
3. Contents octets: il contenuto vero e proprio
</pre>
Negli Identifier octets:
<pre>
1. i primi due bit definiscono la Class del valore: 00 = Universal, 01 = Application, 10 = Context-
specific, 11 = Private. La classe Universal è quella definita nello standard e contiene tutti i tipi di
dato classici, come interi, float, stringhe, sequenze (liste), set (dizionari), ecc… Le altre sono
specifiche per diverse applicazioni.
2. Segue poi un bit fondamentale P/C, ovvero 0 = Primitive, 1 = Constructed. Se il tipo è Primitive
significa che i dati sono contenuto formattato in qualche modo noto, se invece è Constructed,
significa che il valore è un contenitore di altri valori definiti secondo questo standard
3. cinque bit poi sono il Tag, ovvero l’etichetta che caratterizza il tipo di dato.
</pre>
In questo modo si potrebbero avere solo 25 diversi tag, perciò il tag 31 (in binario 11111) indica il caso
particolare di un tag lungo. In questo caso il bit più significativo a 1 di un byte seguente indica che
seguiranno altri bit, mentre se vale 0 è l’ultimo dell’estensione. Il valore è ottenuto concatenando i bit
rimanenti. 
Ad esempio gli identifier octets 1F 83 62 sono in binario

0001.1111 1000.0011 0110.0010

che quindi vanno interpretati come

classe=00, p/c=0, tag=11111 -> tag lungo: primo bit=1, cioè seguiranno altri

byte a questi bit 0000011, primo bit=0, non ne seguiranno altri, 1100010

Il tag complessivo è quindi 00000111100010 cioè il valore 482

Nei Length octets il meccanismo è invece diverso:
<pre>
1. se il bit più significativo vale 0, i sette bit rimanenti sono il valore della lunghezza
2. se il bit più significativo vale 1, i sette bit rimanenti sono il numero di byte che seguiranno e che
dovranno essere concatenati per formare il valore della lunghezza.
</pre>
Ad esempio il valore 25 viene codificato come 0001.1001, mentre il valore 260 viene codificato come
1000.0010 0000.0001 0000.0100, cioè 1=lunghezza lunga, 0000010=seguono due byte da concatenare,
0000000100000100=valore effettivo.

## Esercizio

Si realizzi un programma che esegua la seguente sequenza di operazioni:
1) Legge da standard input un file contenente un certificato, tipo:
<pre>
-----BEGIN CERTIFICATE-----
MIICLDCCAdKgAwIBAgIBADAKBggqhkjOPQQDAjB9MQswCQYDVQQGEwJCRTEPMA0G
A1UEChMGR251VExTMSUwIwYDVQQLExxHbnVUTFMgY2VydGlmaWNhdGUgYXV0aG9y
aXR5MQ8wDQYDVQQIEwZMZXV2ZW4xJTAjBgNVBAMTHEdudVRMUyBjZXJ0aWZpY2F0
ZSBhdXRob3JpdHkwHhcNMTEwNTIzMjAzODIxWhcNMTIxMjIyMDc0MTUxWjB9MQsw
CQYDVQQGEwJCRTEPMA0GA1UEChMGR251VExTMSUwIwYDVQQLExxHbnVUTFMgY2Vy
dGlmaWNhdGUgYXV0aG9yaXR5MQ8wDQYDVQQIEwZMZXV2ZW4xJTAjBgNVBAMTHEdu
dVRMUyBjZXJ0aWZpY2F0ZSBhdXRob3JpdHkwWTATBgcqhkjOPQIBBggqhkjOPQMB
BwNCAARS2I0jiuNn14Y2sSALCX3IybqiIJUvxUpj+oNfzngvj/Niyv2394BWnW4X
uQ4RTEiywK87WRcWMGgJB5kX/t2no0MwQTAPBgNVHRMBAf8EBTADAQH/MA8GA1Ud
DwEB/wQFAwMHBgAwHQYDVR0OBBYEFPC0gf6YEr+1KLlkQAPLzB9mTigDMAoGCCqG
SM49BAMCA0gAMEUCIDGuwD1KPyG+hRf88MeyMQcqOFZD0TbVleF+UsAGQ4enAiEA
l4wOuDwKQa+upc8GftXE2C//4mKANBC6It01gUaTIpo=
-----END CERTIFICATE-----
</pre>
e ignorando quanto viene prima della riga -----BEGIN CERTIFICATE----- e dopo la riga -----END
CERTIFICATE-----, salva il testo in mezzo nel file encoded.txt

2) Decodifica il testo così estratto utilizzando le specifiche della rappresentazione in Base64 e salva l’output
nel file decoded.bin
<pre>
In esadecimale l’esempio precedente dovrebbe essere:
30 82 02 2c 30 82 01 d2 a0 03 02 01 02 02 01 00 30 0a 06 08 2a 86 48 ce 3d 04 03 02 30 7d 31
0b 30 09 06 03 55 04 06 13 02 42 45 31 0f 30 0d 06 03 55 04 0a 13 06 47 6e 75 54 4c 53 31 25
30 23 06 03 55 04 0b 13 1c 47 6e 75 54 4c 53 20 63 65 72 74 69 66 69 63 61 74 65 20 61 75 74
68 6f 72 69 74 79 31 0f 30 0d 06 03 55 04 08 13 06 4c 65 75 76 65 6e 31 25 30 23 06 03 55 04
03 13 1c 47 6e 75 54 4c 53 20 63 65 72 74 69 66 69 63 61 74 65 20 61 75 74 68 6f 72 69 74 79
30 1e 17 0d 31 31 30 35 32 33 32 30 33 38 32 31 5a 17 0d 31 32 31 32 32 32 30 37 34 31 35 31
5a 30 7d 31 0b 30 09 06 03 55 04 06 13 02 42 45 31 0f 30 0d 06 03 55 04 0a 13 06 47 6e 75 54
4c 53 31 25 30 23 06 03 55 04 0b 13 1c 47 6e 75 54 4c 53 20 63 65 72 74 69 66 69 63 61 74 65
20 61 75 74 68 6f 72 69 74 79 31 0f 30 0d 06 03 55 04 08 13 06 4c 65 75 76 65 6e 31 25 30 23
06 03 55 04 03 13 1c 47 6e 75 54 4c 53 20 63 65 72 74 69 66 69 63 61 74 65 20 61 75 74 68 6f
72 69 74 79 30 59 30 13 06 07 2a 86 48 ce 3d 02 01 06 08 2a 86 48 ce 3d 03 01 07 03 42 00 04
52 d8 8d 23 8a e3 67 d7 86 36 b1 20 0b 09 7d c8 c9 ba a2 20 95 2f c5 4a 63 fa 83 5f ce 78 2f
8f f3 62 ca fd b7 f7 80 56 9d 6e 17 b9 0e 11 4c 48 b2 c0 af 3b 59 17 16 30 68 09 07 99 17 fe
dd a7 a3 43 30 41 30 0f 06 03 55 1d 13 01 01 ff 04 05 30 03 01 01 ff 30 0f 06 03 55 1d 0f 01
01 ff 04 05 03 03 07 06 00 30 1d 06 03 55 1d 0e 04 16 04 14 f0 b4 81 fe 98 12 bf b5 28 b9 64
40 03 cb cc 1f 66 4e 28 03 30 0a 06 08 2a 86 48 ce 3d 04 03 02 03 48 00 30 45 02 20 31 ae c0
3d 4a 3f 21 be 85 17 fc f0 c7 b2 31 07 2a 38 56 43 d1 36 d5 95 e1 7e 52 c0 06 43 87 a7 02 21
00 97 8c 0e b8 3c 0a 41 af ae a5 cf 06 7e d5 c4 d8 2f ff e2 62 80 34 10 ba 22 dd 35 81 46 93
22 9a
</pre>
3) Decodifica i dati binari così estratti e ne produce una rappresentazione in memoria in una struttura che
contenga tutti i dati presenti nel file.
4) Invia su standard output la struttura con un formato in cui ogni elemento viene rappresentato come:
<class>, <p/c>, <tag>, <header length>+<content length>
<header length> è il numero di byte che compongono gli Identifier octets e i Length octets, mentre <content
length> è il valore del campo length.
e se il campo è Primitive della classe Universal con tag 19 (valori di tipo PrintableString) aggiunge
: <contenuto come stringa>
Se il campo è di tipo Constructed, i campi contenuti vanno indentati con due spazi.

Nel caso dell’esempio dovrebbe stampare
<pre>
0, 1, 16, 4+556
0, 1, 16, 4+466
2, 1, 0, 2+3
0, 0, 2, 2+1
0, 0, 2, 2+1
0, 1, 16, 2+10
0, 0, 6, 2+8
0, 1, 16, 2+125
0, 1, 17, 2+11
0, 1, 16, 2+9
0, 0, 6, 2+3
0, 0, 19, 2+2: BE
0, 1, 17, 2+15
0, 1, 16, 2+13
0, 0, 6, 2+3
0, 0, 19, 2+6: GnuTLS
0, 1, 17, 2+37
0, 1, 16, 2+35
0, 0, 6, 2+3
0, 0, 19, 2+28: GnuTLS certificate authority
0, 1, 17, 2+15
0, 1, 16, 2+13
0, 0, 6, 2+3
0, 0, 19, 2+6: Leuven
0, 1, 17, 2+37
0, 1, 16, 2+35
0, 0, 6, 2+3
0, 0, 19, 2+28: GnuTLS certificate authority
0, 1, 16, 2+30
0, 0, 23, 2+13
0, 0, 23, 2+13
0, 1, 16, 2+125
0, 1, 17, 2+11
0, 1, 16, 2+9
0, 0, 6, 2+3
0, 0, 19, 2+2: BE
0, 1, 17, 2+15
0, 1, 16, 2+13
0, 0, 6, 2+3
0, 0, 19, 2+6: GnuTLS
0, 1, 17, 2+37
0, 1, 16, 2+35
0, 0, 6, 2+3
0, 0, 19, 2+28: GnuTLS certificate authority
0, 1, 17, 2+15
0, 1, 16, 2+13
0, 0, 6, 2+3
0, 0, 19, 2+6: Leuven
0, 1, 17, 2+37
0, 1, 16, 2+35
0, 0, 6, 2+3
0, 0, 19, 2+28: GnuTLS certificate authority
0, 1, 16, 2+89
0, 1, 16, 2+19
0, 0, 6, 2+7
0, 0, 6, 2+8
0, 0, 3, 2+66
2, 1, 3, 2+67
0, 1, 16, 2+65
Esame di Laboratorio di Elaborazione di Dati Multimediali
12/06/2019
Pagina 4 di 4
0, 1, 16, 2+15
0, 0, 6, 2+3
0, 0, 1, 2+1
0, 0, 4, 2+5
0, 1, 16, 2+15
0, 0, 6, 2+3
0, 0, 1, 2+1
0, 0, 4, 2+5
0, 1, 16, 2+29
0, 0, 6, 2+3
0, 0, 4, 2+22
0, 1, 16, 2+10
0, 0, 6, 2+8
0, 0, 3, 2+72
</pre>
