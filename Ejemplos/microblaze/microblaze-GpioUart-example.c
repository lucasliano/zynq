/******************************************************************************
* Copyright (C) 2005 - 2021 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/****************************************************************************/
/**
 *
 *	Author: Lucas Liaño
 *
 *	Description: Simple microblaze example combining various examples. We will
 *				 use the UARTLite IP and the AXI-GPIO IP.
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xuartlite.h"
#include "xgpio.h"
#include "xil_printf.h"

/************************** Constant Definitions *****************************/

#define UARTLITE_DEVICE_ID		XPAR_UARTLITE_0_DEVICE_ID
#define GPIO_EXAMPLE_DEVICE_ID  XPAR_GPIO_0_DEVICE_ID
#define GPIO_CHANNEL 1

#define DELAY     10000000


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

int UartLiteInitAndTest(u16 DeviceId);

/************************** Variable Definitions *****************************/

XUartLite UartLite;		 /* Instance of the UartLite device */
XGpio Gpio; /* The Instance of the GPIO Driver */

/*****************************************************************************/
/**
*
*	Pone un contador que barre los leds y printea comentarios de estado por
*	la UART.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
#ifndef TESTAPP_GEN
int main(void)
{
	int Status;
	int delay;
	uint32_t i;

	// Ejecuta la inicialización y un test
	Status = UartLiteInitAndTest(UARTLITE_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("ERROR: UARTLite self-test failed.\r\n");
		return XST_FAILURE;
	}


	// A partir de acá funciona la UART
	xil_printf("Uartlite was successfully initialized.\r\n");


	// Inicializa el GPIO Driver
	Status = XGpio_Initialize(&Gpio, GPIO_EXAMPLE_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

	// Todos los pines como salidas
	XGpio_SetDataDirection(&Gpio, GPIO_CHANNEL, 0);


	while (1) {
		xil_printf("Iniciando barrido..\r\n");
		for (i = 0; i < 4; i++)
		{
			xil_printf("N = %d\r\n", i);

			/* Set the LED to High */
			XGpio_DiscreteWrite(&Gpio, GPIO_CHANNEL, i);

			/* Wait a small amount of time so the LED is visible */
			for (delay = 0; delay < DELAY; delay++);
		}
		xil_printf("Barrido finalizado\r\n");

		for (delay = 0; delay < 10*DELAY; delay++);
	}



	// You should not get here
	return XST_SUCCESS;
}
#endif

/*****************************************************************************/
/**
*
* This function does a minimal test on the UartLite device and driver as a
* design example. The purpose of this function is to illustrate
* how to use the XUartLite component.
*
*
* @param	DeviceId is the XPAR_<uartlite_instance>_DEVICE_ID value from
*		xparameters.h.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None.
*
****************************************************************************/
int UartLiteInitAndTest(u16 DeviceId)
{
	int Status;

	/*
	 * Initialize the UartLite driver so that it is ready to use.
	 */
	Status = XUartLite_Initialize(&UartLite, DeviceId);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XUartLite_SelfTest(&UartLite);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

