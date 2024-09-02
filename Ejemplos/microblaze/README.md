# Explicación


En este ejemplo se sintetiza un procesador Microblaze dentro de la PL de la ZYNQ7000. Para ello se utiliza una PYNQ-Z2 como placa de desarrollo.

Cabe destacar que en este ejemplo no estamos usando el PS. Es por ello que la conexión USB-UART que disponemos dentro de la PYNQ-Z2 no se encuentra disponible para utilizar como UART ni como debugger del microblaze. Esto se debe a que la PYNQ-Z2 tiene routeado un el USB un FT2232HL que puede ser utilizado como JTAG para programar el soc, o como UART para debuggear, pero que se encuentra conectado a pines del PS, por lo que es inaccesible desde el PL.


Esto tiene varias implicancias. En primera instancia, no es posible debuggear el microblaze con ese puerto. Luego, el microblaze deberá bootear con la imagen de código que hayamos creado en Vitis desde el PL. Para ello hay que explicarle a Vivado que debe cargar dicho .ELF en fabric, y luego debemos generar un bitfile.


# Pasos

Para poder reproducir este ejemplo se deberán seguir los siguientes pasos

## 1) Creación de HDL

Se deberá reproducir el diagrama en bloques mostrado en block_diagram.pdf. El microblaze está setteado en su configuración por default, con la única modificación de la memoria bram que está en 128kB.

Cabe destacar que se utiliza una UartLite conectada a unos pines del pmodA. Recomiendo ver las lineas descomentadas del .xdc (pin constrains file). Se deberá contar con un módulo USB-UART para poder ver los prints desde Putty.

## 2) Generación de bitstream

Se deberá generar el bitstream y exportarlo para poder iniciar desde el Vitis

## 3) Vitis

Desde Vitis tenemos que importar la plataforma de hardware con el archivo .xsa previamente generado en el paso anterior. Luego debemos crear un proyecto para el microblaze y agregar el .c del repositorio. Deberíamos poder compilar sin ningún problema.

## 4) Agregar .elf a vivado

Para poder indicarle a Vivado que debe precargar la blockram con el código generado en Vitis, es necesario incluirlo al proyecto. Para ello debemos ir a la opción Tools -> Associated ELF files..

Allí podremos agregar dentro de la carpeta designs el archivo .elf que se encuentra en Vitis. El archivo está en la carpeta vitis_worksace/myproject/Debug/filename.elf. 

## 5) Generar bitstream

Para generar el bitstream tenemos que tirar un comando dentro de la consola TCL de Vivado. En primera instancia tenemos que decirle a Vivado que se pare sobre la carpeta "../myproject.runs/impl_1". Esto lo hacemos con el comando "cd".

Luego procedemos a generar el bitstream con el siguiente comando:

"write_cfgmem -format mcs -interface bpix16 -size 128 -loadbit "up 0x0 design_1_wrapper.bit" -file hola_exp.mcs -force"

donde "design_1_wrapper.bit" es el bitstream del proyecto, y "hola_exp.mcs" es el archivo de salida con el contenido de la blockram (es decir el código del .elf).

## 6) Subir código

Para ejecutar el código desde Vivado tenemeos que ir a la sección "Program And Debug", seleccionar la opción "open target" y luego "program target". Allí se deberá seleccionar el "design_1_wrapper.bit" generado en el paso anterior (que se encuentra dentro de la carteta "../myproject.runs/impl_1")