/*
 * Autor: Lucas Liaño
 *
 * Descripción: Un proyecto un poquito más avanzado, donde tenemos 2 cores AXI-GPIO.
 *
 */



/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"

/************************** Constant Definitions *****************************/


/* La constante presentada a continuacion esta definida en xparameters.h */
#define AXI_GPIO_0_ID  XPAR_GPIO_0_DEVICE_ID
#define AXI_GPIO_1_ID  XPAR_GPIO_1_DEVICE_ID

/* Aclaramos que vamos a usar el canal 1 del IPcore AXI-GPIO */
#define LED_CHANNEL 1
#define AND_CHANNEL 1

/* El bit 0 del AXI-GPIO está conectado a un LED*/
#define LED_PIN 0x1
#define AND_PIN 0x1

#define LED_DELAY     10000000


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/


XGpio axi_gpio0; /* Instancia del GPIO Driver */
XGpio axi_gpio1; /* Instancia del GPIO Driver */


/*****************************************************************************/
/*
* Rutina main
*
* @return	XST_FAILURE para indicar un error durante la inicializacion.
*
* @note		This function will not return if the test is running.
*
******************************************************************************/
int main(void)
{
	int status;
	volatile int delay;

	/* Comienza a rutina */
	xil_printf("Inicializando ambos AXI-GPIO Drivers\r\n");



	/* Inicializacion */
	status = XGpio_Initialize(&axi_gpio0, AXI_GPIO_0_ID);
	if (status != XST_SUCCESS) {
		xil_printf("Gpio0 Initialization Failed\r\n");
		return XST_FAILURE;
	}

	status = XGpio_Initialize(&axi_gpio1, AXI_GPIO_1_ID);
	if (status != XST_SUCCESS) {
		xil_printf("Gpio1 Initialization Failed\r\n");
		return XST_FAILURE;
	}

	/* Set the direction for all signals as inputs except the LED output */
	XGpio_SetDataDirection(&axi_gpio0, AND_CHANNEL, AND_PIN);
	XGpio_SetDataDirection(&axi_gpio1, LED_CHANNEL, ~LED_PIN);

	/* Loop forever blinking the LED */
	while (1) {

		/* El led va a titilar si esta la AND encendida */
		if ((XGpio_DiscreteRead(&axi_gpio0, AND_CHANNEL) & 0x1) == 1)
		{
			/* Set the LED to High */
			XGpio_DiscreteWrite(&axi_gpio1, LED_CHANNEL, LED_PIN);
			xil_printf("LED ON\r\n");

			/* Wait a small amount of time so the LED is visible */
			for (delay = 0; delay < LED_DELAY; delay++);

			/* Clear the LED bit */
			XGpio_DiscreteClear(&axi_gpio1, LED_CHANNEL, LED_PIN);
			xil_printf("LED OFF\r\n");

			/* Wait a small amount of time so the LED is visible */
			for (delay = 0; delay < LED_DELAY; delay++);

		}

	}

}
