#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h" // Necessário para controlar o LED da Pico W
#include "hardware/watchdog.h" // Necessário para reinicialização

// --- Funções de Ação ---

void print_hello_world_c() {
    printf("--------------------\n");
    printf("Ola Mundo!\n");
    printf("Hello World!\n");
    printf("--------------------\n");
    sleep_ms(1000);
}

void piscar_led_c() {
    printf("Iniciando a sequencia de piscar o LED (5x)...\n");
    
    // Inicializa o driver do chip Wi-Fi (CYW43)
    if (cyw43_arch_init()) {
        printf("Falha ao inicializar CYW43.\n");
        return;
    }

    for (int i = 0; i < 5; i++) {
        // CYW43_WL_GPIO_LED_PIN é o pino do LED da Pico W
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1); // Acende
        sleep_ms(500); // 0.5s
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0); // Apaga
        sleep_ms(500); // 0.5s
    }
    
    // Desinicializa o CYW43 para liberar o recurso
    cyw43_arch_deinit();
    printf("LED parou de piscar.\n");
}

void print_logo(){
    // Garantindo que a saída seja limpa
    fflush(stdout);
    printf("⠀⢀⣠⣤⣶⣶⣶⣤⣄⠀⠀⣀⣤⣶⣶⣶⣤⣄⡀⠀\n");
    printf("⠀⢸⣿⠁⠀⠀⠀⠀⠙⢷⡾⠋⠀⠀⠀⠀⠈⣿⡇⠀\n");
    printf("⠀⠘⢿⡆⠀⠀⠀⠢⣄⣼⣧⣠⠔⠀⠀⠀⢰⡿⠃⠀\n");
    printf("⠀⠀⠈⠻⣧⣤⣀⣤⣾⣿⣿⣷⣤⣀⣤⣼⠟⠁⠀⠀\n");
    printf("⠀⠀⣰⡾⠋⠉⣩⣟⠁⠀⠀⠈⣻⣍⠉⠙⢷\n");
    printf("⠀⢀⣿⣀⣤⡾⠛⠛⠷⣶⣶⠾⠛⠛⢷⣤⣀⣿⡀⠀\n");
    printf("⣰⡟⠉⣿⡏⠀⠀⠀⠀⢹⡏⠀⠀⠀⠀⢹⣿⠉⢻⣆\n");
    printf("⣿⡇⠀⣿⣇⠀⠀⠀⣠⣿⣿⣄⠀⠀⠀⣸⣿⠀⢸⣿\n");
    printf("⠙⣷⣼⠟⠻⣿⣿⡿⠋⠁⠈⠙⢿⣿⣿⠟⠻⣧⣾\n");
    printf("⠀⢸⣿⠀⠀⠈⢿⡇⠀⠀⠀⠀⢸⡿⠁⠀⠀⣿⡇⠀\n");
    printf("⠀⠀⠻⣧⣀⣀⣸⣿⣶⣤⣤⣶⣿⣇⣀⣀⣼⠟⠀⠀\n");
    printf("⠀⠀⠀⠈⠛⢿⣿⣿⡀⠀⠀⢀⣿⣿⡿⠛⠁⠀⠀⠀\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠙⠻⠿⠿\n");
    sleep_ms(3000); // Pausa para visualização do logo
}

void print_logo_PO(){
    print_logo();
    printf("    Power OFF / Reset     \n");
    sleep_ms(900);
}

void exibir_menu_c() {
    // 🛑 NOVO CÓDIGO: Tenta limpar a tela com ANSI, com fallback de pulos de linha
    
    // Tenta limpar a tela usando a Sequência de Escape ANSI (Método 1: Mais limpo, se suportado)
    printf("\033[2J\033[H");
    fflush(stdout); // Garante que o comando ANSI seja enviado imediatamente

    // Método 2 (Fallback): Imprime 50 linhas vazias para forçar o scroll da tela
    // Se o ANSI falhar, isso simula uma limpeza na maioria dos terminais.
    for (int i = 0; i < 50; i++) {
        printf("\n");
    }
    fflush(stdout);

    // Fim da lógica de limpeza


    printf("\033[2J\033[H");
    printf("\n" "==============================\n");
    printf(" ▖  ▖        ▄▖▘      ▖  ▖ \n");
    printf(" ▛▖▞▌█▌▛▌▌▌  ▙▌▌▛▘▛▌  ▌▞▖▌ \n");
    printf(" ▌▝ ▌▙▖▌▌▙▌  ▌ ▌▙▖▙▌  ▛ ▝▌ \n");
    printf("                              \n"); 
    printf("==============================\n");
    printf("1 -  Imprimir 'Hello World'\n");
    printf("2 -  Piscar o LED Integrado\n");
    printf("3 -  Raspberry logo\n");
    printf("0 -  Reiniciar (Reset)\n"); 
    printf("------------------------------\n");
}

int main(void) {
    // Inicializa USB (Stdio USB)
    stdio_init_all(); 

    // 🛑 CORREÇÃO: Espera Ativa pela Conexão USB (Minicom/Terminal)
    // Isso garante que o terminal esteja pronto para receber o menu.
    while (!stdio_usb_connected()) {
        // Pausa brevemente para evitar travar a CPU
        sleep_ms(100); 
    }
    // --------------------------------------------------------------------

    int escolha_char = -1; 

    while (1) {
        exibir_menu_c();
        printf("Escolha uma opcao (0, 1, 2 ou 3): ");
        fflush(stdout); 
        
        // --- Leitura de Entrada ROBUSTA usando getchar() ---
        
        // 1. Tenta ler o primeiro caractere (o '1', '2', '3' ou '0')
        escolha_char = getchar();
        
        // 2. Limpa o buffer: consome todos os caracteres restantes (o '\r' e o '\n' do ENTER)
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            if (c == '\r') {
                break; 
            }
        }
        
        
        // 3. Processa a escolha
        switch (escolha_char) {
            case '1':
                print_hello_world_c();
                break;
            case '2':
                piscar_led_c();
                break;
            case '3' :
                print_logo();
                break; 
            case '0':
                printf("Reiniciando a Pico W via Watchdog...\n");
                print_logo_PO(); 
                watchdog_reboot(0, 0, 100); 
                while(1) { tight_loop_contents(); }
                break; 
            default:
                if (escolha_char != EOF && escolha_char != '\r' && escolha_char != '\n') {
                    printf("Opcao invalida ('%c'). Por favor, digite 0, 1, 2 ou 3.\n", escolha_char);
                }
                sleep_ms(1000);
                break;
        }
    }
}