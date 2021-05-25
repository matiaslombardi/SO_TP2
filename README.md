# SO_TP2
Trabajo Práctico Nr.2: Construcción del Núcleo de un Sistema Operativo y estructuras de administraciónde recursos de la materia 72.11 - Sistemas Operativos - Primer Cuatrimestre 2021

## Autores:
-Arce Doncella , Julian Francisco 60509                         
-Domingues, Paula Andrea 60148                                
-Lombardi, Matias Federico 60527

## Introducción
Este documento contiene un manual básico respecto de los comandos ofrecidos al usuario para desenvolverse en la terminal.

## Syscalls
A continuación, se encuentra un cuadro de las syscalls provistas al usuario.<br>

|Number|Syscall Name		|Call	|First			|Second		|Third		|Fourth	|Fifth		|Sixth		|Seventh	|
|-------|----------------------|-------|----------------------|----------------------|--------------|---------------|--------------|--------------|---------------|
|0	    |read			        |0x00	|unsignedintfd		|char*buffer		|intlength	|-	-	|-	-	|		|		|
|1	    |write			        |0x01	|unsignedintfd		|char*buffer		|introw	|intcol	|intcolor	|-	-	|		|
|2	    |draw			        |0x02	|int*matrix		|introw		|	intcol	|introws	|intcolumns	|-	-	|		|
|3	    |clear			        |0x03	|-			|-	-	-	|-		|		|		|		|		|
|4	    |getElapsedTicks	    |0x04	|-			|-			|	-	|-	-	|-	-	|		|		|
|5	    |exit			        |0x05	|uint64_tvalue		|			|	-	|-	-	|-	-	|		|		|
|6	    |getRegister		    |0x06	|uint64_t*registers	|	-		|	-	|-	-	|-		|-		|		|
|7	    |memDump		        |0x07	|char*dir		|char*dump		|-	-	|-	-	|-		|		|		|
|8	    |getTime		        |0x08	|dateDate		|-	-		|-	-	|-	-	|		|		|		|
|9	    |setAlarm		        |0x09	|intlength		|uint64_tenabled	|	-	|-	-	|-		|-		|		|
|10	    |screenHeight		    |0x10	|-			|-			|	-	|-	-	|-	-	|		|		|
|11	    |screenWidth		    |0x11	|-			|-			|	-	|-	-	|-		|-		|		|	
|12	    |processesList		    |0x12	|-			|-			|	-	|-	-	|-	-	|		|		|
|13	    |createProcess		    |0x13	|uint64\_t*entryPoint	|intforeground		|uint64_t fdIn	|uint64_t fdOut|uint64_t first|uint64_t second|uint64_t third|
|14	    |kilProcess		        |0x14	|unsignedintpid	|	-		|-		|-		|	-	|		|		|
|15	    |changeProcessPriority	|0x15	|unsignedintpid	|	intpriority	|	-	|-	-	|-	-	|		|		|
|16	    |changeProcessState 	|0x16	|unsignedintpid	|	-		|-	-	|-	-	|-		|		|		|
|17	    |getPID		            |0x17	|	-		|	-		|-	-	|-	-	|-		|		|		|
|18	    |getMem		|0x18	|uint64_tsize		|-			|-	-	|-	-	|		|-		|		|
|19	    |freeMem		|0x19	|void*ptr		|-			|	-	|-	-	|-	-	|		|		|
|20	    |getMemInfo		|0x20	|char*info		|-			|	-	|-	-	|-	-	|		|		|
|21	    |semOpen		|0x21	|char*semId		|uint64_tinitialValue	|	-	|-	-	|-	-	|		|		|
|22	    |semClose		|0x22	|char*semId		|-			|-	-	|-	-	|-		|		|		|
|23	    |semPost		|0x23	|char*semId		|-			|-	-	|-	-	|-		|		|		|
|24	    |semWait		|0x24	|char*buffer		|-			|-	-	|-	-	|-		|		|		|
|25	    |semInfo		|0x25	|intfd[2]		|-			|-	-	|-	-	|-		|		|		|
|26	    |pipeOpen		|0x26	|intfd			|-			|-	-	|-	-	|-		|		|		|
|27	    |pipeClose		|0x27	|char*buffer		|-			|-	-	|-	-	|-		|		|		|
|28	    |waitPid		|0x28	|intpid		|-			|-	-	|-	-	|-		|		|		|