GLOBAL sysTime
GLOBAL sysRead
GLOBAL sysWrite

GLOBAL _syscall
GLOBAL invalidOpCode

GLOBAL yield

;------------------------------------------------------------
;                       USERLAND
;------------------------------------------------------------
section .text
;------------------------------------------------------------
; syscall recibe argumentos variables. 
;------------------------------------------------------------
_syscall:
    int 80h;
    ret
;------------------------------------------------------------

invalidOpCode:
    UD2
    ret

;TODO Modificar para que llame a una interrupcion que cambie de contexto de una, sin importar prioridad.
yield:
    int 20h;
    ret
