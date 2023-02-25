#include "include/main.h"

static void setup(){
    setup_uart();
    setup_gpio();

    /* enable interrupts */
    sei();
}

int main(){

    char x[50] = {0};
    setup();
    while(1){
    }
}
