#include "include/main.h"

static void setup(){
    setup_uart();
    setup_gpio();

    /* enable interrupts */
    sei();
}

int main(){
    setup();
    while(1){
    }
}
