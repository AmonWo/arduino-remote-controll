# Pin Belegung

## H-Brücke Gas Eingang
| H-Brücke | Arduino | Versorgung |
|----------|---------|------------|
| 1        | ~11     |            |
| 2        | ~3      |            |
| 3        |         | 5V         |
| 4        |         | 5V         |
| 5        |         ||
| 6        |         ||
| 7        |         | 5V         |
| 8        |         | GND        |


## H-Brücke Lenkung Eingang
| H-Brücke | Arduino | Versorgung |
|----------|---------|------------|
| 1        | ~10     |            |
| 2        | ~9      |            |
| 3        |         | 5V         |
| 4        |         | 5V         |
| 5        |         ||
| 6        |         ||
| 7        |         | 5V         |
| 8        |         | GND        |


## Arduino

| Arduino | Input                          | 
|---------|--------------------------------|
| 0       |                                |            
| 1       |                                |            
| 2       |                                |            
| ~3      | Output an H-Brücke Gas Vorwärts|            
| 4       |                                |            
| ~5      | RC Inpt Lenkung                |
| ~6      | RC Input Gas                   |
| 7       |                                |            
| 8       |                                |            
| ~9      | Output an H-Brücke Lenkung(2)  |            
| ~10     | Output an H-Brücke Lenkung(1)  |            
| ~11     | Output an H-Brücke Gas Rückwärts|            
| 12      |                                |            
| 13      |                                |            
| A1      | Poti Gas Motor                 |            
| A2      | Poti Lenkmotor                 |            
