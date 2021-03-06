//Import bibliotek
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "hd44780.h"
#include "uart.h"
#include "rfid_em4095.h"

//Zmienne do obslugi przycisku w enkoderze
#define KEY (1<<PC0)
uint16_t key_lock;

//Wartosc enkodera
unsigned int encoder = 0;


//Przerwanie sprzetowe INT0
ISR( INT0_vect ) {
    if ( !bit_is_clear( PIND, PD3 ) ) {
        encoder ++;
    } else {
        encoder --;
    }
}

//Przerwanie sprzetowe INT0
ISR( INT1_vect ) {
    if ( !bit_is_clear( PIND, PD2 ) ) {
        encoder ++;
    } else {
        encoder --;
    }
}

int main( void ) {
    //Numer zabezpieczenia
    int securityNumber = 0;

    //Wybrane liczby za pomoca enkodera
    int encoderResult[3];
    int encoderCount = 0;

    //Bufor do wypisania tablicy encoderResult
    char bufor [4];
    char buforEnc0 [4];
    char buforEnc1 [4];
    char buforEnc2 [4];

    //Zmienne do obslugi klawiatury
    int keypressed=0;
    int keyboardCount=0;
    char keyboardResult[4];
    uint8_t keyboardListener = 1;

    //Konfiguracja portow enkodera
    PORTC |= KEY;

    DDRD &= ~( 1 << PD2 );
    DDRD &= ~( 1 << PD3 );
    PORTD |= ( 1 << PD3 ) | ( 1 << PD2 );

    GICR |= ( 1 << INT0 ) | ( 1 << INT1 );
    MCUCR |= ( 1 << ISC01 ) | ( 1 << ISC11 ) | ( 1 << ISC10 );

    //Wlaczenie przerwan
    sei();

    //Inicjalizacja LCD
    LCD_Initalize();
    LCD_Home();
    LCD_Clear();
    LCD_GoTo( 0, 0 );

    _delay_ms( 10 );

    while ( 1 ) {

          //Pierwsze zabezpieczenie
          if(securityNumber == 0){

          //Jezeli przycisk enkodera nacisniety
          if( !key_lock && !(PINC & KEY ) ) {
           key_lock = 65000;

           //Pobieram wartosc i umieszczam w tablicy wybranych liczb
           encoderResult[encoderCount] = encoder;
           if(encoderCount == 0){
               itoa(encoderResult[encoderCount],buforEnc0,10);
               LCD_GoTo( 0, 1 );
               LCD_WriteText( buforEnc0 );
           } else if (encoderCount == 1){
               itoa(encoderResult[encoderCount],buforEnc1,10);
               LCD_GoTo( 3, 1);
               LCD_WriteText( buforEnc1 );
           } else if (encoderCount == 2){
               itoa(encoderResult[encoderCount],buforEnc2,10);
               LCD_GoTo( 6, 1);
               LCD_WriteText( buforEnc2 );
           }

           //Jezeli wybrano wszystkie liczby to sprawdzam czy sa poprawne
           if(encoderCount < 2){
               encoderCount = encoderCount + 1;
           }else{
               if(encoderResult[0] == 20 && encoderResult[1] == 40 && encoderResult[2] == 60){
                    //Jezeli ok to wypisuje info i przechodze dalej
                    LCD_GoTo(10,1);
                    LCD_WriteText("OPEN");
                    _delay_ms( 1000 );

                    securityNumber++;
                    LCD_Clear();
                    //Jezeli NOK to nalezy wprowadzic od nowa
               }else{
                    LCD_GoTo(10,1);
                    LCD_WriteText("WRONG");
                    encoderCount = 0;
                    _delay_ms( 1000 );
                    LCD_Clear();
               }
           }

          } else if( key_lock && (PINC & KEY ) ) key_lock++;

        //Ograniczenie enkodera do liczb 1 - 99
        if(encoder > 99){
             encoder = 99;
        }

        if(encoder < 1){
            encoder = 1;
        }

        //Wypisanie wybranych liczb na LCD
        LCD_GoTo( 0, 0 );
        itoa(encoder,bufor,10);
        if(encoder < 10){
            bufor[1] = bufor[0];
            bufor[0] = '0';
            bufor[2] = ' ';
            bufor[3] = ' ';
        }

        LCD_WriteText( bufor );

        LCD_GoTo( 2, 0 );
        LCD_WriteText("               ");

        //Drugie zabezpieczenie
        }else if (securityNumber == 1){

             LCD_GoTo(0,0);
             LCD_WriteText("Keyboard");

             //Konfiguracja portu klawiatury
             DDRB=0xF0;
             _delay_ms(1);
             PORTB=0x0F;
             _delay_ms(1);

             keyboardResult[0] = '_';
             keyboardResult[1] = '_';
             keyboardResult[2] = '_';
             keyboardResult[3] = '_';

             LCD_GoTo(0,1);
             LCD_WriteText(keyboardResult);
             LCD_GoTo(4,1);
             LCD_WriteText("              ");

             keyboardListener = 1;
             //W petli odczytuje wybrane przyciski z klawiatury
             while(keyboardListener == 1){

                 if (PINB!=0b11110000){

                     _delay_ms(5);
                     keypressed = PINB;
                     DDRB ^=0b11111111;
                     _delay_ms(1);
                     PORTB ^= 0b11111111;
                     _delay_ms(1);
                     keypressed |=PINB;

                     if (keypressed==0b01111110 && keyboardCount < 4){
                        LCD_GoTo(0,1);
                        keyboardResult[keyboardCount] = '7';
                        LCD_WriteText(keyboardResult);
                        LCD_GoTo(4,1);
                        LCD_WriteText("              ");
                        keyboardCount++;
                     }

                     if (keypressed==0b10111110 && keyboardCount < 4){
                        LCD_GoTo(0,1);
                        keyboardResult[keyboardCount] = '8';
                        LCD_WriteText(keyboardResult);
                        LCD_GoTo(4,1);
                        LCD_WriteText("              ");
                        keyboardCount++;
                     }

                     if (keypressed==0b11011110 && keyboardCount < 4){
                        LCD_GoTo(0,1);
                        keyboardResult[keyboardCount] = '9';
                        LCD_WriteText(keyboardResult);
                        LCD_GoTo(4,1);
                        LCD_WriteText("              ");
                        keyboardCount++;
                     }

                     if (keypressed==0b01111101 && keyboardCount < 4){
                        LCD_GoTo(0,1);
                        keyboardResult[keyboardCount] = '4';
                        LCD_WriteText(keyboardResult);
                        LCD_GoTo(4,1);
                        LCD_WriteText("              ");
                        keyboardCount++;
                     }

                     if (keypressed==0b10111101 && keyboardCount < 4){
                        LCD_GoTo(0,1);
                        keyboardResult[keyboardCount] = '5';
                        LCD_WriteText(keyboardResult);
                        LCD_GoTo(4,1);
                        LCD_WriteText("              ");
                        keyboardCount++;
                     }

                     if (keypressed==0b11011101 && keyboardCount < 4){
                        LCD_GoTo(0,1);
                        keyboardResult[keyboardCount] = '6';
                        LCD_WriteText(keyboardResult);
                        LCD_GoTo(4,1);
                        LCD_WriteText("              ");
                        keyboardCount++;
                     }

                     if (keypressed==0b01111011 && keyboardCount < 4){
                        LCD_GoTo(0,1);
                        keyboardResult[keyboardCount] = '1';
                        LCD_WriteText(keyboardResult);
                        LCD_GoTo(4,1);
                        LCD_WriteText("              ");
                        keyboardCount++;
                     }

                     if (keypressed==0b10111011 && keyboardCount < 4){
                        LCD_GoTo(0,1);
                        keyboardResult[keyboardCount] = '2';
                        LCD_WriteText(keyboardResult);
                        LCD_GoTo(4,1);
                        LCD_WriteText("              ");
                        keyboardCount++;
                     }

                     if (keypressed==0b11011011 && keyboardCount < 4){
                        LCD_GoTo(0,1);
                        keyboardResult[keyboardCount] = '3';
                        LCD_WriteText(keyboardResult);
                        LCD_GoTo(4,1);
                        LCD_WriteText("              ");
                        keyboardCount++;
                     }

                     if (keypressed==0b10110111 && keyboardCount < 4){
                        LCD_GoTo(0,1);
                        keyboardResult[keyboardCount] = '0';
                        LCD_WriteText(keyboardResult);
                        LCD_GoTo(4,1);
                        LCD_WriteText("              ");
                        keyboardCount++;
                     }

                     //Jezeli zostaly wybrane wszystkie cyfry, to sprawdzam czy sa poprawne
                     if(keyboardCount >= 4){
                        if(keyboardResult[0] == '1' && keyboardResult[1] == '9' && keyboardResult[2] == '9' && keyboardResult[3] == '5'){
                            LCD_GoTo(10,1);
                            LCD_WriteText("OPEN");
                            _delay_ms( 1000 );

                            securityNumber++;
                            keyboardListener = 0;
                            LCD_Clear();

                       }else{
                            LCD_GoTo(10,1);
                            LCD_WriteText("WRONG");
                            keyboardCount = 0;
                            keyboardListener = 0;
                            _delay_ms( 1000 );
                            LCD_Clear();

                        }
                     }

                     keypressed=0;
                     DDRB ^=0b11111111;
                     _delay_ms(1);
                     PORTB ^= 0b11111111;
                     _delay_ms(220);

                 }

             }


        }else if (securityNumber == 2){
             LCD_GoTo(0,0);
             LCD_WriteText("Pass RFID card");

             //Inicjalizacja EM4095 i procesora do dekodowania kart
             RFID_init();

             //Sprawdzam w petli czy zostala przylozona odpowiednia karta
             while(1){
                 if(RFID_id[0]==0002 && RFID_id[1]==3346 && RFID_id[2]==5003 && RFID_id[3]==5408){
                     LCD_GoTo(10,1);
                    LCD_WriteText("OPEN");
                 }
             }
        }


        _delay_ms( 1 );
    }

    return 0;
}
