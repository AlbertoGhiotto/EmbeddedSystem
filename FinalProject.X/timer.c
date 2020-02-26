/*
 * File:   scheduler.c
 * Author:   Filippo Gandolfi    S4112879
 *           Alberto Ghiotto     S4225586
 *
 * Created on 25 December 2019, 19:32
 */

#include "xc.h"
#include "global.h"

int main(void) {
    return 0;
}

// Scheduler Timer - setup function
void tmr1_setup_period(int ms) {
    TMR1 = 0;               // Reset timer counter
    Fcy = (Fosc / 4.0);
    PR1 = (Fcy) / 64.0 * (ms / 1000.0); 

    IFS0bits.T1IF = 0;      // Resets interrupt flag (is necessary? Already don in wait period)
    
    T1CONbits.TCKPS = 0b10; // Prescaler 1:64    -> up to a bit more than 2 seconds
    T1CONbits.TON = 1;      // Starts the timer!
}

// Timer 1 temporization function 
void tmr1_wait_period() {
    while (IFS0bits.T1IF == 0)  // Wait for the timer to finish
    {
    }
    IFS0bits.T1IF = 0;          // Set the timer flag to zero to be notified of a new event    
}

// Timeout mode timer - setup function
void tmr2_setup_period(int ms)
{
    TMR2 = 0;                               // Reset timer counter
    //Fcy = (Fosc / 4.0); // already global
    PR2 = (Fcy) / 256.0 * (ms / 1000.0); 
    // TODO: set prescaler to allow 5 seconds -> set it to 256. 7200 clock step for each second -> up to
    T2CONbits.TCKPS = 0b11;                 // Prescaler 1:256    
    T2CONbits.TON = 1;                      // Starts the timer!
    //IEC0bits.INT0IE = 1;                  // Re - enable interrupt of button s5
    IEC0bits.T2IE = 1;                      // Enable interrupt of timer t2
}

// Timer 2 temporization function 
void tmr2_wait_period() {
    while (IFS0bits.T2IF == 0)              // Wait for the timer to finish
    {
    }
    IFS0bits.T2IF = 0;                      // Set the timer flag to zero to be notified of a new event    
}

// T2 ISR
void __attribute__((__interrupt__, _auto_psv_)) _T2Interrupt() {
    IEC0bits.T2IE = 1;               // Reset interrupt flag
    
    // Set timeout state
    //boardState = STATE_TIMEOUT;
    // Set motor velocity to zero
    int rpm1 = 0;
    int rpm2 = 0;
    normalizeDC(&rpm1, &rpm2);
    actualRPM1 = 0;
    actualRPM2 = 0;
    
    IFS0bits.T2IF = 0;          // Reset interrupt flag for timer 2
    T2CONbits.TON = 0;          // Stop the timer
    TMR2 = 0;                   // Reset the timer 
}