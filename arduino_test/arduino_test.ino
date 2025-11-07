/**
 * @file arduino_test.ino
 * @brief Código Arduino para probar el Decodificador PRT-7
 * 
 * Este sketch envía una secuencia de tramas PRT-7 por el puerto serial
 * para probar el decodificador. El mensaje oculto es "HOLA MUNDO".
 * 
 * Configuración:
 * - Baud Rate: 9600
 * - Line Ending: Newline
 */

// Array con las tramas a enviar
const char* tramas[] = {
  "L,H",      // H -> H
  "L,O",      // O -> O
  "L,L",      // L -> L
  "M,2",      // Rotar +2
  "L,C",      // C con rotación +2 -> A (C-2=A)
  "L, ",      // Espacio
  "L,O",      // O con rotación +2 -> M (O-2=M)  
  "M,-2",     // Rotar -2 (volver al inicio)
  "L,U",      // U -> U
  "L,N",      // N -> N
  "L,D",      // D -> D
  "L,O"       // O -> O
};

const int numTramas = sizeof(tramas) / sizeof(tramas[0]);
int tramaActual = 0;

void setup() {
  // Inicializar comunicación serial a 9600 baud
  Serial.begin(9600);
  
  // Esperar a que se establezca la conexión
  delay(2000);
  
  Serial.println("Arduino PRT-7 - Iniciando transmisión...");
  delay(1000);
}

void loop() {
  if (tramaActual < numTramas) {
    // Enviar la trama actual
    Serial.println(tramas[tramaActual]);
    
    // Avanzar a la siguiente trama
    tramaActual++;
    
    // Esperar 1 segundo entre tramas
    delay(1000);
  } else {
    // Todas las tramas enviadas, esperar
    if (tramaActual == numTramas) {
      Serial.println("FIN");
      tramaActual++;  // Para no seguir enviando "FIN"
    }
    delay(5000);
  }
}

/**
 * NOTAS SOBRE EL MENSAJE:
 * 
 * Secuencia de decodificación:
 * L,H -> H (sin rotación)
 * L,O -> O (sin rotación)
 * L,L -> L (sin rotación)
 * M,2 -> Rotar +2 posiciones
 * L,C -> C con rotación +2 = A (porque C está 2 posiciones adelante de A)
 * L,  -> Espacio (no se mapea)
 * L,O -> O con rotación +2 = M 
 * M,-2 -> Volver rotación 0
 * L,U -> U (sin rotación)
 * L,N -> N (sin rotación)
 * L,D -> D (sin rotación)
 * L,O -> O (sin rotación)
 * 
 * Mensaje final: "HOLA MUNDO"
 * 
 * INSTRUCCIONES DE USO:
 * 1. Cargar este código en un Arduino
 * 2. Conectar el Arduino por USB
 * 3. Ejecutar el decodificador PRT-7 en la computadora
 * 4. Especificar el puerto correcto (ej. COM3 o /dev/ttyUSB0)
 * 5. El decodificador mostrará "HOLA MUNDO"
 */
