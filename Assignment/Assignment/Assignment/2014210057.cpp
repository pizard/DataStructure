#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define IS_FULL(ptr) (!(ptr))
#define MAX_POLYNOMIAL_SIZE 100
#define COMPARE(x,y) (((x)<(y)) ? -1:((x)==(y)) ? 0:1)
#define FALSE 0
#define TRUE 1

/* 다항식 리스트의 구조체 :정수형 계수(coef)와 지수(expon), 노드의 구조.*/
typedef struct polyNode *polyPointer;
typedef struct polyNode {
	int coef;
	int expon;
	polyPointer link;
} poly_node;

polyPointer firstPolynomial, secondPolynomial, resultPolynomial;
polyPointer PolynomialRead();
polyPointer nodeCreate(char* polyarray);
void PolynomialWrite(polyPointer polynomial);
polyPointer cpadd(polyPointer first, polyPointer second);
void attach(int coefficient, int exponent, polyPointer *ptr);
polyPointer psub(polyPointer first, polyPointer second);
polyPointer pmult(polyPointer first, polyPointer second);
void eval(polyPointer polynomial, double f);

void print_menu();
void submenu_read();
void submenu_write();
void submenu_add();
void submenu_substract();
void submenu_eval();
void submenu_read();

int main()
{
	// 메뉴를 입력할 때 사용할 변수 
	int MenuNumber;
	while (1)
	{
		print_menu();
		scanf_s("%d", &MenuNumber);
		fflush(stdin);
		switch (MenuNumber)
		{
		case 1:			// 입력
			submenu_read();
			break;
		case 2:			// 출력
			submenu_write();
			break;
		case 3:			// 다항식의 덧셈
			submenu_add();
			break;
		case 4:			// 다항식의 뺄셈
			submenu_substract();
			break;
		case 5:			// 다항식 계산 
			submenu_eval();
			break;
		case 6:			// 종료 
			return 0;
		default:
			printf("1-6까지의 숫자를 입력하십시오..\n");
			break;
		}
	}
}

// 메인 메뉴를 출력하는 함수  
void print_menu()
{
	printf("\n\n");
	printf("1. Polynomial Read\n");
	printf("2. Polynomial Write\n");
	printf("3. Polynomial Add\n");
	printf("4. Polynomial Subtraction\n");
	printf("5. Polynomial Evaluation\n");
	printf("6. Exit\n");
	printf("----------------------------\n");
	printf("You select a menu number\n");
	printf("Menu: ");
}

// 메인 메뉴의 다항식 입력에 대한 함수 
void submenu_read()
{
	printf("다항식을 4x^14 + 5x^10 + 4x^2 + 1 의 형태로 입력하세요.\n");
	printf("첫번째 다항식:");
	fflush(stdin);
	firstPolynomial = PolynomialRead();
	fflush(stdin);

	printf("\n두번째 다항식:");
	fflush(stdin);
	secondPolynomial = PolynomialRead();
}

// 메인 메뉴의 다항식 출력에 대한 함수 
void submenu_write()
{
	if (firstPolynomial != NULL && secondPolynomial != NULL)
	{
		printf("첫번째 다항식");
		PolynomialWrite(firstPolynomial);

		printf("두번째 다항식");
		PolynomialWrite(secondPolynomial);
	}
	else
		printf("다항식을 입력해 주세요...\n");

}

// 메인 메뉴의 다항식 덧셈에 대한 함수 
void submenu_add()
{
	if (firstPolynomial != NULL && secondPolynomial != NULL)
	{
		printf("두 다항식의 합 계산");
		PolynomialWrite(firstPolynomial);
		printf("                +\n");
		PolynomialWrite(secondPolynomial);

		resultPolynomial = cpadd(firstPolynomial, secondPolynomial);

		printf("결과\n");
		PolynomialWrite(resultPolynomial);
	}
	else
		printf("다항식을 입력해 주세요...\n");

}

// 메인 메뉴의 다항식 뺄셈에 대한 함수 
void submenu_substract()
{
	if (firstPolynomial != NULL && secondPolynomial != NULL)
	{
		printf("두 다항식의 뺄셈 계산\n");
		PolynomialWrite(firstPolynomial);
		printf("                -\n");
		PolynomialWrite(secondPolynomial);

		resultPolynomial = psub(firstPolynomial, secondPolynomial);
		printf("결과\n");
		PolynomialWrite(resultPolynomial);
	}
	else
		printf("다항식을 입력해 주세요...\n");

}

// 메인 메뉴의 다항식 계산에 대한 함수 
void submenu_eval()
{
	double a;

	if (firstPolynomial != NULL && secondPolynomial != NULL)
	{
		printf("임의의 실수 k에 대한 다항식 계산");
		printf("다항식의 x값에 넣을 실수를 입력하세요.\n");
		printf("실수: ");
		scanf_s("%lf", &a);

		PolynomialWrite(firstPolynomial);
		printf("위의 다항식에 ");
		eval(firstPolynomial, a);
		PolynomialWrite(secondPolynomial);
		printf("위의 다항식에 ");
		eval(secondPolynomial, a);
	}
	else
		printf("다항식을 입력해 주세요...\n");
}

/* 다항식을 입력 받아 원형 연결 리스트로 변환하는 함수 :
다항식을 스트링으로 입력 받아 항의 수 만큼 노드를 연결하고
각 항에서 계수와 지수를 노드의 데이터에 입력한다.
*/
polyPointer PolynomialRead()
{
	// 입력 받는 다항식에 사용 될 변수 
	char c;
	// 스트링 배열의 index를 나타낼 변수 
	int i = 0;
	int j = 0;
	// 처음 스트링으로 입력 받는 다항식을 위한 변수 
	char Poly[MAX_POLYNOMIAL_SIZE] = { '\0', };
	// 다항식을 항으로 잘라 낼 때 사용할 변수 
	char poly[MAX_POLYNOMIAL_SIZE] = { '\0', };
	polyPointer prePolynomial, Head = NULL, Tail = NULL;

	Head = (polyPointer)malloc(sizeof(poly_node));
	if (IS_FULL(Head))
	{
		fprintf(stderr, "The memory is full\n");
		exit(2);
	}

	Head->expon = -1;			// 헤드 노드 
	Head->link = Head;
	Tail = Head;

	// 다항식을 입력 

	printf("Polynomial: ");
	fflush(stdin);
	while ((c = getchar()) != '\n')
	{
		if (c == EOF)
		{
			printf("Program terminated...\n");
			fflush(stdin);
			break;
		}
		if (c != ' ')
			Poly[i++] = c;
	}
	strcat_s(Poly, "\0");
	printf("\n");
	fflush(stdin);

	poly[0] = '\0';

	i = 0;
	do {
		// 다항식을 항으로 잘라 냄 
		// 항의 구분을 부호로 하여 노드 생성 
		if (Poly[i] == '-' || Poly[i] == '+' || Poly[i] == '\0')
		{
			if (poly[0] != '\0')
			{
				poly[j] = '\0';
				// 잘라낸 항(poly)의 노드 생성 
				prePolynomial = nodeCreate(poly);
				// 생성된 노드를 원형 연결 리스트에 연결
				Tail->link = prePolynomial;
				Tail = Tail->link;
				Tail->link = Head;

				// poly배열을 초기화 
				for (; j>0; j--)
					poly[j] = '\0';
			}
			// 계수 부호를 결정 
			poly[j] = Poly[i];
		}
		else
			// 부호 이전까지 정보를 poly에 삽입(항 삽입)
			poly[j] = Poly[i];
		j++;
	} while (Poly[i++] != '\0');

	fflush(stdin);			// 버퍼를 비운다.
	return Head;			// 헤드를 반환 		
}

/* 다항식을 항으로 잘라낸 배열 polyarray의 정보로
coef|expon|link 노드를 생성하는 함수 :
스트링으로 표현된 x 계수를 숫자로 바꾸어 노드에 입력,
'^' 뒤쪽에 붙는 지수도 숫자로 바꾸어 노드에 입력
*/
polyPointer nodeCreate(char* polyarray)
{
	// 항의 계수와 지수를 잘라낼 때 사용하는 배열  
	char poly[20] = { '\0' };
	// 배열의 index를 나타낼 변수 
	int i, j;
	// 항에 x변수가 있는지 확인하기 위한 변수 
	int check = 0;
	// 스트링을 숫자로 바꾸어 저장 될 변수(계수, 지수)  
	int transCoef = 0;
	int transExpon = 0;
	// 계수와 지수를 입력할 노드 포인터 
	polyPointer polynode;

	polynode = (polyPointer)malloc(sizeof(poly_node));

	// 항 배열이 끝날 때까지 반복 
	for (i = 0, j = 0; polyarray[i] != '\0'; i++)
	{
		// 항에서 계수만 잘라냄 - x 변수 이전까지의 정보 입력 
		if (!(polyarray[i] == 'x' || polyarray[i] == 'X'))
		{
			poly[j] = polyarray[i];
			j++;
		}
		else if (polyarray[i] == 'x' || polyarray[i] == 'X')
		{
			// 항에 x변수가 존재하므로 1로 변환 
			check = 1;
			// poly에 정보가 없지만 x인 경우 - 입력값 : x
			if (poly[0] == '\0')
				polynode->coef = 1;	// x의 계수 : 1	
			else
			{
				// poly에 부호만 있을 경우 : +x or -x
				if (polyarray[i - 1] == '+' || polyarray[i - 1] == '-')
				{
					poly[j] = '1';
					j++;
				}
				poly[j] = '\0';
				// 스트링으로 표현된 poly의 계수를 숫자로 변환 
				transCoef = atoi(poly);
				polynode->coef = transCoef;	// 계수 입력 
			}

			// '^'의 다음 숫자: 지수
			if (polyarray[i + 1] == '^')
			{
				// 계수를 위해 사용된 poly를 초기화
				for (; j>0; j--)
					poly[j] = '\0';
				// polyarray의 현재 지점에서 지수의 숫자가 나오도록 이동
				i += 2;
				j = 0;
				// 지수 입력
				while (polyarray[i] != '\0')
				{
					poly[j++] = polyarray[i++];
				}
				poly[j] = '\0';
				transExpon = atoi(poly);
				polynode->expon = transExpon;	// 지수를 노드에 입력 
			}
			else
			{	// 입력값이 : ax 인 경우 (a: 0이 아닌 상수)		
				polynode->expon = 1;		// 지수 : 1 
			}
			break;

		}
	}
	// 상수만 입력 했을 경우 
	if (poly != NULL && check == 0)
	{
		poly[j] = '\0';
		transCoef = atoi(poly);
		polynode->coef = transCoef;
		polynode->expon = 0;
	}
	polynode->link = NULL;
	return polynode;
}

/* 입력한 다항식 출력하는 함수 :
다항식의 최상위 포인터를 매개변수로 하여
노드의 포인터를 따라 가면서 헤드 노드가
나올 때까지 다항식의 계수와 지수를 출력
*/
void PolynomialWrite(polyPointer polynomial)
{
	// 연결 리스트에서 헤드 노드를 제외한 노드를 가리킬 포인터 
	polyPointer Link;

	printf("다항식 : ");
	Link = polynomial->link;
	// 노드가 헤드 노드가 될 때까지 반복 
	for (; Link->expon != -1; Link = Link->link)
	{
		if (Link->coef >= 0)
			printf("+%dx^%d ", Link->coef, Link->expon);
		else
			printf("%dx^%d ", Link->coef, Link->expon);
	}
	printf("\n");
}

/* 두 다항식을 더하는 함수 :
두 다항식의 최상위 포인터를 매개변수로 하여
두 다항식의 지수를 비교하면서 덧셈을 한다.
입력한 두 다항식은 변하면 안된다.
*/
polyPointer cpadd(polyPointer first, polyPointer second)
{
	// first의 시작 위치를 기록하기 위한 포인터 
	polyPointer starta;
	// 덧셈한 결과값 리스트를 위한 최상위 포인터
	polyPointer d = NULL;
	polyPointer lastd;
	int sum, done = FALSE;
	starta = first;			// first의 시작을 기록 
	// first와 second의 헤드 노드를 건너뜀 
	first = first->link;
	second = second->link;

	// 합산용 헤드 노드를 위한 메모리 할당 
	d = (polyPointer)malloc(sizeof(poly_node));
	if (IS_FULL(d))
	{
		fprintf(stderr, "The memory is full\n");
		exit(2);
	}

	d->expon = -1;
	lastd = d;
	do{
		// 두 다항식의 지수를 비교 
		switch (COMPARE(first->expon, second->expon))
		{
		case -1:		// first->expon < second->expon		
			attach(second->coef, second->expon, &lastd);
			second = second->link;
			break;
		case 0:		// first->expon = second->expon
			if (starta == first)
			{
				done = TRUE;
			}
			else
			{
				sum = first->coef + second->coef;
				if (sum)
					attach(sum, first->expon, &lastd);
				first = first->link;
				second = second->link;
			}
			break;
		case 1:		// first->expon < second->expon
			attach(first->coef, first->expon, &lastd);
			first = first->link;
		}
	} while (!done);
	lastd->link = d;
	return d;
}

/* coef = coefficient이고 exopn = exponent인 새로운 노드를 생성하고,
그것을 ptr에 의해 참조되는 노드에 첨가한다.
ptr을 갱신하여 이 새로운 노드를 참조하도록 한다.
*/
void attach(int coefficient, int exponent, polyPointer *ptr)
{
	polyPointer temp;
	temp = (polyPointer)malloc(sizeof(poly_node));
	if (IS_FULL(temp)){
		fprintf(stderr, "The memory is full\n");
		exit(3);
	}
	temp->coef = coefficient;
	temp->expon = exponent;
	(*ptr)->link = temp;
	*ptr = temp;
}

/* 두 다항식의 뺄셈을 하는 함수 :
두 다항식의 최상위 포인터를 매개변수로 하여
두 다항식의 지수를 비교하면서 뺄셈을 한다.
입력한 두 다항식은 변하면 안된다.
*/
polyPointer psub(polyPointer first, polyPointer second)
{
	polyPointer starta, d, lastd;
	int subresult, done = FALSE;
	starta = first;
	// 두 다항식의 헤드 노드를 건너뛴다.
	first = first->link;
	second = second->link;

	// 뺄셈용 헤드 노드를 위한 메모리 할당 
	d = (polyPointer)malloc(sizeof(poly_node));
	if (IS_FULL(d))
	{
		fprintf(stderr, "The memory is full\n");
		exit(2);
	}

	d->expon = -1;
	lastd = d;
	do{
		// 두 다항식의 지수를 비교한다.
		switch (COMPARE(first->expon, second->expon))
		{
		case -1:		// first->expon < second->expon
			second->coef = (-1)*second->coef;
			attach(second->coef, second->expon, &lastd);
			second = second->link;
			break;
		case 0:		// first->expon = second->expon
			if (starta == first)
				done = TRUE;
			else
			{
				subresult = first->coef - second->coef;
				if (subresult)
					attach(subresult, first->expon, &lastd);
				first = first->link;
				second = second->link;
			}
			break;
		case 1:		// first->expon < second->expon
			attach(first->coef, first->expon, &lastd);
			first = first->link;
		}
	} while (!done);
	lastd->link = d;
	return d;
}


/* 어떤 실수값 f에 대해서 다항식을 계산하며,
그 결과를 실수로 반환하는 함수
*/
void eval(polyPointer polynomial, double f)
{
	// 현재 노드의 지수만큼 실수값을 곱해주기 위해 사용하는 변수 
	int expon;
	//실수값에 대한 계산값이 들어 갈 변수 
	double sum = 0;
	// 변수 x에 실수를 넣어 계산된 값을 위한 변수 
	double multsum = 1;
	polyPointer Link;

	// 다항식의 헤드 노드를 건너뜀 
	Link = polynomial->link;
	for (; Link->expon != -1; Link = Link->link)
	{
		for (expon = Link->expon; expon >0; expon--)
			multsum = multsum * f;		// 지수만큼 f를 곱한다. 
		if (Link->expon != 0)	// f를 넣어 나온 x값과 계수를 곱한 후 더한다.
			sum = sum + (Link->coef * multsum);
		else
			sum = sum + Link->coef;
		multsum = 1;
	}
	printf("실수 %0.5f를 넣어서 계산된 결과 : %0.5f\n", f, sum);
}