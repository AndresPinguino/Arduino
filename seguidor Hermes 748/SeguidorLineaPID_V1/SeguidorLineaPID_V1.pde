/*--------------------------------------------------------------------------------------------------------------
Author:  Ing. Leonardo Javier Alvarez
Date: 2016-05-01
Description:

Programa de control de un robot seguidor de lineas
Utiliza la libreria modificada QTRSensors para Pinguino

############## Para P32 ####################

Copiar QTRSensors.pdl32 en la siguiente ubicacion

C:\pinguino-11\p32\pdl\

Copiar libreria ( QTRSensors.c y QTRSensors.h )

C:\pinguino-11\p32\include\pinguino\libraries

############## Para P8 ####################

Copiar QTRSensors.pdl en la siguiente ubicacion

C:\pinguino-11\p8\pdl\

Copiar libreria ( QTRSensors.c y QTRSensors.h )

C:\pinguino-11\p8\include\pinguino\libraries

--------------------------------------------------------------------------------------------------------------*/
/**********************************************************************
                                        SECCION DE DEFINICIONES GLOBALES
**********************************************************************/
// Sensores
#define NUMERO_SENSORES 5    // Defino la cantidad de sensores que se utilizaran
#define LINEA_BLANCA 1            // Defino el color de la linea blanca
#define LINEA_NEGRA 0            // Defino el color de la linea negra
#define TIME_OUT     2000         // Defino el tiempo maximo de espera

// Motores
#define PWM_FREQ     20000    // Defino la frecuencia de PWM = 20KHz

// Boton de comienzo y parada del robot
#define GO     5    // Defino boton conectado al pin RB7
/**********************************************************************
                                           SECCION DE VARIABLES GLOBALES
**********************************************************************/
// Defino un arreglo(vector) de los pines conectados a los sensores
unsigned char pines[NUMERO_SENSORES]={12,11,10,9,8};     // Pines RB0, RB1, RB2, RB3, RB4

// Defino los valores de calibracion
// OJO!! Debo cargar tantos valores como NUMERO_SENSORES !!!
// Agrego los valores de calibración obtenidos con el programa de prueba de sensores!!!
unsigned int valores_calibracion_maximos[NUMERO_SENSORES]={475,381,497,412,412};
unsigned int valores_calibracion_minimos[NUMERO_SENSORES]={0,0,0,0,0};

// Defino el vector donde voy a almacenar los valores que obtenga de los sensores!!
unsigned int valores_sensores[NUMERO_SENSORES];

unsigned char flag_start = 0;    // Variable auxiliar que retiene el estado de comienzo/parada

/**********************************************************************
                                        SECCION DE FUNCIONES DE PROGRAMA
**********************************************************************/
/******************************************************************************
* void comenzar(void)
* Descripcion: Funcion encargada de verificar el estado del pulsador de inicio de carreera.
*                    Una vez que el pulsador es liberado el robot comienza automaticamente a correr!!
* Parametros: Ninguno
* Retorna:
* Cambios:
*         - Creada 01/04/2016
******************************************************************************/
void comenzar(){ 

    unsigned int j = 0;
    unsigned int k = 0;
    int estado_boton = 0;
    
    while(!flag_start){
        estado_boton = digitalRead(GO);
        if(estado_boton != 1){
            // Si estoy aca es porque se presiono el boton de inicio
            // Delay antirebote
            for(j = 0; j<65000;j++)
                for(k=0;k<10;k++);
            // Vuelvo a verificar el estado del boton de inicio
            estado_boton = digitalRead(GO);
            if(estado_boton != 1){
                // Cambio el estado de la bandera de inicio
                flag_start = TRUE;
                // Mientras el boton de inicio este presionado no hago nada
                // Es decir, espero a que dejen de presionar el boton
                while(estado_boton != 1)
                    estado_boton = digitalRead(GO);
            }                
        }    
    }
}
/******************************************************************************
* void detener(void)
* Descripcion: Funcion encargada de verificar el estado del pulsador de fin de la carrera. Si el mismo fue
*                   presionado detiene los motores. El sistema queda re-armado para ser nuevamente iniciado.
* Parametros: Ninguno
* Retorna:
* Cambios:
*         - Creada 01/05/2016
******************************************************************************/
void detener(){

    unsigned int j = 0;
    unsigned int k = 0;
    
    int estado_boton = 0;
    // Verifico si se presiono el boton para detener el combate
    estado_boton = digitalRead(GO);
    if(estado_boton != 1){
        // Si estoy aca es porque se presiono el boton de inicio
        // Delay antirebote
        for(j = 0; j<65000;j++)
            for(k=0;k<10;k++);            
        // Vuelvo a verificar el estado del boton de inicio
        estado_boton = digitalRead(GO);
        if(estado_boton != 1){
            // Cambio el estado de la bandera de inicio
            flag_start = FALSE;
            // Mientras el boton de inicio este presionado no hago nada
            // Es decir, espero a que dejen de presionar el boton
            while(estado_boton != 1)
                estado_boton = digitalRead(GO);
            // delay antirebote por las dudas
            for(j = 0; j<65000;j++)
                for(k=0;k<10;k++);
            // Detengo los motores!!
            PWM.setPercentDutyCycle(PWM1,0);
            PWM.setPercentDutyCycle(PWM2,0);
        }                
    }    
}

/******************************************************************************
* void setup(void)
* Descripcion: Funcion encargada del codigo de inicializacion del sistema
* Parametros: Ninguno
* Retorna:
* Cambios:
*         - Creada 01/05/2016
******************************************************************************/
void setup() {
    // Codigo de inicializacion

    unsigned char i=0;    // variable auxiliar
    
    ///////////////// INICIALIZO MOTORES /////////////////////
    // Fijo la frecuencia de PWM (Modulacion de ancho de pulso)
    PWM.setFrequency(PWM_FREQ);
    // Fijo el ciclo de trabajo de ambos motores a cero! APAGADOS = 0%
    PWM.setPercentDutyCycle(PWM2,0);
    PWM.setPercentDutyCycle(PWM1,0);
    
    /////////////// INICIALIZO SENSORES /////////////////////
    // Inicializo los sensores
    // Llamo a la funcion que inicializa los sensores
    // Le tengo que pasar los siguientes parametros:
    // 1- Pines a los que estan conectados los sensores
    // 2- El numero de sensores conectados
    // 3- Si los diodos emisores tienen algun pin de control (QTR_NO_EMITTER_PIN)
    // 4- Tiempo de espera maximo por sensor.
    QTRSensors.init(pines,NUMERO_SENSORES,QTR_NO_EMITTER_PIN,TIME_OUT);
    
    // Cargo los valores de calibración obtenidos con el programa de prueba de sensores!!!
    for(i=0;i<NUMERO_SENSORES;i++){
         calibratedMinimumOn[i] =  valores_calibracion_minimos[i];
         calibratedMaximumOn[i] = valores_calibracion_maximos[i];
    }
    
    // Normalizo los sensores al rango de valores de 0 a 1000
    // Le tengo que pasar los siguientes parametros a la funcion:
    // 1- Un vector donde voy a almacenar los valores leidos y normalizados de los sensores (el vector valores_sensores)
    // 2- Un parametro que indica que los diodos de los sensores estan siempre encendidos (QTR_EMITTERS_ON)
    QTRSensors.readCalibrated(valores_sensores,QTR_EMITTERS_ON);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////// ESQUEMA SEGUIDOR //////////////////////////////////////////
    /////////
    /////////                                                IZQUIERDA                          DERECHA
    /////////    5 sensores                                S0      S1        S2       S3       S4
    /////////    rango de valores                        0      1000    2000    3000    4000
    /////////
    /////////
    /////////     2 motores                         MOTOR1 (PWM2)               MOTOR2(PWM1)
    /////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    ///////////////// BOTON DE INICIO //////////////////////////
    // Fijo al pin GO (RB7), conectado al boton, como entrada digital
    pinMode(GO,INPUT);
    
    ////////////////// FIN DE INICIALIZACION ////////////////////////  
}

/******************************************************************************
* void loop(void)
* Descripcion: Funcion encargada ejecutar repetidamente el codigo principal del sistema
* Parametros: Ninguno
* Retorna:
* Cambios:
*         - Creada 01/05/2016
******************************************************************************/
void loop() {
    // Codigo principal que se ejecutara repetidamente
    unsigned char i = 0;
    unsigned int posicion=0;    // Variable que va a retener el valor de la posicion de la linea
    int e_posicion = 0;            // Variable que almacena el error en la posicion
    int e_posicion_anterior = 0;    // Variable que almacena el error en la posicion pasada
    int derivativo = 0;                  // Variable que retiene el ezfuerso de control derivativo (derivada - D)
    int integral = 0;                     // Variable que retiene el ezferzo de control integral (integral- I)
    int dif_potencia = 0;              // Variable que retiene la diferencia de potencia necesaria aplicar para controlar el seguidor
    int potencia_max = 50;         // Maxima potencia de salida de los motores. Expresado en porcentaje.
    
    // Aguardo a que se presione el boton de inicio/parada de la competencia
    comenzar();
    
    // Loop prinipal del programa. Aca se desarrollan todas las acciones de sensado y control
    while(flag_start){
    
        // Leo la posicion actual de la linea
        // Llamo a la funcion que devuelve la posicion de la linea
        // Debo pasarle a la funcion como parametros:
        // 1- Un vector donde almacenar los valores obtenidos de los sensores.
        // 2- El parametro que indica que los diodos siempre estan encendidos (QTR_EMITTERS_ON)
        // 3- El color de linea que vamos a seguir.
        posicion = QTRSensors.readLine(valores_sensores,QTR_EMITTERS_ON,LINEA_BLANCA);  
           
        // Termino proporcional
        // Obtengo la posicion actual de la linea y calculo el error. Es decir, cuan lejos estoy del valor 
        // que yo quiero alcanzar. En definitiva, el valor ideal donde debe estar la linea. En nuestro
        // caso, como tenemos 5 sensores la posicion puede variar de 0 a 4000, por lo tanto, si queremos
        // la linea en el centro de la pista nuestro valor de referencia (que queremos alcanzar) seria 2000.
        // El valor 2000 representa que la linea estara en el centro de la barra de sensores.
        
        // A la posición leida debo restarle la posición que queremos alcanzar. El resultado es el error
        // entre lo deseado y lo que realmente esta pasando
        e_posicion = posicion - 2000;
  
        // Termino derivativo
        // Con la resta posicion actual y la posicion pasada calculo el termino derivativo.
        // La acción derivativa da una respuesta proporcional a la derivada del error (velocidad de cambio del error).
        // Esto quiere decir que el este termino va a tratar de "corregir" anticipando (estimando la tendenia a futuro) 
        // el error en el proximo ciclo.
        // Con este termino se logra que disminuyan las oscilaciones sobre la linea. 
        // Si se calcula mal, el sistema se vuelve inestable!!
        derivativo = e_posicion - e_posicion_anterior;
    
        // Termino integral
        // La acción integral da una respuesta proporcional a la integral del error.
        // Es la sumartoria del error actual mas el error acumulado a lo largo del tiempo.
        // Es como el historial del error y permite al sistema de control "ver" como fue
        // creciendo/disminuyendo el error en el pasado para tomar acciones de corrección en el presente.
        // Tiene la desventaja de disminuir el tiempo de respuesta.
        // La ventaja de utilizarlo es que logran un error cero en regimen estacionario
        // Y tambien es mas facil lograr que el sistema se estabilice
        integral = integral + e_posicion;
        
        // Limitamos el termino integral
        // En ciertas ocasiones la acumulacion de error puede diverger o tornar muy lento el sistema
        // Se lo conoce como anti-windup
        if(integral > 12000){
            integral = 12000;        
        }else{ 
            if(integral < -12000){ 
                integral = -12000;
            }
        }
                
        // Almacenamos la posicion actual como ya pasada, para poder tenerla en 
        // cuenta en el proximo ciclo de calculo!!
        e_posicion_anterior = e_posicion;
    
        // Esfuerzo de Control
        // Ahora calculamos la diferencia del valor de potencia de los motores, M1-M2
        // Esto nos dara el esfuerzo de control necesario para que el robot siga la linea!!!
        // Si se trata de un número positivo -> deberemos girar a la derecha. 
        // Si se trata de un número negativo -> deberemos girar a la izquierda
        // Cuanto mas grande la diferencia, mayor sera el giro que deberemos
        // realizar para corregir la posicion 
        // Las constantes del control PID son las obtenidas en las pruebas
        // P = 1/8
        // I = 0 -> Porque queremos que el seguidor actue lo mas rapido posible!!
        // D = 1/3
        dif_potencia = e_posicion/8 + derivativo*3;
     
        // Ahora debo traducir la diferencia de potencia entre los motores al valor real de 
        // porcentaje de potencia de cada motor!!
        // Limitamos la diferencia de potencia al valor maximo de potencia admisible
        // En nuestro caso eso esta fijado en la variable potencia_max 

        // Si la diferencia es mayor a potencia maxima, la limito a potencia maxima!    
        if(dif_potencia > potencia_max) 
            dif_potencia = potencia_max;
        // Si la diferencia es menor negativamente que el negativo de potencia maxima,
        // limito la diferencia al negativo de la potencia maxima!  
        if(dif_potencia < -potencia_max) 
            dif_potencia = -potencia_max;

        // Activo los motores con la potencia segun corresponda.
        // Aca se tiene en cuenta que no podemos enviar potencias negativas al motor!!
        // Recuerden que si la diferencia era positiva indicaba cuanto mas rapido debe girar M1 (PMW2) que M2 (PWM1)
        // para girar a la derecha y corregir el error en la posicion!!
        // Y si por el contrario la diferencia era negativa, era indicativo de cuanto mas lento debe girar M2 (PWM1) que M1(PWM2)
        // para girar a la izquierda y corregir el error en la posicion!!
        if(dif_potencia > 0){ 
            PWM.setPercentDutyCycle(PWM2,potencia_max + dif_potencia );
            PWM.setPercentDutyCycle(PWM1,potencia_max );
        }else{ 
            PWM.setPercentDutyCycle(PWM2,potencia_max );
            PWM.setPercentDutyCycle(PWM1,potencia_max - dif_potencia );
        }
        // Finalmente verifico si se presiono el boton para detener el robot
        detener();
    }
}
