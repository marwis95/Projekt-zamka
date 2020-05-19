/*
 * rfid_em4096.h
 *
 *  Created on: 2012-04-07
 *       Autor: Piotr Rzeeszut
 */

#ifndef RFID_EM4095_H_
#define RFID_EM4095_H_

#define OUT_PORT PORTC
#define OUT_DDR DDRC
#define OUT_PIN PC0


#define MOD_PORT PORTC
#define MOD_DDR DDRC
#define MOD_PIN PC1

#define SHD_PORT PORTC
#define SHD_DDR DDRC
#define SHD_PIN PC2

#define CLK_PORT PORTC
#define CLK_DDR DDRC
#define CLK_PIN PC3


//maski 64 bit do obs�ugi tagu

#define SBIT_MASK  0b1111111110000000000000000000000000000000000000000000000000000000
#define MBIT_MASK  0b1000000000000000000000000000000000000000000000000000000000000000
#define LBIT_MASK  0b0000000000000000000000000000000000000000000000000000000000000001

#define D1_MASK    0b0000000001111000000000000000000000000000000000000000000000000000
#define D1_SHIFT   51
#define P1_MASK    0b0000000000000100000000000000000000000000000000000000000000000000
#define P1_SHIFT   50

#define D2_MASK    0b0000000000000011110000000000000000000000000000000000000000000000
#define D2_SHIFT   46
#define P2_MASK    0b0000000000000000001000000000000000000000000000000000000000000000
#define P2_SHIFT   45

#define D3_MASK    0b0000000000000000000111100000000000000000000000000000000000000000
#define D3_SHIFT   41
#define P3_MASK    0b0000000000000000000000010000000000000000000000000000000000000000
#define P3_SHIFT   40

#define D4_MASK    0b0000000000000000000000001111000000000000000000000000000000000000
#define D4_SHIFT   36
#define P4_MASK    0b0000000000000000000000000000100000000000000000000000000000000000
#define P4_SHIFT   35

#define D5_MASK    0b0000000000000000000000000000011110000000000000000000000000000000
#define D5_SHIFT   31
#define P5_MASK    0b0000000000000000000000000000000001000000000000000000000000000000
#define P5_SHIFT   30

#define D6_MASK    0b0000000000000000000000000000000000111100000000000000000000000000
#define D6_SHIFT   26
#define P6_MASK    0b0000000000000000000000000000000000000010000000000000000000000000
#define P6_SHIFT   25

#define D7_MASK    0b0000000000000000000000000000000000000001111000000000000000000000
#define D7_SHIFT   21
#define P7_MASK    0b0000000000000000000000000000000000000000000100000000000000000000
#define P7_SHIFT   20

#define D8_MASK    0b0000000000000000000000000000000000000000000011110000000000000000
#define D8_SHIFT   16
#define P8_MASK    0b0000000000000000000000000000000000000000000000001000000000000000
#define P8_SHIFT   15

#define D9_MASK    0b0000000000000000000000000000000000000000000000000111100000000000
#define D9_SHIFT   11
#define P9_MASK    0b0000000000000000000000000000000000000000000000000000010000000000
#define P9_SHIFT   10

#define D10_MASK   0b0000000000000000000000000000000000000000000000000000001111000000
#define D10_SHIFT  6
#define P10_MASK   0b0000000000000000000000000000000000000000000000000000000000100000
#define P10_SHIFT  5

#define CP1_MASK   0b0000000000000000000000000000000000000000000000000000000000010000
#define CP1_SHIFT  4

#define CP2_MASK   0b0000000000000000000000000000000000000000000000000000000000001000
#define CP2_SHIFT  3

#define CP3_MASK   0b0000000000000000000000000000000000000000000000000000000000000100
#define CP3_SHIFT  2

#define CP4_MASK   0b0000000000000000000000000000000000000000000000000000000000000010
#define CP4_SHIFT  1

/*
//taktowanie z sygna�u CLK uk�adu EM4095
#define TOLERANCE 10
#define MIN_HALF_BIT     32 - TOLERANCE
#define MAX_HALF_BIT     32 + TOLERANCE
#define MAX_BIT         64 + TOLERANCE
*/
//taktowanie z preskalera timera
#define T1_PRESCALER 8

#define T_TOLERANCE     10
#define T_MIN_HALF_BIT     32 - T_TOLERANCE
#define T_MAX_HALF_BIT     32 + T_TOLERANCE
#define T_MAX_BIT       64 + T_TOLERANCE

#define TOLERANCE       ((F_CPU*(T_TOLERANCE   ))/T1_PRESCALER)/125000
#define MIN_HALF_BIT     ((F_CPU*(T_MIN_HALF_BIT))/T1_PRESCALER)/125000
#define MAX_HALF_BIT     ((F_CPU*(T_MAX_HALF_BIT))/T1_PRESCALER)/125000
#define MAX_BIT         ((F_CPU*(T_MAX_BIT     ))/T1_PRESCALER)/125000

extern volatile uint64_t RFID_data;

// flaga = 1 - informuje, �e odebrany zosta� nowy kod z pilota
extern volatile uint8_t RFID_decoded_flag;

extern volatile uint8_t RFID_id[5];

// deklaracje funkcji dla u�ytkownika
void RFID_init();

uint8_t header_align();

uint8_t parity_cal(uint8_t value);

uint8_t h_parity();

uint8_t v_parity();


#endif /* RFID_EM4095_H_ */