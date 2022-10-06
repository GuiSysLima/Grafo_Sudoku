#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

// verifica se um array contém um valor
bool array_contains(int *array, int count, int value){
	int i = 0;
	for (i = 0; i < count; i++)
		if (array[i] == value)
			return true;
	
	return false;
}

// determina se um vértice está restrito a apenas uma única cor dada
bool vertex_must_be(struct vertex *vertex, int color){
	if (vertex->num_possible == 1 && vertex->current_value == color)
		return true;

	return false;
}


// determina se as arestas de um vértice não permitiriam a escolha de uma determinada cor
bool edges_disallow_color(struct vertex *vertex, int color){
	bool in_edge = false;
	struct edge *last_edge = vertex->edges;

	while (last_edge != NULL) {
		if (last_edge->connects_to != NULL) {
			struct vertex *connected = last_edge->connects_to;
			in_edge = vertex_must_be(connected, color);
		}

		if (in_edge == true)
			return true;

		last_edge = last_edge->next;
	}

	return in_edge;
}


// Verifica se a cor é valida para a célula, não contida em nenhum vizinho e não foi removida
int is_valid_color(struct vertex *vertex, int *removed, int removed_count, int color){
	if (vertex->num_possible == 1 && vertex->current_value == color)
		return true;
	else
		if (!array_contains(removed, removed_count, color))
			return !edges_disallow_color(vertex, color);
	
	return false;
}


bool color_graph(struct vertex *vertex, int num_colors){
	// O código começa verificando se o vértice é NULL.
	if (vertex == NULL){
		//Se for, então a função retorna true.
		return true;
	}
	int removed_index = 0;
	int removed_colors[num_colors];
	int original_value = vertex->current_value;
	int original_num = vertex->num_possible;
	// verifica se o número de cores em que esse vértice pode ser colorido foi atingido 
	// e se não há mais cores possíveis para esse vértice.
	if (original_num == 1){
		// Nesse caso, um novo índice de cores é criado e todos os seus valores possíveis são definidos como 1.
		// O valor original do valor_atual também será redefinido para o que era antes (na primeira vez).
		return color_graph(vertex->next, num_colors);
	}		
	int i = 0;
	// Em seguida, um loop começa onde cada iteração verificará se alguma cor válida foi encontrada 
	//  ou não para o índice dessa iteração em particular.
	for (i = 1; i <= original_num; i++) {
		if (!is_valid_color(vertex, removed_colors, removed_index, i))
			continue;
		// Se não tiverem, continue com outra iteração até que ocorra sucesso ou falha
		// momento em que uma dessas duas coisas acontece: sucesso ou falha;
		// Sucesso significa que encontramos uma cor válida para nosso índice atual; 
		// Falha significa que ficamos sem cores e precisamos recomeçar de 0 com um original_num igual a 1 novamente.
		vertex->current_value = i;
		vertex->num_possible = 1;

		bool success = color_graph(vertex->next, num_colors);

		if (success == false) {
			removed_colors[removed_index] = i;
			removed_index++;
			vertex->num_possible = original_num;
			vertex->current_value = original_value;
		}
		else {
			return true;
		}
	}
	return false;
}

//A função tenta encontrar o próximo vértice no gráfico
bool graph_colored(struct graph *graph){
	struct vertex *next_vertex = graph->vertices;
	
	//Começa verificando se existe um próximo vértice e, em seguida, verifica se esse próximo vértice tem mais de um valor possível.
	while (next_vertex != NULL) {
		if(next_vertex->num_possible > 1){
			//Se sim, então o retorno false; instrução será executada.
			return false;
		}
		//Caso contrário, o loop while continuará até que não haja mais vértices no gráfico ou até que todos eles tenham sido verificados por terem vários valores.
		next_vertex = next_vertex->next;
	}
	return true;
}