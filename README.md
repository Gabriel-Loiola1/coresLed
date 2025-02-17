## 🟦🟥🟪Projeto: coresLed
# Descrição
O intuito desse projeto é demonstrar na prática o uso em conjunto dos componentes LED SDM RGB, Botões Push Buttons (A e B), Disploy OLED e Buzzer, por meio da placa de circuito integrado Raspberry Pi Pico W.
# Funcionamento
Ao ligar a placa, um menu é exibido no Display apresentando três opções ("A para Azul | B para Vermelho | A e B para Roxo").
1) A para Azul:
   Ao pressionar o botão A, o LED emite uma luz azul e a mensagem "AZUL" é exibida no display, enquanto isso o buzzer emite um som por 1 segundo (repetindo o toque até o botão deixar de ser pressionado).
2) B para Vermelho:
   Ao pressionar o botão B, o LED emite uma luz vermelha e a mensagem "VERMELHO" é exibida no display, enquanto isso o buzzer emite um som por 1 segundo (repetindo o toque até o botão deixar de ser pressionado).
3) A e B para Roxo:
   Ao pressionar ambos os botões, o LED emite uma luz azul e uma luz vermelha, gerando uma luz roxa e a mensagem "ROXO" é exibida no display, enquanto isso o buzzer emite um som por 1 segundo (repetindo o toque até os botões deixarem de serem pressionados).
# Componentes Necessários
1) Hardware:
- Placa de Desenvolvimento BitDogLab
- Placa de Circuito Integrada Raspberry Pi Pico W
- Botões Push Button (A e B)
- LED SMD RGB
- Display OLED
- Buzzer
2) Software:
- SDK do Raspberry Pi Pico W
3) Linguagem utilizada:
- C
# Execução do código
1) Inicializa os componentens necessários (LED, Botões, Display, Buzzer)
2) Declara as estruturas necessárias para o uso do repositório do Display OLED.
3) Inicia um loop e cada inicio o buffer do ssd1306 é resetado.
4) Verifica qual ou quais botões foram pressionados e executa as funcionalidades já descritas.
5) Caso nenhum botão tenha sido pressionado, mantém o LED apagado e exibe o menu no display.
6) Aplica um delay de 100 ms para evitar debounce.
