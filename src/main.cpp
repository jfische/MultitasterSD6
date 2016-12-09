/*
 * main.cpp
 *
 * Created: 03.12.2016 02:31:44
 *  Author: haus-bus
 */ 

#include <Framework/Framework.h>
#include "Units/PwmLed.h"
#include "Elements/Button.h"

static const uint8_t debugLevel( DEBUG_LEVEL_LOW );
#define getId() FSTR("Main::")


static usart_serial_options_t usart_options = 
{
	.baudrate = USART_SERIAL_BAUDRATE,
	.charlength = USART_SERIAL_CHAR_LENGTH,
	.paritytype = USART_SERIAL_PARITY,
	.stopbits = USART_SERIAL_STOP_BIT
};

void putc( char c )
{
	udi_cdc_putc( c );
}

int main (void)
{
	// Authorize interrupts
	irq_initialize_vectors();
	cpu_irq_enable();
	
	// Initialize the sleep manager service
	sleepmgr_init();
	
	// Initialize the clock service
	sysclk_init();
	board_init();
	
	// Initialize system timer
	rtc_init();
	
	// Initialize RS485 interface
	usart_serial_init(USART_SERIAL, &usart_options);
	
	// Enable USB Stack Device
	udc_start ();
	
	Logger::setStream( putc );
	
	Timestamp timestamp;
	
	PwmLed led1( PWM_TCC0, PWM_CH_A, 5000 );
	PwmLed led2( PWM_TCC0, PWM_CH_B, 5000 );
	PwmLed led3( PWM_TCC0, PWM_CH_C, 5000 );
	PwmLed led4( PWM_TCC0, PWM_CH_D, 5000 );
	PwmLed led5( PWM_TCC1, PWM_CH_A, 5000 );
	PwmLed led6( PWM_TCC1, PWM_CH_B, 5000 );
	
	uint8_t cnt = 0;
	
	while (true)
	{
		uint8_t divisor = 1;
		
		if( BUTTON_isPressed( BUTTON_S1_GPIO ) ) divisor += 2 ;
		if( BUTTON_isPressed( BUTTON_S2_GPIO ) ) divisor += 4 ;
		if( BUTTON_isPressed( BUTTON_S3_GPIO ) ) divisor += 8 ;
		if( BUTTON_isPressed( BUTTON_S4_GPIO ) ) divisor += 16 ;
		if( BUTTON_isPressed( BUTTON_S5_GPIO ) ) divisor += 32 ;
		if( BUTTON_isPressed( BUTTON_S6_GPIO ) ) divisor += 64 ;
		
		uint8_t duty = ( SystemTime::now() / 10 )  % 100;
		
		led1.setDutyCycle( ( cnt & 0x01 ) ? duty : 0  );
		led2.setDutyCycle( ( cnt & 0x02 ) ? duty : 0  );
		led3.setDutyCycle( ( cnt & 0x04 ) ? duty : 0  );
		led4.setDutyCycle( ( cnt & 0x08 ) ? duty : 0  );
		led5.setDutyCycle( ( cnt & 0x10 ) ? duty : 0  );
		led6.setDutyCycle( ( cnt & 0x20 ) ? duty : 0  );
		
		if( timestamp.since() >= SystemTime::S/divisor )
		{
			timestamp = Timestamp();
			PORTR.OUTTGL = 1;
			DEBUG_H1( "loop");
			cnt++;
		}
		
	}
	
	return 1;
}
