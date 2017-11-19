#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define IS_FULL(ptr) (!(ptr))
#define MAX_POLYNOMIAL_SIZE 100
#define COMPARE(x,y) (((x)<(y)) ? -1:((x)==(y)) ? 0:1)
#define FALSE 0
#define TRUE 1

/* ���׽� ����Ʈ�� ����ü :������ ���(coef)�� ����(expon), ����� ����.*/
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
	// �޴��� �Է��� �� ����� ���� 
	int MenuNumber;
	while (1)
	{
		print_menu();
		scanf_s("%d", &MenuNumber);
		fflush(stdin);
		switch (MenuNumber)
		{
		case 1:			// �Է�
			submenu_read();
			break;
		case 2:			// ���
			submenu_write();
			break;
		case 3:			// ���׽��� ����
			submenu_add();
			break;
		case 4:			// ���׽��� ����
			submenu_substract();
			break;
		case 5:			// ���׽� ��� 
			submenu_eval();
			break;
		case 6:			// ���� 
			return 0;
		default:
			printf("1-6������ ���ڸ� �Է��Ͻʽÿ�..\n");
			break;
		}
	}
}

// ���� �޴��� ����ϴ� �Լ�  
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

// ���� �޴��� ���׽� �Է¿� ���� �Լ� 
void submenu_read()
{
	printf("���׽��� 4x^14 + 5x^10 + 4x^2 + 1 �� ���·� �Է��ϼ���.\n");
	printf("ù��° ���׽�:");
	fflush(stdin);
	firstPolynomial = PolynomialRead();
	fflush(stdin);

	printf("\n�ι�° ���׽�:");
	fflush(stdin);
	secondPolynomial = PolynomialRead();
}

// ���� �޴��� ���׽� ��¿� ���� �Լ� 
void submenu_write()
{
	if (firstPolynomial != NULL && secondPolynomial != NULL)
	{
		printf("ù��° ���׽�");
		PolynomialWrite(firstPolynomial);

		printf("�ι�° ���׽�");
		PolynomialWrite(secondPolynomial);
	}
	else
		printf("���׽��� �Է��� �ּ���...\n");

}

// ���� �޴��� ���׽� ������ ���� �Լ� 
void submenu_add()
{
	if (firstPolynomial != NULL && secondPolynomial != NULL)
	{
		printf("�� ���׽��� �� ���");
		PolynomialWrite(firstPolynomial);
		printf("                +\n");
		PolynomialWrite(secondPolynomial);

		resultPolynomial = cpadd(firstPolynomial, secondPolynomial);

		printf("���\n");
		PolynomialWrite(resultPolynomial);
	}
	else
		printf("���׽��� �Է��� �ּ���...\n");

}

// ���� �޴��� ���׽� ������ ���� �Լ� 
void submenu_substract()
{
	if (firstPolynomial != NULL && secondPolynomial != NULL)
	{
		printf("�� ���׽��� ���� ���\n");
		PolynomialWrite(firstPolynomial);
		printf("                -\n");
		PolynomialWrite(secondPolynomial);

		resultPolynomial = psub(firstPolynomial, secondPolynomial);
		printf("���\n");
		PolynomialWrite(resultPolynomial);
	}
	else
		printf("���׽��� �Է��� �ּ���...\n");

}

// ���� �޴��� ���׽� ��꿡 ���� �Լ� 
void submenu_eval()
{
	double a;

	if (firstPolynomial != NULL && secondPolynomial != NULL)
	{
		printf("������ �Ǽ� k�� ���� ���׽� ���");
		printf("���׽��� x���� ���� �Ǽ��� �Է��ϼ���.\n");
		printf("�Ǽ�: ");
		scanf_s("%lf", &a);

		PolynomialWrite(firstPolynomial);
		printf("���� ���׽Ŀ� ");
		eval(firstPolynomial, a);
		PolynomialWrite(secondPolynomial);
		printf("���� ���׽Ŀ� ");
		eval(secondPolynomial, a);
	}
	else
		printf("���׽��� �Է��� �ּ���...\n");
}

/* ���׽��� �Է� �޾� ���� ���� ����Ʈ�� ��ȯ�ϴ� �Լ� :
���׽��� ��Ʈ������ �Է� �޾� ���� �� ��ŭ ��带 �����ϰ�
�� �׿��� ����� ������ ����� �����Ϳ� �Է��Ѵ�.
*/
polyPointer PolynomialRead()
{
	// �Է� �޴� ���׽Ŀ� ��� �� ���� 
	char c;
	// ��Ʈ�� �迭�� index�� ��Ÿ�� ���� 
	int i = 0;
	int j = 0;
	// ó�� ��Ʈ������ �Է� �޴� ���׽��� ���� ���� 
	char Poly[MAX_POLYNOMIAL_SIZE] = { '\0', };
	// ���׽��� ������ �߶� �� �� ����� ���� 
	char poly[MAX_POLYNOMIAL_SIZE] = { '\0', };
	polyPointer prePolynomial, Head = NULL, Tail = NULL;

	Head = (polyPointer)malloc(sizeof(poly_node));
	if (IS_FULL(Head))
	{
		fprintf(stderr, "The memory is full\n");
		exit(2);
	}

	Head->expon = -1;			// ��� ��� 
	Head->link = Head;
	Tail = Head;

	// ���׽��� �Է� 

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
		// ���׽��� ������ �߶� �� 
		// ���� ������ ��ȣ�� �Ͽ� ��� ���� 
		if (Poly[i] == '-' || Poly[i] == '+' || Poly[i] == '\0')
		{
			if (poly[0] != '\0')
			{
				poly[j] = '\0';
				// �߶� ��(poly)�� ��� ���� 
				prePolynomial = nodeCreate(poly);
				// ������ ��带 ���� ���� ����Ʈ�� ����
				Tail->link = prePolynomial;
				Tail = Tail->link;
				Tail->link = Head;

				// poly�迭�� �ʱ�ȭ 
				for (; j>0; j--)
					poly[j] = '\0';
			}
			// ��� ��ȣ�� ���� 
			poly[j] = Poly[i];
		}
		else
			// ��ȣ �������� ������ poly�� ����(�� ����)
			poly[j] = Poly[i];
		j++;
	} while (Poly[i++] != '\0');

	fflush(stdin);			// ���۸� ����.
	return Head;			// ��带 ��ȯ 		
}

/* ���׽��� ������ �߶� �迭 polyarray�� ������
coef|expon|link ��带 �����ϴ� �Լ� :
��Ʈ������ ǥ���� x ����� ���ڷ� �ٲپ� ��忡 �Է�,
'^' ���ʿ� �ٴ� ������ ���ڷ� �ٲپ� ��忡 �Է�
*/
polyPointer nodeCreate(char* polyarray)
{
	// ���� ����� ������ �߶� �� ����ϴ� �迭  
	char poly[20] = { '\0' };
	// �迭�� index�� ��Ÿ�� ���� 
	int i, j;
	// �׿� x������ �ִ��� Ȯ���ϱ� ���� ���� 
	int check = 0;
	// ��Ʈ���� ���ڷ� �ٲپ� ���� �� ����(���, ����)  
	int transCoef = 0;
	int transExpon = 0;
	// ����� ������ �Է��� ��� ������ 
	polyPointer polynode;

	polynode = (polyPointer)malloc(sizeof(poly_node));

	// �� �迭�� ���� ������ �ݺ� 
	for (i = 0, j = 0; polyarray[i] != '\0'; i++)
	{
		// �׿��� ����� �߶� - x ���� ���������� ���� �Է� 
		if (!(polyarray[i] == 'x' || polyarray[i] == 'X'))
		{
			poly[j] = polyarray[i];
			j++;
		}
		else if (polyarray[i] == 'x' || polyarray[i] == 'X')
		{
			// �׿� x������ �����ϹǷ� 1�� ��ȯ 
			check = 1;
			// poly�� ������ ������ x�� ��� - �Է°� : x
			if (poly[0] == '\0')
				polynode->coef = 1;	// x�� ��� : 1	
			else
			{
				// poly�� ��ȣ�� ���� ��� : +x or -x
				if (polyarray[i - 1] == '+' || polyarray[i - 1] == '-')
				{
					poly[j] = '1';
					j++;
				}
				poly[j] = '\0';
				// ��Ʈ������ ǥ���� poly�� ����� ���ڷ� ��ȯ 
				transCoef = atoi(poly);
				polynode->coef = transCoef;	// ��� �Է� 
			}

			// '^'�� ���� ����: ����
			if (polyarray[i + 1] == '^')
			{
				// ����� ���� ���� poly�� �ʱ�ȭ
				for (; j>0; j--)
					poly[j] = '\0';
				// polyarray�� ���� �������� ������ ���ڰ� �������� �̵�
				i += 2;
				j = 0;
				// ���� �Է�
				while (polyarray[i] != '\0')
				{
					poly[j++] = polyarray[i++];
				}
				poly[j] = '\0';
				transExpon = atoi(poly);
				polynode->expon = transExpon;	// ������ ��忡 �Է� 
			}
			else
			{	// �Է°��� : ax �� ��� (a: 0�� �ƴ� ���)		
				polynode->expon = 1;		// ���� : 1 
			}
			break;

		}
	}
	// ����� �Է� ���� ��� 
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

/* �Է��� ���׽� ����ϴ� �Լ� :
���׽��� �ֻ��� �����͸� �Ű������� �Ͽ�
����� �����͸� ���� ���鼭 ��� ��尡
���� ������ ���׽��� ����� ������ ���
*/
void PolynomialWrite(polyPointer polynomial)
{
	// ���� ����Ʈ���� ��� ��带 ������ ��带 ����ų ������ 
	polyPointer Link;

	printf("���׽� : ");
	Link = polynomial->link;
	// ��尡 ��� ��尡 �� ������ �ݺ� 
	for (; Link->expon != -1; Link = Link->link)
	{
		if (Link->coef >= 0)
			printf("+%dx^%d ", Link->coef, Link->expon);
		else
			printf("%dx^%d ", Link->coef, Link->expon);
	}
	printf("\n");
}

/* �� ���׽��� ���ϴ� �Լ� :
�� ���׽��� �ֻ��� �����͸� �Ű������� �Ͽ�
�� ���׽��� ������ ���ϸ鼭 ������ �Ѵ�.
�Է��� �� ���׽��� ���ϸ� �ȵȴ�.
*/
polyPointer cpadd(polyPointer first, polyPointer second)
{
	// first�� ���� ��ġ�� ����ϱ� ���� ������ 
	polyPointer starta;
	// ������ ����� ����Ʈ�� ���� �ֻ��� ������
	polyPointer d = NULL;
	polyPointer lastd;
	int sum, done = FALSE;
	starta = first;			// first�� ������ ��� 
	// first�� second�� ��� ��带 �ǳʶ� 
	first = first->link;
	second = second->link;

	// �ջ�� ��� ��带 ���� �޸� �Ҵ� 
	d = (polyPointer)malloc(sizeof(poly_node));
	if (IS_FULL(d))
	{
		fprintf(stderr, "The memory is full\n");
		exit(2);
	}

	d->expon = -1;
	lastd = d;
	do{
		// �� ���׽��� ������ �� 
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

/* coef = coefficient�̰� exopn = exponent�� ���ο� ��带 �����ϰ�,
�װ��� ptr�� ���� �����Ǵ� ��忡 ÷���Ѵ�.
ptr�� �����Ͽ� �� ���ο� ��带 �����ϵ��� �Ѵ�.
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

/* �� ���׽��� ������ �ϴ� �Լ� :
�� ���׽��� �ֻ��� �����͸� �Ű������� �Ͽ�
�� ���׽��� ������ ���ϸ鼭 ������ �Ѵ�.
�Է��� �� ���׽��� ���ϸ� �ȵȴ�.
*/
polyPointer psub(polyPointer first, polyPointer second)
{
	polyPointer starta, d, lastd;
	int subresult, done = FALSE;
	starta = first;
	// �� ���׽��� ��� ��带 �ǳʶڴ�.
	first = first->link;
	second = second->link;

	// ������ ��� ��带 ���� �޸� �Ҵ� 
	d = (polyPointer)malloc(sizeof(poly_node));
	if (IS_FULL(d))
	{
		fprintf(stderr, "The memory is full\n");
		exit(2);
	}

	d->expon = -1;
	lastd = d;
	do{
		// �� ���׽��� ������ ���Ѵ�.
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


/* � �Ǽ��� f�� ���ؼ� ���׽��� ����ϸ�,
�� ����� �Ǽ��� ��ȯ�ϴ� �Լ�
*/
void eval(polyPointer polynomial, double f)
{
	// ���� ����� ������ŭ �Ǽ����� �����ֱ� ���� ����ϴ� ���� 
	int expon;
	//�Ǽ����� ���� ��갪�� ��� �� ���� 
	double sum = 0;
	// ���� x�� �Ǽ��� �־� ���� ���� ���� ���� 
	double multsum = 1;
	polyPointer Link;

	// ���׽��� ��� ��带 �ǳʶ� 
	Link = polynomial->link;
	for (; Link->expon != -1; Link = Link->link)
	{
		for (expon = Link->expon; expon >0; expon--)
			multsum = multsum * f;		// ������ŭ f�� ���Ѵ�. 
		if (Link->expon != 0)	// f�� �־� ���� x���� ����� ���� �� ���Ѵ�.
			sum = sum + (Link->coef * multsum);
		else
			sum = sum + Link->coef;
		multsum = 1;
	}
	printf("�Ǽ� %0.5f�� �־ ���� ��� : %0.5f\n", f, sum);
}