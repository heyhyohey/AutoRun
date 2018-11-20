#include <stdio.h>
#include <stdlib.h>

// �ڵ��� ����ü ����
typedef struct car {
	int dir;
	int x;
	int y;
}Car;

// ��ɾ� ��� ����
typedef struct node {
	char commend;
	struct node* np;
}Node;

// ���� ��� ����
#define EAST 1		// ����
#define WEST 2		// ����
#define SOUTH 3		// ����
#define NORTH 4		// ����

// ���� ��� ����
#define FILENAME "c:\\move.txt"

// �Լ� ����
Node* makeNode(char);
void insertNode(Node**, char);
Node* getCommend();
void freeCommend(Node* commends);
void moveCar(Node*, Car*);
void moveCarFront(Car*);
void moveCarLeft(Car*);
void moveCarRight(Car*);
void moveCarBack(Car*);

int main(int argc, char* argv[]) {
	// 1. ��ɾ �о��
	Node *commends = getCommend();
	Node *temp = commends;

	// 2. �ڵ��� ����ü �ʱ�ȭ
	Car car = { NORTH, 0, 0 };

	// 3. ��ɾ ���� �ڵ����� �̵�
	printf("��� �ڵ����� ��ġ�� (%d, %d) �Դϴ�.\n", car.x, car.y);
	moveCar(commends, &car);

	// 4. ���� ��ġ ���
	printf("���� �ڵ����� ��ġ�� (%d, %d) �Դϴ�.\n", car.x, car.y);

	// 5. �����Ҵ� ����
	freeCommend(commends);

	return 0;
}

// ���Ͽ��� ��ɾ �о�ͼ� ���Ḯ��Ʈ�� �����ϴ� �Լ�
Node* getCommend() {
	char commend;			// ��ɾ� ���� ����
	FILE* fp = NULL;		// ���� ������ ����
	Node* commends = NULL;	// ��ɾ� ���Ḯ��Ʈ ������ ����

	// 1. ���� �����Ϳ� ���� ����
	fopen_s(&fp, FILENAME, "r");

	// 1-1. ������ �������� ���� ��� ���α׷� ����
	if (!fp) {
		fprintf(stdout, "File I/O Error\n");
		exit(1);
	}

	// 2. ���Ͽ��� �о�� ���� ����Ʈ�� ����
	while (!feof(fp)) {
		fscanf_s(fp, "%c\n", &commend);
		insertNode(&commends, commend);
	}

	// 3. ���� ������ �ݱ�
	fclose(fp);

	// 4. ��ɾ� ���Ḯ��Ʈ�� �����͸� ��ȯ
	return commends;
}

// ��ɾ� �����Ҵ� ����
void freeCommend(Node* commends) {
	Node* p = commends->np;

	while (p != NULL) {
		free(commends);
		commends = p;
		p = p->np;
	}
}

// �ڵ����� ��ɾ ���� �̵���Ű�� �Լ�
void moveCar(Node* commends, Car* car) {
	// 1. �ݺ����� ���� ������ ���� ����
	Node* currentComend = commends;
	// 2. ��ɾ ���� �̵�
	while (currentComend != NULL) {
		switch (currentComend->commend) {
		// 2-1. ����
		case 'F':
			moveCarFront(car);
			break;
		// 2-2. ��ȸ��
		case 'R':
			moveCarRight(car);
			break;
		// 2-3. ��ȸ��
		case 'L':
			moveCarLeft(car);
			break;
		// 2-4. ����
		case 'B':
			moveCarBack(car);
			break;
		}
		currentComend = currentComend->np;
		printf("���� �ڵ����� ��ġ�� (%d, %d) �Դϴ�.\n", car->x, car->y);
	}
}

// ���� ��ɾ� ó�� �Լ�
void moveCarFront(Car* car) {
	// ���� �ڵ����� ���⿡ ���� �̵�
	switch (car->dir) {
	case EAST:
		car->x++;
		break;
	case WEST:
		car->x--;
		break;
	case SOUTH:
		car->y--;
		break;
	case NORTH:
		car->y++;
		break;
	}
}

// ��ȸ�� ��ɾ� ó���Լ�
void moveCarLeft(Car* car) {
	// 1. �ڵ����� ������ ����
	switch (car->dir) {
	case EAST:
		car->dir = NORTH;
		break;
	case WEST:
		car->dir = SOUTH;
		break;
	case SOUTH:
		car->dir = EAST;
		break;
	case NORTH:
		car->dir = WEST;
		break;
	}
	// 2. ������ ������ �� ����
	moveCarFront(car);
}

// ��ȸ�� ��ɾ� ó���Լ�
void moveCarRight(Car* car) {
	// 1. �ڵ����� ������ ����
	switch (car->dir) {
	case EAST:
		car->dir = SOUTH;
		break;
	case WEST:
		car->dir = NORTH;
		break;
	case SOUTH:
		car->dir = WEST;
		break;
	case NORTH:
		car->dir = EAST;
		break;
	}
	// 2. ������ ������ �� ����
	moveCarFront(car);
}

// ���� ��ɾ� ó���Լ�
void moveCarBack(Car* car) {
	// ���� �ڵ����� ���⿡ ���� �̵�
	switch (car->dir) {
	case EAST:
		car->x--;
		break;
	case WEST:
		car->x++;
		break;
	case SOUTH:
		car->y++;
		break;
	case NORTH:
		car->y--;
		break;
	}
}

// ��� ���� �Լ�
void insertNode(Node** commends, char commend) {
	Node *currentNode = NULL;	// ���� ��� ������ ����
	Node *preNode = NULL;		// ���� ��� ������ ����

	// 1. ������ ������ ��尡 ���� ��� ���� ����
	if (*commends == NULL)
		*commends = makeNode(commend);

	// 2. ������ ������ ��尡 ���� ��� ������ ��带 ã�Ƽ� ����
	else {

		// 2-1. ���� ����� �����Ϳ� ���� ����� �����͸� �ʱ�ȭ
		currentNode = *commends;
		preNode = currentNode;

		// 2-2. ������ ��带 �˻�
		while (currentNode != NULL) {
			preNode = currentNode;
			currentNode = currentNode->np;
		}

		// 2-3. ��带 ����
		preNode->np = makeNode(commend);
	}
}

// ��� ���� �Լ�
Node* makeNode(char commend) {
	// 1. �����Ҵ��Ͽ� ��带 ����
	Node *node = malloc(sizeof(Node));

	// 2. ����� �����Ϳ� ���� ����� �����͸� �ʱ�ȭ
	node->commend = commend;
	node->np = NULL;

	return node;
}