# Implementacao_multi_lista_arqbin
Este é um projeto feito para implementar os conceitos estudados de multi lista com manipulação de arquivos binários.
 
 ## 🚀 Funcionalidade
 O projeto contém dois códigos que devem ser rodados na seguinte ordem: ArquivoBinario.c (é onde criamos o arq.bin onde armazenamos o código do produto, sigla do local de origem do produto e se tem em estoque).
 O segundo código é utilizado para fazer consultas de índices (indice.bin) no arq.bin para localizar o produto desejado.

## 💻 Tecnologia utilizada
- C: Linguagem de programação principal.
- Arquivo binário.

 ## ⚙ Como funciona
O objetivo do projeto é estabelecer um estoque, registrando os produtos dentro de um arquivo binário e conseguindo realizar pesquisas através de índices em outro arquivo binário.
- É necessário que o ArquivoBinario.c seja executado primeiro, para que o arq.bin seja criado corretamente.
- Menu ArquivoBinario.c:
1. Inserir: Insere um novo produto no arquivo binário. O produto é separado em: código "1234", sigla "RJ", disponibilidade no estoque "1" e preço "5.99".
2. Mostrar todos: Mostra todos os produtos presentes no arquivo binário.
3. Recriar arquivo: Recria o arq.bin, limpando todos os produtos antigos e zerando o estoque.
0/ESQ. Sair.

- Menu IndiceSecundario.c:
0. Sair.
1. Criar/Recriar arquivo de indice: Cria ou recria o arquivo de indices para consultas.
2. Imprimir conteudo do indice: Imprime os indices presente no indice.bin
3. Consultar produtos por sigla e disp: Consulta um produto pela sigla "RJ" e por sua disponibilidade "1".

## 🆘 Ajuda

Apesar de funcionar, o projeto tem um bug com os códigos dos produtos. Os códigos aparecem com valor 0 caso o usuário escolha um número com menos de 4 digitos. 
Além disso, quando fornecido um valor com mais digitos, o código aparece diferente do número desejado. Não sei o que poderia estar causando isso,
utilizei o fflush(stdin) para tentar limpar qualquer lixo possível de entrada, mas mesmo assim o erro persistiu. Caso queira ajudar, sintaa-se livre para modificar o código.
