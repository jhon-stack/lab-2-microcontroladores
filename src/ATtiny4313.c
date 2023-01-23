#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <funciones.h>//Aqui estan desarrolladas y decalaras las fucniones que se van a utilizar

////////////////////////////////////////////////////////////////////////////////////////////////
//Declaracion de estados
#define idle_state  0//Estado inicial. Todo en 0.
#define lose_state  1//Estado cuando se acerta mal los inputs (regresa al estado de idle).
#define blink_1     2//Primer juego. Primero
#define blink_2     3//Segundo juego
#define blink_3     4//tercer juego
#define blink_4     5//Cuarto juego (Si se acerta, regresa a idle)
////////////////////////////////////////////////////////////////////////////////////////////////
//Delay universal en (ms)
#define manuel_delay 2500
//Delays's decrementales (ms) 
#define DELAY_2     2000
#define DELAY_1p8   1800
#define DELAY_1p6   1600
#define DELAY_1p4   1400
////////////////////////////////////////////////////////////////////////////////////////////////
//Declaracion de funciones
//Maquina de Estados finita
void fsm();
//Esta funcion hace que los leds parpadeen 3 veces
void blink_error();
//Esta funcion es la que genera parpadeos aleatorios para cada LED
void blink_random();
//esta funcion es la incial despues del estado de Idle (son dos veces)
void blink_init();
//Funcion delay utilizando los registros del timer
void delay_timer(int n);
////////////////////////////////////////////////////////////////////////////////////////////////
//Variables de Estado y variables globales:

//Necesarias para el proximo estado
int state, next_state;
//Pulsacion de botones
int bt_1,bt_2,bt_3,bt_4;
//buffers para la funcion blink_random
int buff = 0;
////////////////////////////////////////////////////////////////////////////////////////////////
//interrupciones
ISR (INT1_vect){    
  if (PINB&0b10000000)
  {
    //!((PINB) & (1 << 1));
    next_state = state;
  }
  
  
}



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
    DDRB = 0xF; // I/O para los LEDS [0-3-> Output]
    //Entradas
    DDRB &= ~((1 << 7) | (1 << 6) | (1 << 4) | (1 << 5));
    PORTB = 0x00;//Incializamos el puerto
    //Interrupciones configuracion para el timer
    TCCR0A=0x00;   //Modo normnal del timer 
    TCCR0B=0x00;
    TCCR0B |= (1<<CS00)|(1<<CS02); //prescaler en 1024
    TCNT0=0;//Inicializar en 0

    // Interrupciones 
    GIMSK |= (1<<INT0);  //  Habilitando interrupciones INT0 e INT1
    GIMSK |= (1<<INT1);
    MCUCR |= (1<<ISC00)|(1<<ISC01); 
    sei();
    while(1){

        //Poner aqui la maquina de Estados
        state = next_state;
        //Asignar botones a las entradas 
       
        fsm();
        


    }//Termina el while(1)


}//Aqui termina la funcion main(void)

void delay_timer(int n){
  
  unsigned int i=0; 

  
  volatile int valor = n*2193;
  if (n == 0)
  {
    valor = 1097;
  }
  TIFR|=(1<<TOV0);  
   while(i<=valor)
   { 
      while((TIFR & (1 << TOV0) )==0);  //Se espera a que contador llegue de 0 a 255
      TIFR|=(1<<TOV0);                  // Se limpia la bandera al finalizar cuenta
      i++;                              // Se incrementa en 1
   }
  
  
}

void blink_init(){
    //Apagar LEDS
    PORTB = 0x00;
    _delay_ms(5000);
    //Encender LEDS
    PORTB = 0b00001111;
    _delay_ms(5000);
    //Apagar 
    PORTB = 0x00;
    _delay_ms(5000);
    //Encender LEDS
    PORTB = 0xF;
    _delay_ms(5000);
    //Apagar  
    PORTB = 0x00;
    _delay_ms(5000);
    //Encender LEDS
    PORTB = 0xF;
    _delay_ms(5000);
    //Apagar  
}//Aqui termina la funcion blink_init

void blink_error(){
    //Apagar LEDS
    PORTB = 0x00;
    _delay_ms(5000);
    //Encender LEDS
    PORTB = 0xF;
    _delay_ms(5000);
    //Apagar 
    PORTB = 0x00;
    _delay_ms(5000);
    //Encender LEDS
    PORTB = 0xF;
    _delay_ms(5000);
    //Apagar  
    PORTB = 0x00;
    _delay_ms(5000);
    //Encender LEDS
    PORTB = 0xF;
    _delay_ms(5000);
    //Apagar 
    PORTB = 0x00;
    _delay_ms(5000);
    //Encender LEDS
    PORTB = 0xF;
    _delay_ms(5000);
    //Apagar 
    PORTB = 0x00;
    _delay_ms(5000);
}//Aqui termina blink_error

void blink_random(){
    //uso del buffer
    if (buff < 50)
    {
        buff++;
    }
    else
    {
        buff = 0;
    }
    if ((0<=buff) & (buff<10))
    {
        //Al final esto no estan aleatorio
        //Encendemos el LED 1,3,2 y 4
        PORTB = 0x1;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x4;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x2;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x8;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
    }

    if ((10<=buff) & (buff<19))
    {
        //Encendemos el LED 4,2,1 y 3
        PORTB = 0x8;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x2;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x1;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x4;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
    }
    if ((20<=buff) & (buff<29))   
    {
        //Encendemos el LED 3,3,4 y 3
        PORTB = 0x4;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x4;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x8;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x4;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
    }

    if ((30<=buff) & (buff<39))   
    {
        //Encendemos el LED 4,4,1 y 1
        PORTB = 0x8;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x8;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x1;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x1;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
    }

    if ((40<=buff) & (buff<49))   
    {
        //Encendemos el LED 4,3,2 y 1
        PORTB = 0x8;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x4;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x2;
        _delay_ms(5000);
        PORTB = 0x0;
        _delay_ms(5000);
        PORTB = 0x1;
        _delay_ms(5000);
        PORTB = 0x0;
        //_delay_ms(5000);
    }



}//Aqui termina blink_random
void fsm(){

    switch (state)
        {
        case (idle_state):
       /* bt_1 = PINB & (1 << PINB4);
        bt_2 = PINB & (1 << PINB5);
        bt_3 = PINB & (1 << PINB6);
        bt_4 = PINB & (1 << PINB7);*/

            //checamos si se ha pulsado alguno de los 4 botones
            //if ((!(PINB & (1 << 1)) == 1) |  (!(PINB & (1 << 2)) == 1) | (!(PINB & (1 << 3)) == 1) | (!(PINB & (1 << 4)) == 1))
            if ((PINB&00010000) | (PINB&00100000) | (PINB&01000000) | (PINB&10000000))
            {
                //generar parpadeo inical (2 veces)-> llamo a 
                blink_init();
                //Reinicio el buffer de los botones
                
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
            blink_random();
            _delay_ms(DELAY_2);
            //Si los botones son correctos salte al siguiente estado
            if ((PINB&00010000) & (PINB&01000000) & (PINB&00100000) & (PINB&10000000) )
            {
                next_state = blink_2;
            }
            else
            {
                next_state = lose_state;
            }
            
            
            // Si no, vaya al estado de lose
        ////////////////////////////////////////////////////////////////////
        case(blink_2):
            //Ecender los leds (tienen que durar 1.8 segundos)
            blink_random();
            _delay_ms(DELAY_1p8);
            //Si los botones son correctos salte al siguiente estado
            if ((PINB&00010000) & (PINB&01000000) & (PINB&00100000) & (PINB&10000000) )
            {
                next_state = blink_3;
            }
            else
            {
                next_state = lose_state;
            }
            
            // Si no, continue en este
        ////////////////////////////////////////////////////////////////////
        case(blink_3):
            //Ecender los leds (tienen que durar 1.6 segundos)
            blink_random();
            _delay_ms(DELAY_1p6);
            //Si los botones son correctos salte al siguiente estado
            if ((PINB&00010000) & ~(PINB&01000000) & ~(PINB&00100000) & (PINB&10000000) )
            {
                next_state = blink_4;
            }
            else
            {
                next_state = lose_state;
            }
            
            // Si no, continue en este
        ////////////////////////////////////////////////////////////////////
        case(blink_4):
            //Ecender los leds (tienen que durar 1.4 segundos)
            blink_random();
            _delay_ms(DELAY_1p4);
            //Si los botones son correctos salte al siguiente estado
            if ((PINB&00010000) & (PINB&01000000) & (PINB&00100000) & (PINB&10000000) )
            {
                next_state = idle_state;
            }
            else
            {
                next_state = lose_state;
            }
            
            // Si no, continue en este
        ////////////////////////////////////////////////////////////////////    
        case(lose_state):
            //Ecender los leds (3 veces)-->llamo a blink_error()
            blink_error();
            //Paso al estado de idle
            next_state = idle_state;

        //default:
           break;
        }
    
}//Aqui termina la maquina de Estados

