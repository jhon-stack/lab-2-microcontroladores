#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <funciones.h>//Aqui estan desarrolladas y decalaras las fucniones que se van a utilizar
//Declaracion de estados
#define idle_state  0//Estado inicial. Todo en 0.
#define lose_state  1//Estado cuando se acerta mal los inputs (regresa al estado de idle).
#define blink_1     2//Primer juego. Primero
#define blink_2     3//Segundo juego
#define blink_3     4//tercer juego
#define blink_4     5//Cuarto juego (Si se acerta, regresa a idle)

//Delay universal en ms
#define DELAY 1000


//Variables de Estado y variables globales
//Necesarias para el proximo estado
int state, next_state;
//Pulsacion de botones
int bt_1,bt_2,bt_3,bt_4;

int main(void){

    
    //Configuracion de los registros.

    //declaraciones de registros/pines/interrupciones/etc
    //DDRx: 1 para salida y 0 para entrada.
    /*
    Vamos a tener 4 botones, es decir y entradas e igual
    para las salidas las cuales se conectan directamente al lED
    respectivo. Tomar en cuenta que funcionara mediante interrupciones
    Por lo tanto las interrupciones seran en estas entradas de botones 
    asignadas a subrutinas.
    */
    //Entradas y Salidas derecha.Usar el B
    DDRB = 0xF // I/O para los LEDS [0-3-> Output]

    void sei()
    while(true){

        //Poner aqui la maquina de Estados
        state = next_state;
        fsm();
        


    }


}
void blink_init(){
    //Apagar LEDS
    PORTB = 0x00;
    _delay_ms(DELAY);
    //Encender LEDS
    PORTB = 0xF;
    _delay_ms(DELAY);
    //Apagar   
}//Aqui termina la funcion blink_init



void fsm(){

    switch (state)
        {
        case (idle_state):
            //checamos si se ha pulsado alguno de los 4 botones
            if ((bt_1 == 1) |  (bt_1 == 2) | (bt_1 == 3) | (bt_1 == 4) |)
            {
                //generar parpadeo inical (2 veces)

                //Pase al siguiente estado
                next_state = blink_1;
            }
            else
            {
                //Nos quedamos en el mismo estado (idle_state)
                next_state = idle_state;
            }
            
            

            break;
        ////////////////////////////////////////////////////////////////////
        case(blink_1):
            //Ecender los leds (tienen que durar 2 segundos)
            //Si los botones son correctos salte al siguiente estado
            // Si no, vaya al estado de lose
        ////////////////////////////////////////////////////////////////////
        case(blink_2):
            //Ecender los leds (tienen que durar 1.8 segundos)
            //Si los botones son correctos salte al siguiente estado
            // Si no, continue en este
        ////////////////////////////////////////////////////////////////////
        case(blink_3):
            //Ecender los leds (tienen que durar 1.8 segundos)
            //Si los botones son correctos salte al siguiente estado
            // Si no, continue en este
        ////////////////////////////////////////////////////////////////////
        case(blink_4):
            //Ecender los leds (tienen que durar 1.6 segundos)
            //Si los botones son correctos salte al siguiente estado
            // Si no, continue en este
        ////////////////////////////////////////////////////////////////////    
        case(lose_state):
            //Ecender los leds (3 veces)

            //next_state = idle_state

        //default:
          //  break;
        }
    
}//Aqui termina la maquina de Estados

