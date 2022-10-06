#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <time.h>

#include "graph.h"

// primeiros 3 grupos 3x3, seguidos
int initial_groups[3][9] = {
	{0,1,2, 9,10,11, 18,19,20},
	{27,28,29, 36,37,38, 45,46,47},
	{54,55,56, 63,64,65, 72,73,74}
};


// verifica se um determinado índice está em um grupo (geralmente) 9x9
bool index_in_group(int *group, int size, int index){
	int i = 0;
	//percorre os íncides
	for (i = 0; i < size; i++)
		if (group[i] == index)
		//O índice pertence a o grupo[i]
			return true;
	//O índice não pertence a o grupo[i]
	return false;
}

// Vinculá um vértice no índice com todos os outros vértices do grupo
void link_vertex_with_group(int *group, int size, int index, struct vertex **vertices){
	struct vertex *to_link = vertices[index];

	int i = 0;
	for (i = 0; i < size; i++)
		if (group[i] != index)
			add_edge(to_link, vertices[group[i]]);
}


// Vinculá os grupos 3x3 na lista de vértices
void link_three_groups(struct vertex **vertices){
	
	// Vinculá os grupos 3x3 dos dados iniciais
	int i = 0;
	int j = 0;
	int current_group[9];

	for (i = 0; i < 9; i++) {
		int factor = 0;
		if (i > 2 && i < 6)
			factor = 1;
		else if (i > 5)
			factor = 2;

		for (j = 0; j < 9; j++)
			current_group[j] = initial_groups[i % 3][j] + (factor * 3); 

		// Procura atraves do indice de vértices e os vinculá juntos no grupo
		for (j = 0; j < 81; j++)
			if (index_in_group(current_group, 9, j))
				link_vertex_with_group(current_group, 9, j, vertices);

	}
}

// vinculá colunas e linhas juntas 
void link_cols_and_rows(struct vertex **vertices){
	int current_row[9];
	int current_col[9];
	int i = 0;
	int j = 0;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			current_row[j] = (i * 9) + j;
			current_col[j] = (j * 9) + i;
		}
		// Procura atráves do indice de vértices e os vinculá juntos no grupo
		for (j = 0; j < 81; j++) 
			if (index_in_group(current_row, 9, j))
				link_vertex_with_group(current_row, 9, j, vertices);

		for (j = 0; j < 81; j++) 
			if (index_in_group(current_col, 9, j))
				link_vertex_with_group(current_col, 9, j, vertices);
	}
}


// Cria o grafo, carrega as vértices e os vinculá juntos no padrão de tabuleiro do Sudoku
struct graph *load_initial(){
	
	// Carrega o Sudoku do stdin()
	int puzzle_size = 9;
	int num_chars = (puzzle_size * puzzle_size) + 1;
	int num_boxes = puzzle_size * puzzle_size;

	char *buffer = malloc(sizeof(char) * num_chars);

	// Input do tabuleiro(a input e a output estão no formato da esquerda para a direita).
    printf("\n\n\nBoard input: ");
	fgets(buffer, num_chars, stdin);

	struct graph *graph = create_graph();
	struct vertex **vertices = malloc(sizeof(void *) * num_boxes);

	// monta o grafo com as vértices
	int i = 0;
	for (i = 0; i < num_boxes; i++){ 
		// adiciona todas as possiveis 9 cores se obtermos 0, a qual é uma célula indefinida
		if (buffer[i] == '0') {
			vertices[i] = add_vertex(graph, 9, 0);
		}
		else {
			// adciona a única possibilidade se obtermos uma célula pré-preenchida 
			vertices[i] = add_vertex(graph, 1, buffer[i] - '0');
		}
	}

	link_three_groups(vertices);
	link_cols_and_rows(vertices);

	free(buffer);
	free(vertices);

	return graph;
}


int main(int argc, char **argv){
	struct graph *graph = load_initial();

	color_graph(graph->vertices, 9);

	// verificação do grafo

	if (graph_colored(graph)){
		// Se o grafo foi colorido, então foi resolvido com sucesso então e imprimido o tabuleiro
	    //printf("\n\nSolved!\n\n");
		clock_t inicio = clock();
		printf("\n\nSolved board:\n\n");
        print_sudoku(graph, 9);
        clock_t fim = clock();
        double tempo = ((double)(fim - inicio)*1000) / CLOCKS_PER_SEC; 
        printf("Time: %2.2f sec\n", tempo);
        getchar();
        printf("\n\n");
    }
	else{
		// Se o grafo não foi colorido, então a solução não teve sucesso
        printf("Unable to color sudoku graph.\n");
    }
	free_graph(graph);
	
	return 0;
}
