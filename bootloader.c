#include <stdint.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/resets.h"
#include "hardware/regs/resets.h"

#include "hardware/flash.h"

#include "hardware/regs/addressmap.h"



#include "hardware/irq.h"
#include "hardware/structs/scb.h"
#include "hardware/structs/xip_ctrl.h"
#include "hardware/sync.h"
#include "pico/bootrom.h"
#include "boot/picobin.h"
#include "hardware/watchdog.h"


int check_xip_enabled(void)
{
    uint32_t reg = xip_ctrl_hw->ctrl;
    
    printf("XIP_CTRL: 0x%08lX\n", reg);

    if (!(reg & XIP_CTRL_EN_BITS))
    {
        printf("XIP disabled! App vai travar se executar da flash.\n");
        return -1;
    }
    
    printf("XIP is enabled\n");
    return 0;
}


// Função para pular para o aplicativo
int jump_to_app(uint32_t app_base_address)
{
    // Endereço da tabela de vetores (256 bytes após o base address)
    uint32_t app_address = app_base_address + 0x100;

    // Verificar se há um aplicativo válido
    uint32_t *vector_table = (uint32_t *)app_address;
    uint32_t stack_pointer = vector_table[0];
    uint32_t reset_handler = vector_table[1];


    if (stack_pointer == 0xFFFFFFFF || reset_handler == 0xFFFFFFFF)
    {
        printf("Erro: Nenhum app válido no endereço 0x%08lX\n", app_address);
        return -1;
    }

    // Validação inicial: stack pointer e reset handler em endereços permitidos
    if ((stack_pointer < 0x20000000 || stack_pointer > 0x20042000) || // SP fora da RAM
        (reset_handler < 0x10000000 || reset_handler > 0x10FFFFFF)) {  // Reset handler fora da flash
        printf("Erro: app_address (0x%08lX) Stack Pointer (0x%08lX) ou Reset Handler (0x%08lX) inválidos\n",
               app_address, stack_pointer, reset_handler);
        return -1;
    }

    printf("App encontrado no endereco 0x%08lX\nConfigurando handler...\n", app_address);
    printf("\t0x%08lX (Stack Pointer)\n\t0x%08lX (Reset Handler)\n", stack_pointer, reset_handler);

    if (check_xip_enabled())
    {
        return -1;
    }

    sleep_ms(100);


    // Garantir alinhamento de 64 bytes para a tabela de vetores
    //uint32_t *vector_table = (uint32_t *)((uintptr_t)vector_table & ~0x3F);

    // Desabilitar todas as interrupções
    irq_set_mask_enabled(0xFFFFFFFF, false);
    for (int i = 0; i < NUM_IRQS; ++i)
    {
        irq_clear(i);
    }

    // Desabilitar interrupções globais
    //asm volatile ("cpsid   i");  // Desabilitar interrupções

    // Configurar o stack pointer
    asm volatile ("msr msp, %0" : : "r" (stack_pointer) : );

    // Configurar o vetor de interrupções
    scb_hw->vtor = (uintptr_t)vector_table;
    asm volatile("dsb");
    asm volatile("isb");

    //asm volatile ("cpsie   i");  // Desabilitar interrupções

    // Pular para o reset handler (forçar modo Thumb)
    asm volatile("bx %0" : : "r" (reset_handler) : );

    //  ((void (*)(void))(reset_handler))();

    // Nunca deve chegar aqui
    return 0;
}


int main(void)
{
    stdio_init_all();
    sleep_ms(5000);

    int result = jump_to_app(0x10080000);
    
    // Se jump_to_app retornar, houve um erro
    if (result != 0)
    {
        printf("Falha ao iniciar o aplicativo!\n");
        while (1) {
            // Loop infinito ou tratamento de erro
            tight_loop_contents();
        }
    }

    // Nunca deve chegar aqui
    return 0;
}


