Author: Daltro Oliveira Vinuto
Mat: 160025966

O trabalho esta zipado na pasta work_1 e esta dividido em duas pastas:
1. a pasta assembler com o codigo do montador e seu proprio makefile;
2. a pasta simulator com o codigo do simulador e seu proprio makefile;

Ambos assembler e simulador sao divididos 4 pastas:
1. pasta assets(contendo arquivos de entrada e saida dos programas);
2. pasta src(contendo codigo c++);
3. pasta include(contendo cabecalhos .h);
4. pasta bin(que contera o arquivo executavel).

Apos a montagem o arquivo objeto( .obj) deve ser movido para a pasta adequada para a simulacao. Entao teremos dois casos:
1. Caso tenha sido usado o make load para executar o montador, sua variavel MAIN_ARGUMENTS alterada e o arquivo assembly inserida na pasta assets entao o arquivo objeto sera gerado tambem na pasta assets entao para move-la para a pasta assets/bin do simulador basta de dentro da pasta work_1 digitar: cp ./assembler/assets/nome_do_arquivo_objeto.obj ./simulator/assets && cp ./assembler/assets/nome_do_arquivo_objeto.obj ./simulator/bin

2. Caso nao tenha sido usado o make load para executar o montador entao o arquivo objeto estara dentro da pasta bin do assembler entao para move-la para a pasta assets/bin do simulador basta de dentro da pasta work_1 digitar: cp ./assembler/bin/nome_do_arquivo_objeto.obj ./simulator/assets && cp ./assembler/bin/nome_do_arquivo_objeto.obj ./simulator/bin

O programa deve ser compilado, montado, ligado e carregado em linux usando o GCC;

Referente ao montador ========================================== :


	Para compilar basta entrar na pasta assembler e digitar: make

	Para executar o montador com um arquivo de teste especifico ha duas opcoes:

	1. Insira o arquivo assembly na pasta assets e altere a variavel do makefile MAIN_ARGUMENTS e digite: make load
	Nesse caso o arquivo objeto e .pre e salvo dentro da pasta assets tambem.

	2. Insira o arquivo assembly "nome_do_arquivo.asm" na pasta bin, entre nessa pasta e digite:
	./montador nome_do_arquivo.asm
	Nesse caso o arquivo objeto e .pre sao salvos dentro da pasta bin tambem.

Obs.:
	1. O arquivo preprocessado .pre sera criado independente de haver erros ou nao semelhante ao comportamento do gcc;

	2. O arquivo assembly deve necessaria ter a extensao: .asm ou .mcr;

	3. Para multiplicacao o opcode utilizado sera: mul;

	4. A expansao da macro e feita mesmo quando a extensao e .asm mas nesse caso nao e gerado o arquivo com extensao ".pre";

	5. O arquivo .pre e gerado e salvo na pasta assets;

	6. O arquivo objeto e gerado e salvo na pasta assets;

	7. O programa deve ser NECESSARIAMENTE ser dividido em areas de secao text e secao data caso contrario o montador acusara um erro e nao montara. A ordem das secoes nao importa conforme especificao do trabalho.

	8. O copy pode ou nao conter espaco entre a virgula e os operandos mas o rotulo de array mantem a definicao do trabalho e nao pode conter espaco entre o rotulo e a constante.


Referente ao simulador ============================================ :


	Para compilar basta entrar na pasta simulator e digitar: make

	Para executar o simulador com um arquivo de teste especifico ha duas opcoes:

	1. Insira o arquivo objeto na pasta assets e altere a variavel do makefile MAIN_ARGUMENTS e digite: make load

	2. Insira o arquivo objeto "nome_do_arquivo.obj" na pasta bin, entre nessa pasta e digite:
	./simulador nome_do_arquivo.obj


Obs.:
	1.	Para fins de verificacao de segmentation fault estou considerando que a secao text NECESSARIAMENTE deve terminar com a instrucao stop. As razoes para escolha sao as seguintes: 1. Processadores sao interrompidos ou por uma interrupcao(signal como int 0x80) ou por uma instrucao(HALT) e como estamos considerando que o nosso simulador nao aceita signals entao
	ele deve parar uma instrucao, a instrucao 'stop' caso contrario teriamos o risco do programa tentar executar a secao data se por acaso houvesse um const com valor de um opcode valido. 2. Para verificar se ha segmentation fault tarefa do SO precisamos saber quando a secao de text acaba e como nosso modulo de carga nao possui cabecalho contendo a posicao de fim da secao text nao teriamos como saber quando encerrar o processo, pois se continuassemos incrementando o PC poderiamos entrar na secao de data e encontrar uma const com o mesmo valor de um opcode valido e acabar executando uma instrucao invalida e portanto nao teriamos como saber em qual momento o PC nao assume o valor de um dado e nao de um opcode e mesmo fazendo uma passagem previa e ordenando os argumentos ocorreriamos o risco de ler um endereco para um label que nao existe no codigo e poderia ser o caso do menor endereco usado no programa ja seja de um valor que ja esta na secao data ou esta fora da area de memeoria do programa inteiro(endereco superior que o fim da secao data), essa e a razao pela qual os SO comerciais usam cabecalho no modulo de carga caso contrario ficaria a cargo do programador verificar os enderecoes para posicoes validas e invalidas e e isso que espero nos testes ao colocarmos um stop no fim da secao text.

	2. Caso o STOP nao seja colocado no fim da secao text entao pode ocorrer um falso segmentation fault e o programa encerrar com erros, portanto apresentando comportamento inesperado.
