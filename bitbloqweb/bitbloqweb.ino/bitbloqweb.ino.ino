/***   Included libraries  ***/


/***   Global variables and function definition  ***/
int led_0 = 7;
int led_1 = 6;
int led_2 = 5;
int led_3 = 4;

void detenido() {
    digitalWrite(led_0, LOW);
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, LOW);
}
void adelante() {
    digitalWrite(led_0, LOW);
    digitalWrite(led_1, HIGH);
    digitalWrite(led_2, HIGH);
    digitalWrite(led_3, LOW);
}
void atras() {
    digitalWrite(led_0, HIGH);
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, HIGH);
}
void izquierda() {
    digitalWrite(led_0, LOW);
    digitalWrite(led_1, HIGH);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, HIGH);
}
void derecha() {
    digitalWrite(led_0, HIGH);
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, HIGH);
    digitalWrite(led_3, LOW);
}

/***   Setup  ***/
void setup() {
    pinMode(led_0, OUTPUT);
    pinMode(led_1, OUTPUT);
    pinMode(led_2, OUTPUT);
    pinMode(led_3, OUTPUT);
}

/***   Loop  ***/
void loop() {
    /*
    Movimientos del robot
    */
    adelante();
    delay(2000);
    izquierda();
    delay(2000);
    adelante();
    delay(2000);
    derecha();
    delay(2000);
}
