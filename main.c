//fan ve lamba çalýþýyor

#include <msp430.h>

int receivedVal = 0;
float adc = 0.f;
float adcdeger = 0.f;
float sondeger = 0.f;


int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(100000);


  P1DIR |= 0x01;                            // Set P1.0 to output direction
  P1DIR |= BIT7;

  P1SEL = BIT1 + BIT2;            // P1.1/P1.2 rx/tx olarak kullanmak için yapýlan ayar
  P1SEL2 = BIT1 + BIT2;

  UCA0CTL1 |= UCSSEL_2;   //SMCLK   iþlemciden gelen clocku aldý
  UCA0BR0 = 8; // 115200 BAUDRATE AYARLAMAK ÝÇÝN 8 VE 6 SECÝLDÝ
  UCA0MCTL = UCBRS2 + UCBRS1; // hata payýný gýdermek ýcýn modulasyon katsayýsý
  UCA0CTL1 &= ~UCSWRST;       //aktif etti
  IE2 |= UCA0RXIE;   // interrupt aktif etti

  //ADC

  ADC10CTL0 &= ~ENC;  //Can be modified only when ENC = 0
  ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1 = INCH_3 ;                       // input p1.3
  ADC10AE0 |= BIT3;                         // PA.1 ADC enable
  ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start

  //PWM

   P1DIR |= BIT6;
   P1SEL |= BIT6;
   TA0CCR0 = 20000;
   TA0CCR1 = 0;
   TACCTL1 = OUTMOD_7;

   TA0CTL = TASSEL_2 + MC_1 ;


  __enable_interrupt();

  while (1)
  {


  }
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{


    receivedVal = UCA0RXBUF;

     if (receivedVal == 1)
      {
         P1OUT |= BIT0;
         P1OUT &= ~BIT7;
      }

     if (receivedVal == 2)
      {

         P1OUT |= BIT7;
         P1OUT &= ~BIT0;

      }

     if (receivedVal == 3)
      {

         P1OUT &= ~BIT7;
         P1OUT &= ~BIT0;
         TA0CCR1 = 600; //p1.6


      }

     if (receivedVal == 4)
     {

         P1OUT &= ~BIT7;
         P1OUT &= ~BIT0;
         TA0CCR1 = 1125; //p2.1


     }
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    ADC10CTL0 &= ~ENC;
    adcdeger= ADC10MEM;
    adc = (adcdeger / 1023) * 5000;
    sondeger = (int)(adc / 10.0);


        if ((0x10 & P1IN))
        {

            UCA0TXBUF = 1;
        }

        else if (!(0x20 & P1IN))
        {

             UCA0TXBUF = 3;
        }

        else if (sondeger > 10)
        {
            UCA0TXBUF = sondeger;

        }

        else
        {

            UCA0TXBUF = 0;

        }

    ADC10CTL0 |= ENC + ADC10SC;


}


