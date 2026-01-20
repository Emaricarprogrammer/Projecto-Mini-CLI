# 📘 Projeto de Programação I  
**Licenciatura em Ciências da Computação**

---

## 🧑‍💻 Documentação elaborada por:
**Emanuel António — Analista e desenvolvedor principal**

---

## 🎯 Objetivo

Desenvolver uma **mini CLI (Command Line Interface)** em linguagem **C** para auxiliar na **inclusão digital** e **contextualização da Licenciatura em Ciências da Computação**, oferecendo funcionalidades de **gestão da turma**.

---

## 💻 Linguagem de Programação
**C**

---

## ⚙️ Requisitos Funcionais

### 🔹 Download de Conteúdos Programáticos:
- O programa deve permitir o **download de conteúdos** disponibilizados em um **repositório GitHub**.  
- Cada **cadeira (disciplina)** corresponderá a uma **branch** do repositório.  
- O usuário poderá escolher qual disciplina deseja baixar, e o sistema executará o comando `git pull` para obter o conteúdo atualizado.

---

### 🔹 Geração de Grupos Aleatórios:
- O programa deverá armazenar os **nomes e emails dos alunos da turma**.  
- Com base nessa lista, serão gerados **grupos aleatórios de tamanho configurável**.  
- Após a criação dos grupos, o sistema deverá enviar uma **notificação via email (SMTP)** para cada aluno, informando seu grupo e colegas.

## 🧩 Descrição Geral do Projeto

O sistema será implementado como uma **interface de linha de comando (CLI)** interativa, onde o usuário poderá navegar pelas opções do menu principal:

```
=== Gestão da Turma ===
1 - Download de conteúdos
2 - Gerar grupos aleatórios
3 - Eleger delegado(a)
4 - Sair
```

---

### 🔹 1. Download de Conteúdos

Os conteúdos estarão hospedados em um **repositório GitHub da turma**.  
Cada cadeira será representada por uma **branch distinta**, e o sistema permitirá que o usuário selecione qual deseja baixar.  
A CLI executará internamente o comando:

```
git pull origin <branch>
```

para obter os materiais mais recentes.

---

### 🔹 2. Geração de Grupos Aleatórios

- O programa solicitará uma **lista de alunos (nome e email)**.  
- A lista será armazenada em um arquivo texto (`alunos.txt`) ou em uma estrutura de dados na memória.  
- O **algoritmo de embaralhamento Fisher–Yates** será utilizado para formar os grupos aleatórios.  
- Após a formação, o sistema enviará **emails automáticos via SMTP** notificando os alunos sobre seus grupos.

---

### 🔹 3. Eleição de Delegado(a)

- Caso nenhum aluno manifeste interesse em ser delegado(a), o sistema poderá realizar um **sorteio aleatório** entre os alunos cadastrados.  
- O nome do delegado será exibido no terminal e também poderá ser gravado em um arquivo `delegado.txt`.

---

## 🧠 Estrutura Sugerida do Projeto

```
turma-cli/
│
├── src/
│   ├── main.c
│   ├── alunos.c
│   ├── grupos.c
│   ├── github.c
│   ├── email.c
│   ├── menu.c
│
│
├── include/
│   ├── alunos.h
│   ├── grupos.h
│   ├── github.h
│   ├── email.h
│   ├── menu.h
│   └── utils.h
│
├── data/
│   ├── alunos.txt
│   ├── grupos.txt
│   └── delegado.txt
│
├── Makefile
└── README.md
```

---

## 📧 Funcionalidade Extra (Envio de Email via SMTP)

Para o envio das notificações, será utilizada a biblioteca **libcurl** ou outra equivalente, garantindo a comunicação via **SMTP**.

---

## 🧾 Conclusão

O projeto visa **integrar conceitos fundamentais da linguagem C** — como estruturas de dados, modularização, entrada/saída, aleatoriedade e integração com o sistema — em um **contexto prático e colaborativo**.

A aplicação proporciona uma experiência de aprendizado mais realista e voltada para o trabalho em equipe, reforçando boas práticas de desenvolvimento e organização de código.

---

## 📜 Licença
Este projeto é de caráter **acadêmico** e faz parte da unidade curricular **Programação I** do curso de **Licenciatura em Ciências da Computação**.