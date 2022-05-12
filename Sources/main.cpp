#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "labo.h"

Stack stack_init(size_t max_size) {
	Stack s;
	s.max = max_size;
	s.top = -1;
	s.elements = (void**)allocate(sizeof(void*) * s.max);
	return s;
}

void stack_push(Stack* s, void* element) {
	if (s->max != s->top + 1) {
		s->top++;
		s->elements[s->top] = element;
	}
}

void* stack_pop(Stack* s) {
	if (s->top > -1) {
		void* elem = s->elements[s->top];
		s->elements[s->top] = NULL;
		--s->top;
		return elem;
	}
	return NULL;
}
void queue_init(Queue* q) {
	q->next = q->prev = NULL;
	q->count = 0;
}
void queue_push(Queue* q, void* data) {
	QNode* n = (QNode*)allocate(sizeof(Node));
	n->data = data;
	n->prev = n->next = NULL;
	if (q->prev == NULL) {
		q->prev = q->next = n;
	}
	else {
		QNode* temp = q->next;
		q->next = temp->prev = n;
		n->next = temp;
	}
}
void* queue_pop(Queue* q) {
	if (q->prev != NULL) {
		QNode* temp = q->prev;
		if(temp->prev != NULL)
			temp->prev->next = NULL;
		q->prev = temp->prev;
		q->count++;
		return temp->data;
	}
	else
	{
		return NULL;
	}
}

//#define HEAP_SIZE UINT32_MAX * UINT32_MAX * 9999999999
//static uint8_t* heap = NULL;
//static size_t heap_top = 0;
//void* allocate(size_t size) {
//	size_t old_top = heap_top;
//	heap_top += size;
//	assert(heap_top <= HEAP_SIZE);
//	return &heap[old_top];
//}

int main(int argc, char** argv) {
	
	//trente_et_un_bmp();

	//soixante_quatre_bmp();

	//cent_vingt_huit_bmp();

	cinq_cent_douse_bmp();

	//quarante_bmp();

	//braid2k();

	//combo();

	//perfect2k();

	return 0;
}
