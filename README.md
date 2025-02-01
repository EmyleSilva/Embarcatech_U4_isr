# Projeto de Controle de LEDs e Botões com RP2040

Este projeto foi desenvolvido para consolidar o entendimento sobre o uso de interrupções no microcontrolador RP2040, utilizando a placa de desenvolvimento BitDogLab. O objetivo é controlar LEDs comuns e LEDs endereçáveis WS2812, além de implementar funcionalidades com botões, incluindo o tratamento de debouncing via software.

## Objetivos
- Compreender o funcionamento e a aplicação de interrupções em microcontroladores.
- Implementar debouncing via software para corrigir o fenômeno de bouncing em botões.
- Controlar LEDs comuns e LEDs endereçáveis WS2812.
- Utilizar resistores de pull-up internos em botões de acionamento.
- Desenvolver um projeto funcional que integre hardware e software.

## Descrição do Projeto
O projeto utiliza os seguintes componentes conectados à placa BitDogLab:
- **Matriz 5x5 de LEDs WS2812** (endereçáveis), conectada à GPIO 7.
- **LED RGB**, conectado à GPIO 13.
- **Botão A**, conectado à GPIO 5.
- **Botão B**, conectado à GPIO 6.

### Funcionalidades
1. **LED RGB**:
   - O LED vermelho do LED RGB pisca continuamente a uma frequência de 5 vezes por segundo.

2. **Botão A**:
   - Decrementa o número exibido na matriz de LEDs WS2812 cada vez que é pressionado.

3. **Botão B**:
   - Incrementa o número exibido na matriz de LEDs WS2812 cada vez que é pressionado.

4. **Matriz de LEDs WS2812**:
   - Exibe números de 0 a 9 em um formato fixo.

## Como Compilar e Executar
1. **Pré-requisitos**:
   - Ter o ambiente de desenvolvimento para o Raspberry Pi Pico configurado (compilador, SDK, etc.).
   - CMake instalado.

2. **Compilação**:
   - Clone o repositório ou baixe os arquivos do projeto.
   - Navegue até a pasta do projeto e crie uma pasta de build:
     ```bash
     mkdir build
     cd build
     ```
   - Execute o CMake para configurar o projeto:
     ```bash
     cmake ..
     ```
   - Compile o projeto:
     ```bash
     make
     ```

3. **Upload para a placa**:
   - Conecte o Raspberry Pi Pico ao computador.
   - Copie o arquivo `.uf2` gerado para a placa.

## Simulação no Wokwi
Para visualizar a simulação do projeto no Wokwi:
1. Instale e configure o simulador wokwi seguindo as instruções encontradas no link abaixo:
[Introdução ao Wokwi para VS Code](https://docs.wokwi.com/pt-BR/vscode/getting-started)
2. Abra o arquivo diagram.json no vsCode.
3. Clique em "start simulation". 

## Vídeo de Demonstração
Video demonstrativo do projeto: [Link para o video](https://youtu.be/fasQcL8gpOk)


