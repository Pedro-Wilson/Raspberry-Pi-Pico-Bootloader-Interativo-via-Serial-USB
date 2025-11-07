🚀 Raspberry Pi Pico: Bootloader Interativo via Serial USB

Descrição: Bootloader interativo e robusto para Raspberry Pi Pico. Apresenta um menu CLI otimizado com sincronização serial, códigos ANSI para interface gráfica e resolução completa de desafios de I/O e sincronização de terminal.

O objetivo deste projeto é demonstrar o controle de periféricos, gerenciamento de entrada/saída (I/O) e o domínio das funcionalidades de baixo nível do Pico SDK através de um menu interativo e visualmente aprimorado.

✨ Destaques Técnicos

Feature	Descrição e Solução Implementada
I/O Resiliente	Implementação de lógica de limpeza de buffer (getchar()) para garantir que o caractere 'Enter' (\r ou \n) não seja lido como um comando inválido, tornando a entrada de dados 100% confiável.
Sincronização Ativa	Utilização de while (!stdio_usb_connected()) para forçar o Pico a esperar a conexão do terminal (/dev/ttyACM0), eliminando falhas de timing e o problema de "menu que abre na sorte".
Performance	Configuração do baud rate para 921600 via CMake, otimizando a velocidade de comunicação serial.
Gráficos (ANSI)	Uso extensivo de códigos de escape ANSI para cores, fundo, negrito e limpeza de tela, proporcionando uma experiência de usuário limpa e profissional.
Controle de Hardware	Demonstração do controle do LED onboard e da função de reset via Watchdog.

📋 Funcionalidades do Menu

Opção	Comando	Descrição
1	printf()	Imprime mensagens de teste ("Hello World" / "Olá Mundo!").
2	cyw43_arch_gpio_put	Executa uma sequência de piscar do LED integrado da placa.
3	printf()	Exibe um logotipo gráfico (ASCII Art) do Raspberry Pi no terminal.
0	watchdog_reboot	Reinicia o Raspberry Pi Pico via Watchdog, forçando uma reinicialização limpa do sistema.

🛠️ Configuração, Build e Execução

1. Pré-requisitos

Certifique-se de que o Raspberry Pi Pico SDK e o ambiente de desenvolvimento (cmake, ninja e toolchain ARM GCC) estejam configurados em seu sistema operacional.

2. Configuração do Baud Rate (CMake)

A velocidade da comunicação serial USB foi ajustada para um valor alto, sendo necessário recompilar o projeto:

    Baud Rate Definido: 921600

    Local: CMakeLists.txt (via pico_set_default_usb_serial_baud_rate(921600))

3. Comandos de Build

Execute o processo de build a partir do diretório raiz do projeto:
Bash

# 1. Cria e entra no diretório de build
mkdir build
cd build

# 2. Configura o projeto (garante que as variáveis do CMakeLists.txt sejam lidas)
cmake ..

# 3. Compila o projeto e gera o arquivo .uf2
ninja

4. Conexão e Execução

Para interagir com a placa após a gravação:

    Grave o arquivo build/bootloader.uf2 na sua Raspberry Pi Pico (pressionando BOOTSEL).

    Configure o Terminal: Abra o Minicom ou VS Code Serial Monitor e defina o baud rate para 921600.

    O menu aparecerá automaticamente. Digite a opção desejada (1, 2, 3 ou 0) e pressione Enter.