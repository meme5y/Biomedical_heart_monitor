// Projeto 1: Simulador de Medidor de Frequência Cardíaca (PPG)
// Baseado no sensor infravermelho

// Definições dos pinos
const int pinoSensorIR = 2;  // Pino do sensor IR (com interrupção)
const int pinoBuzzer = 8;    // Pino do Buzzer
const int pinoLED = 7;       // Pino do LED

// Variáveis para o cálculo do batimento
volatile int batimentos = 0; // 'volatile' é crucial para variáveis usadas em interrupções
unsigned long tempoAnterior = 0;
const unsigned long intervaloLeitura = 10000; // Lê a cada 10 segundos (10000 ms)

// Variáveis para detecção de pulso (para evitar ruído)
unsigned long ultimoPulso = 0;
const unsigned long intervaloRefratario = 300; // Tempo mínimo entre pulsos (ms)

// Função de interrupção: chamada toda vez que o pino D2 muda de estado (LOW<->HIGH)
void detectaPulso() {
  // Verifica se passou tempo suficiente desde o último pulso para evitar ruído (debounce)
  if (millis() - ultimoPulso > intervaloRefratario) {
    batimentos++;           // Incrementa a contagem de batimentos
    ultimoPulso = millis(); // Registra o momento deste pulso

    // Feedback visual e sonoro imediato
    digitalWrite(pinoLED, HIGH);
    tone(pinoBuzzer, 1000, 100); // Toca um tom de 1000Hz por 100ms
    // O LED será apagado pela lógica principal após um tempo curto
  }
}

void setup() {
  // Inicializa os pinos
  pinMode(pinoSensorIR, INPUT);
  pinMode(pinoBuzzer, OUTPUT);
  pinMode(pinoLED, OUTPUT);

  // Configura a interrupção no pino 2 (INT0)
  // A interrupção é acionada na transição de LOW para HIGH (RISING)
  attachInterrupt(digitalPinToInterrupt(pinoSensorIR), detectaPulso, RISING);

  // Inicia a comunicação serial para debug e exibição do BPM
  Serial.begin(9600);
  Serial.println("Iniciando Monitor de Frequência Cardíaca...");
  Serial.println("Coloque o dedo no sensor.");
}

void loop() {
  unsigned long tempoAtual = millis();

  // A cada 'intervaloLeitura' (10 segundos), calcula e exibe o BPM
  if (tempoAtual - tempoAnterior >= intervaloLeitura) {
    // Desabilita interrupções momentaneamente para evitar leitura inconsistente
    detachInterrupt(digitalPinToInterrupt(pinoSensorIR));

    // Calcula a Frequência Cardíaca (BPM)
    // batimentos contados em 10 segundos * 6 = BPM
    int bpm = batimentos * 6;

    // Exibe os resultados no Monitor Serial
    Serial.print("Batimentos detectados: ");
    Serial.println(batimentos);
    Serial.print("Frequência Cardíaca (BPM): ");
    Serial.println(bpm);

    // Reinicia a contagem e o tempo para a próxima leitura
    batimentos = 0;
    tempoAnterior = tempoAtual;

    // Reabilita a interrupção para a próxima leitura
    attachInterrupt(digitalPinToInterrupt(pinoSensorIR), detectaPulso, RISING);
  }

  // Apaga o LED pouco depois de acender (para não ficar aceso continuamente)
  // Se um pulso foi detectado há mais de 150ms, apaga o LED.
  if (millis() - ultimoPulso > 150 && digitalRead(pinoLED) == HIGH) {
    digitalWrite(pinoLED, LOW);
  }

  // Pequena pausa para estabilidade do sistema
  delay(10);
}
