# Relatório de Banco de Dados

## Documentação do Código fonte

### Article .hpp (Desenvolvido por Gabriel Pereira)
**Objetivo**: Esse arquivo possui a lógica de controle e manipulaçao dos artigos em diversas partes do código.

As estruturas desse arquivo são:
- Article:
  int m_id.
  int m_year.
  int m_citations.
  char m_date[DATE_SIZE].
  char m_title[TITLE_SIZE].
  char m_authors[AUTHORS_SIZE].
  char m_snippet[SNIPPET_SIZE].

Os metódos presentes nesse arquivo são:

- Article(): Construtor da classe artigo. Ela inicia os valores das variaveis de acordo com os parametros.
- char* toByteArray(): Função que faz os bytes de uma estrutura serem interpretados com um array de char.
- const int getHashCode(): Retorna o código hash do artigo, no caso o ID dele.
- const Data& getData(): Retorna o struct Data que é um struct interno da classe de artigo.
- static const int getSizeOfData(): Retorna o tamanho do struct Data.


### btree .hpp (Desenvolvido por Gabriel Maia)

**Objetivo**: Esse arquivo possui a lógica e manipulação dos indice primário ordenado pelo ID dos registros, feito por uma arvoreB.

As estruturas desse arquivo são:
- Node:
  int offset.

- Page:
  unsigned short KeyNumber.
  Nonde nodes[].
  short pointers.
  char empty[].

- Btree:
  FileInterface stream.
  short pages.
  int root.
  int pointer.

Os métodos presentes nesse arquivo são:
- Node(): Construtor do nó que vai apontar para um registro.
- Page(): Costrutor de uma página na arvore.
- Btree(string fileName): Construtor inicial da arvore em um arquivo que a função recebe de entrada.
- int search(ID): Função que faz busca dentro da arvore, recebe como parametro o ID do registro que está fazendo a busca.
- long long int insert(Node node): Insere um registro novo na arvore.
- long long int splitPage(Page& page, int offset Page, int father): Faz o split na inserçao na arvore.

### btree_title .hpp (Desenvolvido por Gabriel Maia)
**Objetivo**: Esse arquivo possui a lógica e manipulação dos indice secundario ordenado pelo titulo dos registros, feito por uma arvoreB.

As estruturas desse arquivo são:
- TitleNode:
  int offset.
  char[] title.

- TitlePage:
  unsigned short KeyNumber.
  TitleNonde nodes[].
  int pointers.
  char empty[].

- BtreeTitle:
  FileInterface stream.
  short pages.
  int root.
  int pointer.

Os métodos presentes nesse arquivo são:
- TitleNode(): Construtor do nó que vai apontar para um registro.
- PTitleage(): Costrutor de uma página na arvore.
- BtreeTitle(string fileName): Construtor inicial da arvore em um arquivo que a função recebe de entrada.
- int search(char title[]): Função que faz busca dentro da arvore, recebe como parametro o titulo do registro que está fazendo a busca.
- long long int insert(TitleNode node): Insere um registro novo na arvore.
- long long int splitPage(TitlePage& page, int offset Page, int father): Faz o split na inserçao na arvore.
short insertPage(TitlePage page, TitleNode node): Faz a inserção de uma Pagina nova na Arvore.

### file_interface .hpp (Desenvolvido por Gabriel Pereira)
**Objetivo**: Esse arquivo tem como objetivo a manipulação de escrita e leitura de blocos nos arquivos.

A estrutura desse arquivo é:

- FileInterface: 
  File *stream.

Os métodos presentes nesse arquivo são:

- void write(char* bytes, unsigned size): Função que escreve uma quantidade de bytes em um arquivo binario.
- char* read(unsigned size): Função que lê uma quantidade de bytes em um arquivo binario.
- long int getFileSize(): Função que retorna o tamanho do arquivo.
- void close(): Função que fecha o arquivo.
- bool isOpen(): Função que verifica se o arquivo está aberto.
- bool isEOF(): Função que verifica se está no final do arquivo.
- Article readRawArticle(): lê os artigos e interpreta de linha por linha para a formatação utilizada pela aplicação.
- FileInterface(): Construtor da classe FileInterface. Ela recebe o endereço do arquivo onde as operações de leitura e escrita aconteceram.
- int readColumn(*char columnFormat, void* variable, int isEndline): Lê uma coluna do arquivo base.
- void readcolumnByChar(char* variable): Lê caracter por caracter do arquivo.


### block .hpp (Desenvolvido por Gabriel Pereira)
**Objetivo**: Esse arquivo possui a lógica de controle e manipulaçao de Blocos em diversas partes do código.

A estrutura dessee arquivo é:

- FileSystemBlock:
  vhar m_content[].
  unsigned m_header.
  unsigned HEADER_MASK.

Os métodos presentes nesse arquivo são:
- bool tryAdd(char*, unsigned): Função que tenta botar uma quantidade de bytes no bloco.
- char* toByteArray(): Função que faz os bytes de um bloco serem interpretados como um array de char.
- Article getArticle(): Funçao que extrai um artigo dos bytes do bloco.
- static int getBlockSize: Função que pega o tamanho do bloco.
- FileSystemBlock(): Construtor do tipo FileSystemBlock.
- int verifySpaceAvailable(unsigned): Verifica se cabe mais bytes no bloco.