
# Projetos de Sistemas Embarcados - EmbarcaTech 2025

Autor: Danilo Naves do Nascimento

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Brasília, maio de 2025

---

Este projeto simula uma Galton Board (ou Quadro de Galton) utilizando o microcontrolador Raspberry Pi Pico e um display OLED 128x64. A simulação representa o movimento de várias bolas que descem por níveis com obstáculos, desviando aleatoriamente para a esquerda ou direita, até caírem em uma das casas inferiores. O sistema desenha o tabuleiro, anima as bolas em tempo real e exibe a quantidade de bolas que caíram em cada casa.

---

## Vídeo-apresentação
![Apresentacao](assets/apresentacaoGalton.gif)

## Versões

**Versão 1 — Queda única**
Nesta versão inicial, o sistema simula a queda de uma única bola por vez. Assim que o usuário inicia a simulação (por exemplo, ao resetar o dispositivo), uma bola percorre o tabuleiro linha por linha, desviando aleatoriamente para a esquerda ou para a direita nos obstáculos. Ao chegar ao final, a casa correspondente é atualizada e exibida no terminal.

**Versão 2 — Queda contínua com múltiplas bolas**
Na segunda versão, o sistema evolui para simular a queda contínua de várias bolas. Uma nova bola é lançada automaticamente assim que a anterior atinge o final do tabuleiro, permitindo observar a formação de uma distribuição probabilística em tempo real. O número da bola atual é exibido no canto do display OLED e a contagem de bolas por casa é atualizada dinamicamente no terminal. Além disso, é possível ver um **histograma** no display oled.

**Versão 3 - Balanceamento com botões físicos**

Nesta versão, o sistema permite **controlar o viés da direção** das bolas durante a simulação da Galton Board.
O balanceamento é feito em tempo real, por meio dos **botões físicos da placa**:

* **Botão A pressionado** → maior chance da bola ir para a **esquerda**.
* **Botão B pressionado** → maior chance da bola ir para a **direita**.
* **Nenhum botão pressionado** → comportamento **neutro**, 50% de chance para cada lado.

O algoritmo ajusta a probabilidade de cada movimento conforme o botão lido no início da queda de cada bola, resultando em uma distribuição assimétrica no histograma.


## 📜 Licença
MIT License - MIT GPL-3.0.

