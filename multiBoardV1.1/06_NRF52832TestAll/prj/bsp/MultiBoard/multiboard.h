/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
#ifndef MULTIBOARD_H
#define MULTIBOARD_H

#define SYS_POWER_OFF_ENABLE					0

// LEDs definitions for MultiBoard
#define LEDS_NUMBER    							6

#define LED_START      							14
#define LED_1          							14
#define LED_2          							15
#define LED_3          							18
#define LED_4          							20
#define LED_5          							23
#define LED_6          							5
#define LED_STOP       							5
  

#define LEDS_LIST 								{LED_1, LED_2,LED_3,LED_4,LED_5,LED_6}

#define BSP_LED_0      							LED_1
#define BSP_LED_1      							LED_2

#define BSP_LED_0_MASK 							(1<<BSP_LED_0)
#define BSP_LED_1_MASK 							(1<<BSP_LED_1)
#define BSP_LED_2_MASK 							(1<<LED_3)
#define BSP_LED_3_MASK 							(1<<LED_4)
#define BSP_LED_4_MASK 							(1<<LED_5)
#define BSP_LED_5_MASK 							(1<<LED_6)

#define LEDS_MASK      	(BSP_LED_0_MASK | BSP_LED_1_MASK |BSP_LED_2_MASK |BSP_LED_3_MASK |BSP_LED_4_MASK |BSP_LED_5_MASK)
/* all LEDs are lit when GPIO is low */
#define LEDS_INV_MASK  							LEDS_MASK

#define BUTTONS_NUMBER 							2//5  

#define BUTTON_START   							19
#define BUTTON_1       							19//UP
#define BUTTON_2       							22 //20//LEFT  //aizj
#define BUTTON_3       							22//RIGHT
#define BUTTON_4       							26//SELECT  // aizj
#define BUTTON_5       							24//DOWN
#define BUTTON_STOP    							24//23
#define BUTTON_PULL    							NRF_GPIO_PIN_PULLUP

#define BUTTONS_LIST 							{BUTTON_1, BUTTON_2}//, BUTTON_3, BUTTON_4, BUTTON_5}

#define BSP_BUTTON_0   							BUTTON_1
#define BSP_BUTTON_1   							BUTTON_2
//#define BSP_BUTTON_2   							BUTTON_3
//#define BSP_BUTTON_3   							BUTTON_4
//#define BSP_BUTTON_4   							BUTTON_5

#define BSP_BUTTON_0_MASK 						(1<<BSP_BUTTON_0)
#define BSP_BUTTON_1_MASK 						(1<<BSP_BUTTON_1)
#define BSP_BUTTON_2_MASK 						(1<<BSP_BUTTON_2)
#define BSP_BUTTON_3_MASK 						(1<<BSP_BUTTON_3)
#define BSP_BUTTON_4_MASK 						(1<<BSP_BUTTON_4)

#define BUTTONS_MASK   							(BSP_BUTTON_0_MASK | BSP_BUTTON_1_MASK ) //| BSP_BUTTON_2_MASK | \
												BSP_BUTTON_3_MASK | BSP_BUTTON_4_MASK)

#define RX_PIN_NUMBER  							7
#define TX_PIN_NUMBER  							8
#define CTS_PIN_NUMBER 							0XFF
#define RTS_PIN_NUMBER 							0XFF
#define HWFC           							false

#define SPIM1_SCK_PIN       					30     	/**< SPI clock GPIO pin number. */
#define SPIM1_MOSI_PIN      					31     	/**< SPI Master Out Slave In GPIO pin number. */
#define SPIM1_MISO_PIN      					0xFF   	/**< SPI Master In Slave Out GPIO pin number. */
#define SPIM1_SS_PIN        					3      	/**< SPI Slave Select GPIO pin number. */
#define LCD_RST_PIN								6	   	/*OLED reset pin define*/
#define LCD_DC_PIN								4	   	/*OLED data or commander select pin define*/

#define SPIM2_SCK_PIN   						11  	// SPI clock GPIO pin number.
#define SPIM2_MOSI_PIN  						12  	// SPI Master Out Slave In GPIO pin number.
#define SPIM2_MISO_PIN  						13  	// SPI Master In Slave Out GPIO pin number.
#define SPIM2_CS_PIN    						10  	// SPI CS GPIO pin number.

#define MPU_TWI_SCL_PIN							16		//mpu6050 scl pin
#define MPU_TWI_SDA_PIN							17		//mpu6050 sda pin
#define MPU_TWI_INT_PIN							16//18		//mpu6050 interrupt pin 

//moto control pin define
#define MOTO_CNTRL_PIN							25

//RGB pwm pin define
#define RGB_G_PIN								26
#define RGB_R_PIN								27
#define RGB_B_PIN								28

//define beep pwm control pin
#define BEEP_PWM_PIN							29

//battery charge state and dc input detect and battery volt level measure
#define BAT_CHARG_STATE_PIN						0
#define DC_INPUT_DETECTED_PIN					1
#define BAT_LEVEL_ENABLE_PIN					9
#define BAT_LEVEL_ADC_PIN						2

//Light sensor adc detected pin define 
#define LIGHT_ADC_PIN							 6//5

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                 .rc_ctiv       = 0,                                \
                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

#endif // PCA10036_H
