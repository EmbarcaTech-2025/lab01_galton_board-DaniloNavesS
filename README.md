
# Projetos de Sistemas Embarcados - EmbarcaTech 2025

Autor: Danilo Naves do Nascimento

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Brasília, maio de 2025

---

Este projeto simula uma Galton Board (ou Quadro de Galton) utilizando o microcontrolador Raspberry Pi Pico e um display OLED 128x64. A simulação representa o movimento de várias bolas que descem por níveis com obstáculos, desviando aleatoriamente para a esquerda ou direita, até caírem em uma das casas inferiores. O sistema desenha o tabuleiro, anima as bolas em tempo real e exibe a quantidade de bolas que caíram em cada casa.

---

## Versões

**Versão 1 — Queda única**
Nesta versão inicial, o sistema simula a queda de uma única bola por vez. Assim que o usuário inicia a simulação (por exemplo, ao resetar o dispositivo), uma bola percorre o tabuleiro linha por linha, desviando aleatoriamente para a esquerda ou para a direita nos obstáculos. Ao chegar ao final, a casa correspondente é atualizada e exibida no terminal.

**Versão 2 — Queda contínua com múltiplas bolas**
Na segunda versão, o sistema evolui para simular a queda contínua de várias bolas. Uma nova bola é lançada automaticamente assim que a anterior atinge o final do tabuleiro, permitindo observar a formação de uma distribuição probabilística em tempo real. O número da bola atual é exibido no canto do display OLED e a contagem de bolas por casa é atualizada dinamicamente no terminal. Além disso, é possível ver um **histograma** no display oled.


## 📜 Licença
MIT License - MIT GPL-3.0.

