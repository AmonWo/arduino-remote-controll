# Pin Belegung

## H-Brücke Gas

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

| H-Brücke Output<br/>Links -> Rechts | Arduino   |
|-------------------------------------|-----------|
| 1                                   | Motor +   |
| 2                                   | Motor Gnd |
| 3                                   | 12V       |
| 4                                   | Gnd       |

## Arduino

| Arduino | Input                          | 
|---------|--------------------------------|
| 0       |                                |            
| 1       |                                |            
| 2       |                                |            
| ~3      | Fernsteuerung Empfänger Signal |            
| 4       |                                |            
| ~5      | RPWM H-Brücke 2                |
| ~6      | LPWM H-Brücke 2                |
| 7       |                                |            
| 8       |                                |            
| ~9      | LPWM H-Brücke 1                |            
| ~10     | RPWM H-Brücke 1                |            
| ~11     |                                |            
| 12      |                                |            
| 13      |                                |            
| A0      | Poti Gas Motor                 |            
| A1      | Poti Lenkmotor                 |            