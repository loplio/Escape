#include "GameScene.h"
#include "GameManager.h"

static int stack[MAX_STACK_SIZE];
static int top = -1;

bool IsEmpty() {
	if (top < 0)
		return true;
	else
		return false;
}
bool IsFull() {
	if (top >= MAX_STACK_SIZE - 1)
		return true;
	else
		return false;
}
void push(int value) {
	if (IsFull())
		printf("Stack Full.\n");
	else
		stack[++top] = value;
}
int pop() {
	if (IsEmpty())
		printf("Stack Empty.\n");
	else
		return stack[top--];
}

void GM()
{

}