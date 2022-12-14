#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 10

#define NO_ERRORS 0
#define INPUT_ERROR 1
#define INCORRECT_DATA 2 
// Вводится массив, удалить из него квадраты чисел
int form_arr(int *, int);
int delete_quadro(int *, int);
int print_arr(int *a, int m);

int main(void)
{
	int error_code = INPUT_ERROR, rc, cnt;
	int n;
	setbuf(stdout, NULL);
	printf("Введите количество: ");
	rc = scanf("%d", &n);
	printf("\n");
	int arr_input[N];
	if (rc == 1)
	{
		if (n < 1 || n > N)
			error_code = INCORRECT_DATA;
		else
		{
			error_code = form_arr(arr_input, n);
			if (error_code == NO_ERRORS)
			{
				cnt = delete_quadro(arr_input, n);
				if (cnt != n)
					print_arr(&arr_input[0], n - cnt);
				else
					error_code = INCORRECT_DATA;
			}
		}
	}
	else
		error_code = INPUT_ERROR;
	if (error_code == INPUT_ERROR)
		printf("INPUT_ERROR");
	else if (error_code == INCORRECT_DATA)
		printf("INCORRECT_DATA");
	return error_code;
}

int print_arr(int *a, int m)
{
	printf("\n");
	for (int j = 0; j < m; j++)
	{	
		printf("%d ", *a);
		a++;
	}
	printf("\n");
	return 0;
}

int form_arr(int *a, int n)
{
	int i = 0, rc = 1, error_code = NO_ERRORS;
	int tmp = 0; 
	while (i < n && rc != EOF && rc != 0)
	{
		printf("a[%d] = ", i);
		rc = scanf("%d", &tmp);
		if (rc == EOF || rc == 0)
			error_code = INPUT_ERROR;
		else
		{
			a[i] = tmp;
			printf("\n");
			i++;
		}
	}
	return error_code;
}

int delete_quadro(int *a, int n)
{	
	int cnt = 0;
	int k, i = 0;
	while (i < n)
	{
		k = 1;
		while (k * k < abs(a[i - cnt]))
			k++;
		if (abs(k * k) == abs(a[i - cnt])) 
		{
			for (int j = i - cnt; j < n - cnt; j++)
			{
				a[j] = a[j + 1];
			}
			cnt++;
		}
		i++;
	}
	return cnt;
}
