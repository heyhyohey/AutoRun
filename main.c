#include <stdio.h>
#include <stdlib.h>

// 자동자 구조체 정의
typedef struct car {
	int dir;
	int x;
	int y;
}Car;

// 명령어 노드 정의
typedef struct node {
	char commend;
	struct node* np;
}Node;

// 방향 상수 정의
#define EAST 1		// 동쪽
#define WEST 2		// 서쪽
#define SOUTH 3		// 남쪽
#define NORTH 4		// 북쪽

// 파일 경로 정의
#define FILENAME "c:\\move.txt"

// 함수 정의
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
	// 1. 명령어를 읽어옴
	Node *commends = getCommend();
	Node *temp = commends;

	// 2. 자동차 구조체 초기화
	Car car = { NORTH, 0, 0 };

	// 3. 명령어에 따라 자동차를 이동
	printf("출발 자동차의 위치는 (%d, %d) 입니다.\n", car.x, car.y);
	moveCar(commends, &car);

	// 4. 최종 위치 출력
	printf("최종 자동차의 위치는 (%d, %d) 입니다.\n", car.x, car.y);

	// 5. 동적할당 해제
	freeCommend(commends);

	return 0;
}

// 파일에서 명령어를 읽어와서 연결리스트를 생성하는 함수
Node* getCommend() {
	char commend;			// 명령어 저장 변수
	FILE* fp = NULL;		// 파일 포인터 변수
	Node* commends = NULL;	// 명령어 연결리스트 포인터 변수

	// 1. 파일 포인터에 파일 열기
	fopen_s(&fp, FILENAME, "r");

	// 1-1. 파일이 존재하지 않을 경우 프로그램 종료
	if (!fp) {
		fprintf(stdout, "File I/O Error\n");
		exit(1);
	}

	// 2. 파일에서 읽어온 값을 리스트에 저장
	while (!feof(fp)) {
		fscanf_s(fp, "%c\n", &commend);
		insertNode(&commends, commend);
	}

	// 3. 파일 포인터 닫기
	fclose(fp);

	// 4. 명령어 연결리스트의 포인터를 반환
	return commends;
}

// 명령어 동적할당 해제
void freeCommend(Node* commends) {
	Node* p = commends->np;

	while (p != NULL) {
		free(commends);
		commends = p;
		p = p->np;
	}
}

// 자동차를 명령어에 따라 이동시키는 함수
void moveCar(Node* commends, Car* car) {
	// 1. 반복문을 위한 포인터 변수 선언
	Node* currentComend = commends;
	// 2. 명령어에 따라 이동
	while (currentComend != NULL) {
		switch (currentComend->commend) {
		// 2-1. 전진
		case 'F':
			moveCarFront(car);
			break;
		// 2-2. 우회전
		case 'R':
			moveCarRight(car);
			break;
		// 2-3. 좌회전
		case 'L':
			moveCarLeft(car);
			break;
		// 2-4. 후진
		case 'B':
			moveCarBack(car);
			break;
		}
		currentComend = currentComend->np;
		printf("현재 자동차의 위치는 (%d, %d) 입니다.\n", car->x, car->y);
	}
}

// 전진 명령어 처리 함수
void moveCarFront(Car* car) {
	// 현재 자동차의 방향에 따라서 이동
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

// 좌회전 명령어 처리함수
void moveCarLeft(Car* car) {
	// 1. 자동차의 방향을 변경
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
	// 2. 방향이 정해진 후 전진
	moveCarFront(car);
}

// 우회전 명령어 처리함수
void moveCarRight(Car* car) {
	// 1. 자동차의 방향을 변경
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
	// 2. 방향이 정해진 후 전진
	moveCarFront(car);
}

// 후진 명령어 처리함수
void moveCarBack(Car* car) {
	// 현재 자동차의 방향에 따라서 이동
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

// 노드 삽입 함수
void insertNode(Node** commends, char commend) {
	Node *currentNode = NULL;	// 현재 노드 포인터 변수
	Node *preNode = NULL;		// 이전 노드 포인터 변수

	// 1. 기존에 생성된 노드가 없을 경우 새로 생성
	if (*commends == NULL)
		*commends = makeNode(commend);

	// 2. 기존에 생성된 노드가 있을 경우 마지막 노드를 찾아서 삽입
	else {

		// 2-1. 현재 노드의 포인터와 이전 노드의 포인터를 초기화
		currentNode = *commends;
		preNode = currentNode;

		// 2-2. 마지막 노드를 검색
		while (currentNode != NULL) {
			preNode = currentNode;
			currentNode = currentNode->np;
		}

		// 2-3. 노드를 삽입
		preNode->np = makeNode(commend);
	}
}

// 노드 생성 함수
Node* makeNode(char commend) {
	// 1. 동적할당하여 노드를 생성
	Node *node = malloc(sizeof(Node));

	// 2. 노드의 데이터와 다음 노드의 포인터를 초기화
	node->commend = commend;
	node->np = NULL;

	return node;
}