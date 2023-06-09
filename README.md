O IMDB disponibiliza periodicamente datasets com informações sobre a indústria cinematográfica, 
dentre eles o name.basics.tsv.gz (237 MB) e o title.basics.tsv.gz (164 MB). Os arquivos estão 
estruturados em formato TSV (tab separated values). O primeiro arquivo segue o seguinte esquema:

nconst primaryName birthYear deathYear primaryProfession knownForTitles
nm0000001 Fred Astaire 1899 1987 soundtrack,actor,miscellaneous tt0072308,tt0045537,tt0050419,tt0053137
nm0000002 Lauren Bacall 1924 2014 actress,soundtrack tt0037382,tt0117057,tt0038355,tt0075213

Embora aparentem ser espaços, os valores de cada linha estão separados pelo caracter tab (\t). A primeira 
linha é o cabeçalho. A partir da segunda linha, o valor da primeira coluna (nconst) contém um identificador 
de um artista (ator ou atriz) e o valor da última coluna (knownForTitles) contém uma sequência separada por 
vírgula de identificadores dos filmes de destaque deste artista. O segundo arquivo está organizado de maneira 
análoga, mas com identificadores, nomes e outras informações de filmes.

O objetivo deste trabalho é implementar um programa que, utilizando árvores e grafos, identifique quais 
filmes possuem artistas em comum. Para tanto, você deverá seguir algumas etapas:

(1) Implemente uma árvore rubro-negra para utilizar como estrutura de armazenamento dos artistas e dos filmes. (Se precisar você pode utilizar a AVL implementada em sala, porém isso implicará em uma penalização considerável na nota.) Efetue a leitura do arquivo dos artistas, extraia de cada linha o identificador (observe que ele pode ser convertido para inteiro caso sejam removidas as duas letras iniciais), o nome e a lista de filmes daquele artista (a lista de filmes pode ser salva em um array alocado dinamicamente ou em uma lista). Salve estes dados em uma struct apropriada e insira ela na árvore usando o identificador como chave. Para facilitar, dado que o arquivo é extenso, coloque um contador no laço de leitura para interromper a leitura quando achar necessário em seus testes.
(2) Implemente um grafo simples através de listas de adjacência para utilzar como estrutura de armazenamento dos filmes. Cada vértice do grafo deverá representar um filme. De modo semelhante à etapa anterior, efetue a leitura do arquivo de filmes, salve o identificador (como inteiro) e o nome do filme e insira-os no grafo. As linhas que não correspondam a filmes (que não possuam "movie" segunda coluna) podem ser ignoradas. Inicialmente o grafo não deve ter arestas.
(3) Visite à árvore e, para cada artista, cheque seus filmes de destaque e forme no grafo uma clique entre os vértices correspondentes aos filmes nos quais ele ou ela trabalhou. A clique é formada inserindo-se arestas (caso ainda não existam) entre todos os filmes em questão (caso existam) no grafo.
(4) Visite o grafo e em sua visita imprima a estrutura do grafo no formato DOT Language, desta vez utilizando os nomes dos filmes como identificadores, tal como no exemplo a seguir:

graph {

"Matrix" -- "Matrix Reloaded";

"Matrix" -- "John Wick";

"Titanic" -- "Inception";

"Inception" -- "Dark Knight Rises"

}

Se este texto for impresso em um arquivo no formato DOT (ex: input.dot), ele pode ser transformado 
em imagem no terminal pelo comando: dot -Tsvg input.dot > output.svg

O trabalho deve ser feito em duplas ou trios, deve ser implementado em linguagem C e deve ser 
submetido nesta página até o final do dia 21/06. Não serão aceitas entregas que ultrapassem o prazo. 
As defesas ocorrerão durante a aula do dia 21/06. Cada grupo terá até 10 minutos para apresentar seu 
trabalho ao professor. A atribuição de nota será individual e apenas para os integrantes presentes. 
Como parte da avaliação o professor poderá solicitar explicações e modificações no código para qualquer 
um dos integrantes durante a defesa. Os códigos devem compilar e rodar normalmente nas máquinas disponíveis 
no laboratório. Instruções adicionais poderão ser dadas ao longo das próximos aulas e/ou acrescentadas 
nesta especificação.