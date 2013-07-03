/*
 * charlie.h
 *
 * Created: 21-12-2012 13:18:13
 *  Author: Mark
 */ 


#ifndef charlie_H_
#define charlie_H_

//Function declarations:
ISR (TIMER0_OVF_vect); //interrupt routine handles the plexing.
void setup_charlie(void); //set up hardware
void setup_charlie_struct(void); //instantiate the struct with the proper DDR and PORT values
void set_charlie(uint8_t charlieVal); //set a value from 0 to 10 (+ special patterns 11 and default)
void roll_charlie(void); //roll from 0 to 10 in about a second
void charlie_blink(unsigned char repetitions);


#endif /* charlie_H_ */