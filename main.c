/*==============================================================================        
        Project: Remote             		Date: Jan. 22, 2015
        Processor: PIC12F683
==============================================================================*/

#include    "xc.h"              // XC compiler general include file

#include    "stdint.h"          // Integer definition
#include    "stdbool.h"         // Boolean (true/false) definition

#include    "system.h"          // User-created system-level functions
#include    "user.h"            // User-created variables and functions

unsigned char counter;
unsigned char cTemp;
unsigned char i, j;

/*==============================================================================
 Sony Ping Function (Called by Send Function)
==============================================================================*/
void pingSony(unsigned char cPulses) { //Warning is from comparing unsigned chars
    for (counter = cPulses; counter != 0; counter--) { //Decrementing counters work faster than incrementing ones
        Q1 = 1;                                 //Makes Lights Pulse at 40kHz
        Q2 = 1;
        asm("nop"); 
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        Q1 = 0;
        Q2 = 0;
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop"); 
        asm("nop");
        asm("nop");
        asm("nop");
    }
}
/*==============================================================================
 NEC Ping Function (Called by Send Function)
==============================================================================*/
void pingNEC(unsigned char cPulses) {
    for (counter = cPulses; counter != 0; counter--) {
        Q1 = 1;                 //Lights Pulse at 38kHz
        Q2 = 1;
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        Q1 = 0;
        Q2 = 0;
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    }
}
/*==============================================================================
 RC5 Ping Function (Called by Send Function)
==============================================================================*/
void pingRC5(unsigned char cPulses) {
    for (counter = cPulses; counter != 0; counter--) {
        Q1 = 1;                     //Lights Pulse at 36kHz
        Q2 = 1;
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        Q1 = 0;
        Q2 = 0;
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    }
}
/*==============================================================================
 Send Function (Called by sendfivetimes function)
==============================================================================*/
void send(unsigned char cAddress, unsigned char cCommand, unsigned char c, unsigned char flip) {
    if (c == 1) {
        pingSony(91);
        __delay_us(700);
        i = 0;
        j = 0;
        while (i < 7) {
            cTemp = cCommand & 0b00000001; //AND THE VALUE WITH BINARY NUMBER TO CHECK IF THE LSB IS 0
            if (cTemp == 1) {
                pingSony(44);
            } else {
                pingSony(21);

            }
            cCommand = cCommand >> 1; //BIT SHIFT OVER 1
            __delay_us(680);
            i++;
        }
        while (j < 5) {
            cTemp = cAddress & 0b00000001; //AND THE VALUE WITH BINARY NUMBER TO CHECK IF THE LSB IS 0
            if (cTemp == 1) {
                pingSony(44);

            } else {
                pingSony(21);
            }
            cAddress = cAddress >> 1; //BIT SHIFT OVER 1
            __delay_us(680);
            j++;
        }
    } else if (c == 2) {
        pingNEC(290);
        __delay_us(4599);
        for (int i = 0; i < 8; i++) { //Send Address
            cTemp = cAddress & 0b00000001;
            if (cTemp == 1) {
                pingNEC(16);
                __delay_us(1688);
            } else {
                pingNEC(16);
                __delay_us(570);
            }
            cAddress = cAddress >> 1;
        }
        for (int i = 0; i < 8; i++) { //Send Address Inverse
            cTemp = cAddress & 0b00000001;
            if (cTemp == 0) {
                pingNEC(16);
                __delay_us(1688);
            } else {
                pingNEC(16);
                __delay_us(570);
            }
            cAddress = cAddress >> 1;
        }
        for (int i = 0; i < 8; i++) { //Send Command
            cTemp = cCommand & 0b00000001;
            if (cTemp == 1) {
                pingNEC(16);
                __delay_us(1688);
            } else {
                pingNEC(16);
                __delay_us(570);
            }
            cCommand = cCommand >> 1;
        }
        for (int i = 0; i < 8; i++) { //Send Command Inverse
            cTemp = cCommand & 0b00000001;
            if (cTemp == 0) {
                pingNEC(16);
                __delay_us(1688);
            } else {
                pingNEC(16);
                __delay_us(570);
            }
            cCommand = cCommand >> 1;
        }
        pingNEC(16);
    } else {
        pingRC5(28);                //Send Start Bits
        __delay_us(950);
        pingRC5(28);
        __delay_us(950);
        if (flip == 0) {
            pingRC5(28);            //Send Toggle Bit
            __delay_us(950);
        }
        if (flip == 1) {
            __delay_us(950);
            pingRC5(28);
        }
        for (unsigned char i = 0; i < 5; i++) {     //Send Address
            cTemp = cAddress & 0b00000001;
            if (cTemp == 1) {
                pingRC5(28);
                __delay_us(950);
            } else {
                __delay_us(950);
                pingRC5(28);
            }
            cAddress = cAddress >> 1;
        }
        for (unsigned char i = 0; i < 6; i++) {     //Send Command
            cTemp = cCommand & 0b00000001;
            if (cTemp == 1) {
                pingRC5(28);
                __delay_us(950);
            } else {
                __delay_us(950);
                pingRC5(28);
            }
            cCommand = cCommand >> 1;
        }
    }
}
/*==============================================================================
 sendfivetimes Function (Called by Main)
==============================================================================*/
void sendfivetimes(unsigned char cAddress, unsigned char cCommand, unsigned char c, unsigned char flip, int nDelay) {
    for (int i = 0; i < 5; i++) {               //Function to send each code 5 times.
        send(cAddress, cCommand, c, flip);
        if (nDelay == 25) {
            __delay_ms(25);
        } else if (nDelay == 114) {
            __delay_ms(114);
        } else if (nDelay == 140) {
            __delay_ms(140);
        }
    }
}
/*==============================================================================
 panasonic (Called by Main)
==============================================================================*/
void panasonic (unsigned char PanasonicTV, unsigned char PanasonicTV2, unsigned char PanasonicTV3, unsigned char PanasonicTV4, unsigned char PanasonicCommand, unsigned char PanasonicCommand2, unsigned char PanasonicFinal){
    pingSony(142);
    __delay_us(1650);
    for (int i = 0;i<8;i++){
        cTemp = PanasonicTV & 0b00000001;
        if (cTemp == 0) {
             pingSony(20);
            __delay_us(300);
        } else {
            pingSony(20);
            __delay_us(1150);
        }
        PanasonicTV = PanasonicTV >> 1;
    }
    for (int i = 0;i<8;i++){
        cTemp = PanasonicTV2 & 0b00000001;
        if (cTemp == 0) {
             pingSony(20);
            __delay_us(300);
        } else {
            pingSony(20);
            __delay_us(1150);
        }
        PanasonicTV2 = PanasonicTV2 >> 1;
    }
    for (int i = 0;i<8;i++){
        cTemp = PanasonicTV3 & 0b00000001;
        if (cTemp == 0) {
             pingSony(20);
            __delay_us(300);
        } else {
            pingSony(20);
            __delay_us(1150);
        }
        PanasonicTV3 = PanasonicTV3 >> 1;
    }
    for (int i = 0;i<8;i++){
        cTemp = PanasonicTV4 & 0b00000001;
        if (cTemp == 0) {
             pingSony(20);
            __delay_us(300);
        } else {
            pingSony(20);
            __delay_us(1150);
        }
        PanasonicTV4 = PanasonicTV4 >> 1;
    }
    for (int i = 0;i<8;i++){
        cTemp = PanasonicCommand & 0b00000001;
        if (cTemp == 0) {
             pingSony(20);
            __delay_us(300);
        } else {
            pingSony(20);
            __delay_us(1150);
        }
        PanasonicCommand = PanasonicCommand >> 1;
    }
    for (int i = 0;i<8;i++){
        cTemp = PanasonicCommand2 & 0b00000001;
        if (cTemp == 0) {
             pingSony(20);
            __delay_us(300);
        } else {
            pingSony(20);
            __delay_us(1150);
        }
        PanasonicCommand2 = PanasonicCommand2 >> 1;
    }
    for (int i = 0;i<1;i++){
        cTemp = PanasonicFinal & 0b00000001;
        if (cTemp == 0) {
             pingSony(20);
            __delay_us(300);
        } else {
            pingSony(20);
            __delay_us(1150);
        }
        PanasonicFinal = PanasonicFinal >> 1;
    }
}
/*==============================================================================
        Main program code
==============================================================================*/
int main(void) {

    // Initialize I/O and peripherals for PORTB output
    initPorts();
    //Bit codes to be sent to TV
    unsigned char SonyDown = 0b00010011; //7 bit command length   0b01100100
    unsigned char SonyTV = 0b00000001; //5 bit address`   0b00010000
    unsigned char SonyPower = 0b00010101; //0b01010100
    unsigned char SonyUp = 0b00010010; //    0b00100100

    unsigned char RelisysTV = 0b11101101;
    unsigned char RelisysUp = 0b00011010;
    unsigned char RelisysDown = 0b00011110;
    unsigned char RelisysPower = 0b00001011;

    unsigned char PanasonicTV = 0b00000010; //SEND INVERSE FIRST? or send 0000 0010
    unsigned char PanasonicTV2 = 0b00100000;
    unsigned char PanasonicTV3 = 0b10000000;
    unsigned char PanasonicTV4 = 0b00000000;
    unsigned char PanasonicUp = 0b00100000;
    unsigned char PanasonicUp2 = 0b10100000;
    unsigned char PanasonicDown = 0b00100001;
    unsigned char PanasonicDown2 = 0b10100001;              //ALL ARE DONE
    unsigned char PanasonicPower = 0b00111101;
    unsigned char PanasonicPower2 = 0b10111101;
    unsigned char PanasonicFinal = 0b00000000;

    unsigned char SharpTV = 0b00000001; //Taken from paper
    unsigned char SharpUp = 0b00010100;
    unsigned char SharpDown = 0b00010110;
    unsigned char SharpPower = 0b01001000;

    unsigned char ToshibaTV = 0b01000000;
    unsigned char ToshibaPower = 0b00010010;
    unsigned char ToshibaUp = 0b00011010;
    unsigned char ToshibaDown = 0b00011110;

    unsigned char RC5Down = 0b00100010;
    unsigned char RC5Power = 0b00001100;
    unsigned char RC5Up = 0b00000010;
    unsigned char RC5TV = 0b00010000;
    unsigned char cflip = 0;

    while (1) {
        LED1 = 1;
        Q1 = 0;
        Q2 = 0;

        if (S1 == 0) {
            while (S1 == 0) {
                sendfivetimes(SonyTV, SonyDown, 1, 0, 25); //SONY
                __delay_ms(10);
                sendfivetimes(RC5TV, RC5Down, 3, cflip, 114);        //RC5
                __delay_ms(10);
                sendfivetimes(RelisysTV, RelisysDown, 2, 0, 140);        //NEC
                __delay_ms(10);
                panasonic(PanasonicTV, PanasonicTV2, PanasonicTV3, PanasonicTV4, PanasonicDown, PanasonicDown2, PanasonicFinal);
                __delay_ms(10);
                sendfivetimes(SharpTV, SharpDown, 2, 0, 140);        //NEC
                __delay_ms(10);
                sendfivetimes(ToshibaTV, ToshibaDown, 2, 0, 140);        //NEC
                __delay_ms(10);
            }
            if (cflip == 0) { //when released, flip toggle bit
                cflip = 1;
            } else if (cflip == 1) {
                cflip = 0;
            }
        }
        if (S2 == 0) { //APPARENTLY S2 IS ALWAYS PRESSED
            while (S2 == 0) {
                sendfivetimes(SonyTV, SonyPower, 1, 0, 25);      //SONY
                __delay_ms(10);
                sendfivetimes(RC5TV, RC5Power, 3, cflip, 114);       //RC5
                __delay_ms(10);
                sendfivetimes(RelisysTV, RelisysPower, 2, 0, 140);       //NEC
                __delay_ms(10);
                panasonic(PanasonicTV, PanasonicTV2, PanasonicTV3, PanasonicTV4, PanasonicPower, PanasonicPower2, PanasonicFinal);
                __delay_ms(10);
                sendfivetimes(SharpTV, SharpPower, 2, 0, 140);       //NEC
                __delay_ms(10);
                sendfivetimes(ToshibaTV, ToshibaPower, 2, 0, 140);       //NEC
                __delay_ms(10);
            }
            if (cflip == 0) { //when released, flip toggle bit
                cflip = 1;
            } else if (cflip == 1) {
                cflip = 0;
            }
        }
        if (S3 == 0) {
            while (S3 == 0) {
                sendfivetimes(SonyTV, SonyUp, 1, 0, 25);     //SONY
                __delay_ms(10);
                sendfivetimes(RC5TV, RC5Up, 3, cflip, 114);      //RC5
                __delay_ms(10);
                sendfivetimes(RelisysTV, RelisysUp, 2, 0, 140);       //NEC
                __delay_ms(10);
                panasonic(PanasonicTV, PanasonicTV2, PanasonicTV3, PanasonicTV4, PanasonicDown, PanasonicDown2, PanasonicFinal);
                __delay_ms(10);
                sendfivetimes(SharpTV, SharpUp, 2, 0, 140);       //NEC
                __delay_ms(10);
                sendfivetimes(ToshibaTV, ToshibaUp, 2, 0, 140);       //NEC
                __delay_ms(10);
            }
            if (cflip == 0) { //when released, flip toggle bit
                cflip = 1;
            } else if (cflip == 1) {
                cflip = 0;
            }
        }
    }
}