#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double **alocarMatriz(int n);
double *alocarB(int n);
void lerMatriz(int n, double **p, double *B);
void imprimeMatriz(int n, double **p);
void imprimeMatrizB(int n, double **p, double *B);
void pivotearB(int n, double **p, double *B, int b, int a);
void zerarTriangInf(int n, double **p, double **l, double *B);
void geraD(int n, double **p);
void geraDr(int n, double **p);
double  **matrizTransposta(int n, double **p);
double **multiplicaMatriz(int n, double **d, double **l);
double *calculaResultadoL(int n, double **p, double *B);
double *calculaResultadoU(int n, double **p, double *B);
void imprimeResultado(int n, double *r);
void desalocaM(int n, double **p);
void desalocaResultado(double *r);

//MATRIZ 3x3 DE TESTES
//  4  2 -4 | 0 
//  2 10  4 | 6
// -4  4  9 | 5
// M:4 2 -4 2 10 4 -4 4 9
// B:0 6 5
// INPUT: 3 4 2 -4 2 10 4 -4 4 9 0 6 5
//RESULTADO ESPERADO
// 1 0 1

int main(){
	double **p;//Resultara em U e futuramente D e D~
	double **l;//Resultara em L
	double **G, **GT;//Matriz G e G Tranposta
	double *B;

	double *y;
	double *resultado;
	int n;

	printf("Dimensão da matriz: ");
	scanf("%d", &n);

	p = alocarMatriz(n);
	l = alocarMatriz(n);
	B = alocarB(n);

	lerMatriz(n,p,B);

	zerarTriangInf(n,p,l,B);

	printf("MATRIZ U:\n");
	imprimeMatriz(n,p);
	printf("MATRIZ L:\n");
	imprimeMatriz(n,l);

	geraD(n,p);

	geraDr(n,p);

	G = multiplicaMatriz(n,p,l);

	GT = matrizTransposta(n,G);

	y = calculaResultadoL(n,G,B);
	resultado = calculaResultadoU(n,GT,y);

	imprimeResultado(n,resultado);

	desalocaM(n,p);
	desalocaM(n,l);
	desalocaM(n,G);
	desalocaM(n,GT);
	desalocaResultado(B);
	desalocaResultado(y);
	desalocaResultado(resultado);
	return 0;	
}

double **alocarMatriz(int n){
	int i,j; 

	double **m = (double**) malloc(n * sizeof(double*)); // Aloca um vetor de ponteiros

	for (i = 0; i < n; i++)
		m[i] = (double*) malloc(n * sizeof(double));  // Aloca um vetor de valores double
	
	return m;
}

double *alocarB(int n){
	double *m = (double *) malloc(n * sizeof(double));
	return m;
}

void lerMatriz(int n, double **p, double *B){
	int i,j;

	printf("Valores da matriz M:\n");
	for(i = 0; i < n; i++){
		for(j = 0; j < (n); j++){
			scanf("%lf", &p[i][j]);
		}
	}

	printf("Valores da matriz B:\n");
	for(i = 0; i < n; i++){
		scanf("%lf", &B[i]);
	}

}

void zerarTriangInf(int n, double **p, double **l, double *B){
	int a,b,c;
	double x;

	for(a = 0; a < n; a++){//Zera a matriz L e define sua diagonal principal como 1
		for(b = 0; b < n; b++){
			if(a==b){
				l[a][b] = 1;
			}
			else
				l[a][b] = 0;
		}
	}

	for(a = 0; a < (n-1); a++){// Número de elementos de vezes a aplicar o algoritmo
	
		for(b = (a+1); b < n; b++){

			pivotearB(n,p,B,a,a);

			if(p[a][a] == 0){
				printf("Divisão por 0 inesperada\n");
				break;
			}

			x = p[b][a] / p[a][a]; // Calcula o valor a ser aplicado as linhas

			l[b][a] = x;

			for(c = a; c < n; c++){// Aplica o valor calculado as linhas
				p[b][c] = p[b][c] - p[a][c]*x;
			}

		}
	}
}

void pivotearB(int n, double **p, double *B, int b, int a){
	double maior;
	int i; //Contador para o for
	int j; //Posição do maior elemento
	double *q, aux;
	
	maior = fabs(p[b][a]);
	j = b;

	for(i = b; i < n; i++){
		if(fabs(p[i][a]) > maior){
			maior = fabs(p[i][a]);
			j = i;
		}
	}

	//Troca as linhas para evitar divisão por 0
	q = p[b];
	p[b] = p[j];
	p[j] = q; 
	
	aux = B[b];
	B[b] = B[j];
	B[j] = aux;
}

void geraD(int n, double **p){
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			if(i!=j)
				p[i][j]=0;
		}
	}
}

void geraDr(int n, double **p){
	int i;
	for(i = 0; i < n; i++){
		if(p[i][i] < 0){
			printf("VALORES IMAGINARIOS NÃO SÃO ACEITOS\n");
			break;
		}
		p[i][i] = sqrt(p[i][i]);
	}
}

double **multiplicaMatriz(int n, double **d, double **l){
	int i, j;
	double sum;
	double **m = alocarMatriz(n);

	for(i = 0; i < n; i++){//Matriz M recebe matriz l
		for(j = 0; j < n; j++){
			m[i][j] = l[i][j];
		}
	}

	for(i = 0; i < n; i++){//Multiplica cada coluna da matriz M pelos valores da matriz D
		for(j = 0; j < n; j++){
			m[j][i] *= d[i][i];
		}
	}

	return m;
}

double  **matrizTransposta(int n, double **p){
	int i, j;
	
	double **m = alocarMatriz(n);

	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			m[j][i] = p[i][j];
		}
	}

	return m;
}

double *calculaResultadoU(int n, double **p, double *B){
	double *result = malloc(n * sizeof(double));
	double sum;
	int a, b, count=(n-1);

	for(a = (n-1); a >= 0; a--){
		sum = 0;

		pivotearB(n,p,B,a,a);
		
		for(b=count; b < n; b++){
			sum += (p[a][b]*result[b]);
		}

		result[a] = (B[a]-sum)/p[a][a];
		count--;
	}

	return result;
}

double *calculaResultadoL(int n, double **l, double *B){
	double *result = malloc(n * sizeof(double));
	double sum;
	int a, b, count=(n-1);

	for(a = 0; a < n; a++){
		sum = 0;
		
		for(b=0; b < a; b++){
			sum += (l[a][b]*result[b]);
		}

		result[a] = (B[a]-sum)/l[a][a];
		count--;
	}

	return result;
}

void imprimeMatrizB(int n, double **p, double *B){
	int i,j;

	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			printf("%+3.3lf\t", p[i][j]);
		}
		printf("|\t%+3.3lf", B[i]);
		putchar('\n');
	}

	putchar('\n');
}

void imprimeMatriz(int n, double **p){
	int i,j;

	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			printf("%+3.3lf\t", p[i][j]);
		}
		putchar('\n');
	}

	putchar('\n');
}


void imprimeResultado(int n, double *r){
	int i;
	for(i = 0; i < n; i++){
		printf("Resultado de x%2d: %+3.3lf\n", i+1, r[i]);
	}
}

void desalocaM(int n, double **p){
	int i;

	for(i = 0; i < n; i++){
		free(p[i]);//Desaloca vetor de números
	}

	free(p);//Desaloca vetor de ponteiros
}

void desalocaResultado(double *r){
	free(r);
}
