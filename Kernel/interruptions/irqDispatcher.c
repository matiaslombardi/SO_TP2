#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>
#include <videoDriver.h>
#include <scheduler.h>

#include <lib.h>

static uint64_t * int_20(uint64_t *rsp);

static uint64_t * int_21(uint64_t *rsp);


uint64_t * irqDispatcher(uint64_t irq, uint64_t *rsp) {
    switch (irq) {
        case 0:
            return int_20(rsp);
        case 1:
            return int_21(rsp);
    }
    return rsp;
}

uint64_t * int_20(uint64_t *rsp) {
    timer_handler();
    return rsp;
}

uint64_t * int_21(uint64_t *rsp) {
    keyboard_management(rsp);
    return rsp;
}