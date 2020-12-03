#include <iostream> //IO by mcs
#include <stdlib.h> //gestion de memoria dinamica
#include <time.h>// para la funcion  clock() aun que la el mismo editor tiene un contador es necesario incluir para comprar
#include <omp.h>
//definiendo algunos valores
#define		NARRAY 100000
#define		FALSE 0
#define		TRUE  1
using namespace std;

typedef int ty_vec[NARRAY];
void Menu();
void CargarVectorDesordenado(ty_vec &v);
void Burbujeo(ty_vec v2);
void BurbujeoParalelo(ty_vec v2);
void BurbujeoBidireccional(ty_vec v2);
void BurbujeoBidireccionalParalelo(ty_vec v2);
void Shell(ty_vec v2);
void ShellParalelo(ty_vec v2);
void InsertSort(ty_vec v2);
void InsertionSort(ty_vec v2);
void InsertionSortParalelo(ty_vec v2);
void LimpiarCin();


//funacion para cargar vector aleatorio, para poder ordenar,
void CargarVectorDesordenado(ty_vec&v)
{
	//cargamos el vector aleatorio y almacenamos en v
	int f;//declaramos para inicializar el for en 0
	int n;//declaramos la variable de asignacion donde se alamacenara los valores generados aleatoriamente
	for(f=0; f<NARRAY; f++)
	{
		n = rand() %NARRAY+1;
		v[f]=n;
	}
}

//funacion de ordenamiento normal o lineal
void Burbujeo(ty_vec v2)
{
int i;
int iAux;
int iPaso=1;
bool bandera; // bandera = 0 inicia el indicador de vector ordenado
clock_t t;
t = clock();
	cout<<"Realizando ordenamiento utilizando burbujeo LINEAL...\n";
	do
	{
		bandera=TRUE;
		for(i=0;i<NARRAY-iPaso;i++)
		{
			if(v2[i]>v2[i+1])
			{
				iAux	= v2[i];
				v2[i]	= v2[i+1];
				v2[i+1]	= iAux;
				bandera=FALSE; // indicador de vector ordenado
			}
		}
		iPaso++;
	}while(!bandera);
	t = clock() - t;
	cout<<"Ordenamiento realizado.\n";
	cout<<"Tardo "<<t<<" ticks del reloj. ( "<< ((float)t)/CLOCKS_PER_SEC<<" segundos)\n";

}

//funacion de ordenamiento paralelo
void BurbujeoParalelo(ty_vec v2)
{
int i;
int fase;
int tmp;
int iPos;
clock_t t;
t = clock();

	cout<<"Realizando ordenamiento utilizando burbujeo PARALEO...\n";
	#pragma omp parallel default(none) shared(v2) private(i, tmp, fase) //Usando default fuerzo a decidir el scope de TODAS las variables
	for (fase=0; fase<NARRAY;fase++)
	{
		if(fase % 2 == 0)
       #pragma omp parallel for
			for (i=1;i<NARRAY;i+=2)
			{
				if(v2[i-1]>v2[i])
				{
					tmp = v2[i-1];
					v2[i-1] = v2[i];
					v2[i] = tmp;
				}
			}
		else
        #pragma omp for
			for(i=1;i<NARRAY-1;i+=2)
			{
				if(v2[i]>v2[i+1])
				{
					tmp=v2[i+1];
					v2[i+1] = v2[i];
					v2[i]= tmp;
				}
			}
	}

	t = clock() - t;
	cout<<"Ordenamiento realizado.\n";
	cout<<"Tardo "<<t<<" ticks del reloj. ( "<< ((float)t)/CLOCKS_PER_SEC<<" segundos)\n";
	cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
	while(!(cin>>iPos))
	{
		LimpiarCin();
		cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
	}
	while(iPos!=-1)
	{
		cout<<"En la posicion "<<iPos<<" se encuentra el nro "<<v2[iPos]<<endl;
		cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
		while(!(cin>>iPos))
		{
			LimpiarCin();
			cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
		}
	}
}

void BurbujeoBidireccional(ty_vec v2)
{
register int a;
int exchange;
int t;
clock_t ti;
ti = clock();

	cout<<"Realizando ordenamiento utilizando burbujeo bidireccional LINEAL...\n";
	do
	{
		exchange = 0;
		for(a=NARRAY-1; a>0; --a)
		{
			if(v2[a-1] > v2[a])
			{
				t = v2[a-1];
				v2[a-1] = v2[a];
				v2[a] = t;
				exchange = 1;
			}
		}
		for(a=1; a < NARRAY; ++a)
		{
			if(v2[a-1] > v2[a])
			{
				t = v2[a-1];
				v2[a-1] = v2[a];
				v2[a] = t;
				exchange = 1;
			}
		}
	}while(exchange);
	ti = clock() - ti;
	cout<<"Ordenamiento realizado.\n";
	cout<<"Tardo "<<ti<<" ticks del reloj. ( "<< ((float)ti)/CLOCKS_PER_SEC<<" segundos)\n";
}

void BurbujeoBidireccionalParalelo(ty_vec v2)
{
register int a;
int exchange;
int t=0;
long iPos;
clock_t ti;
ti = clock();

	cout<<"Realizando ordenamiento utilizando burbujeo bidireccional PARALELO...\n";
	#pragma omp parallel default(none) shared(v2) private(a, t, exchange)
	do
	{
		exchange = 0;
		//#pragma omp for //SE CUELGA CON 8 NUCLEOS, PERO CON 2 FUNCIONA
		for(a=NARRAY-1; a>0; --a)
		{
			if(v2[a-1] > v2[a])
			{
				t = v2[a-1];
				v2[a-1] = v2[a];
				v2[a] = t;
				exchange = 1;
			}
		}
		//#pragma omp for //SE CUELGA CON 8 NUCLEOS, PERO CON 2 FUNCIONA
		for(a=1; a < NARRAY; ++a)
		{
			if(v2[a-1] > v2[a])
			{
				t = v2[a-1];
				v2[a-1] = v2[a];
				v2[a] = t;
				exchange = 1;
			}
		}
	}while(exchange);
	ti = clock() - ti;
	cout<<"Ordenamiento realizado.\n";
	cout<<"Tardo "<<ti<<" ticks del reloj. ( "<< ((float)ti)/CLOCKS_PER_SEC<<" segundos)\n";
	cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
	while(!(cin>>iPos))
	{
		LimpiarCin();
		cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
	}
	while(iPos!=-1)
	{
		cout<<"En la posicion "<<iPos<<" se encuentra el nro "<<v2[iPos]<<endl;
		cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
		while(!(cin>>iPos))
		{
			LimpiarCin();
			cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
		}
	}
}

void Shell(ty_vec v2)
{
int i, j, incrmnt=0, temp;
clock_t t;
t = clock();

	cout<<"Realizando ordenamiento utilizando Shell LINEAL...";
	incrmnt = NARRAY/2;
	while (incrmnt > 0)
	{
		for (i=incrmnt; i < NARRAY; i++)
		{
			j = i;
			temp = v2[i];
			while ((j >= incrmnt) && (v2[j-incrmnt] > temp))
			{
				v2[j] = v2[j - incrmnt];
				j = j - incrmnt;
			}
			v2[j] = temp;
		}
		incrmnt /= 2;
	}
	t = clock() - t;
	cout<<"Ordenamiento realizado.\n";
	cout<<"Tardo "<<t<<" ticks del reloj. ( "<< ((float)t)/CLOCKS_PER_SEC<<" segundos)\n";

}

void ShellParalelo(ty_vec v2)
{
int i=0, j=0, incrmnt=0, temp=0;
long iPos;
clock_t t;
t = clock();

	cout<<"Realizando ordenamiento utilizando Shell PARALEO...";
	incrmnt = NARRAY/2;
	#pragma omp parallel default(none) firstprivate(incrmnt) shared(v2) private(i, j, temp)//Con firstprivate mantengo el valor inicializado anteriormente de incrmnt
	{
		while (incrmnt > 0)
		{
			#pragma omp for
			for (i=incrmnt; i < NARRAY; i++)
			{
				j = i;
				temp = v2[i];
				while ((j >= incrmnt) && (v2[j-incrmnt] > temp))
				{
					v2[j] = v2[j - incrmnt];
					j = j - incrmnt;
				}
				v2[j] = temp;
			}
			incrmnt /= 2;
		}
	}
	t = clock() - t;
	cout<<"Ordenamiento realizado.\n";
	cout<<"Tardo "<<t<<" ticks del reloj. ( "<< ((float)t)/CLOCKS_PER_SEC<<" segundos)\n";
	cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
	while(!(cin>>iPos))
	{
		LimpiarCin();
		cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
	}
	while(iPos!=-1)
	{
		cout<<"En la posicion "<<iPos<<" se encuentra el nro "<<v2[iPos]<<endl;
		cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
		while(!(cin>>iPos))
		{
			LimpiarCin();
			cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
		}
	}
}

void InsertionSort(ty_vec v2)
{
int i, j, index;
clock_t t;
t = clock();

	cout<<"Realizando ordenamiento utilizando InsertionSort LINEAL...";
	for (i=1; i<NARRAY; i++)
	{
		index = v2[i];
		j = i;
		while ((j>0) && (v2[j-1] > index))
		{
			v2[j] = v2[j-1];
			j = j - 1;
		}
		v2[j] = index;
	}
	t = clock() - t;
	cout<<"Ordenamiento realizado.\n";
	cout<<"Tardo "<<t<<" ticks del reloj. ( "<< ((float)t)/CLOCKS_PER_SEC<<" segundos)\n";
}

void InsertionSortParalelo(ty_vec v2)
{
int i, j, index;
long iPos;
clock_t t;
t = clock();

	cout<<"Realizando ordenamiento utilizando InsertionSort PARALELO...";
	#pragma omp parallel for default(none) shared(v2) private(i, j, index)
	for (i=1; i<NARRAY; i++)
	{
		index = v2[i];
		j = i;
		while ((j>0) && (v2[j-1] > index))
		{
			v2[j] = v2[j-1];
			j = j - 1;
		}
		v2[j] = index;
	}
	t = clock() - t;
	cout<<"Ordenamiento realizado.\n";
	cout<<"Tardo "<<t<<" ticks del reloj. ( "<< ((float)t)/CLOCKS_PER_SEC<<" segundos)\n";
	cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
	while(!(cin>>iPos))
	{
		LimpiarCin();
		cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
	}
	while(iPos!=-1)
	{
		cout<<"En la posicion "<<iPos<<" se encuentra el nro "<<v2[iPos]<<endl;
		cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
		while(!(cin>>iPos))
		{
			LimpiarCin();
			cout<<"Ingrese la posicion que desea ver (-1 para volver al menu principal):";
		}
	}
}

void Menu()
{   cout<<"\n                                          ..............................................";
    cout<<"\n                                          ...UNIVERSIDAD NACIONAL DEL ALTIPLANO PUNO....";
    cout<<"\n                                          ........ESTADISTICA E INFORMATICA.............";
    cout<<"\n                                          .....TRABAJO FINAL COMPUTACION PARALELA.......";
    cout<<"\n                                    ...COMPARACION DE METODOS DE ORDENAMIENTO PARALELO VS LINEAL...\n";
    cout<<"\nDOCENTE : RENZO APAZA CUTIPA :\n";
    cout<<"\nNOMBRES :";
    cout<<"\nValeria  Jara Sanca";
    cout<<"\nMilagros Arocutipa Cusacani ";
    cout<<"\nFranco   Champio Mamani";
    cout<<"\nMICHAEL  CUTIPA SANTI\n";
    cout<<"\n================PROGRAMA===================\n";
    cout<<"===============MENU PRINCIPAL==============\n";
	cout<<" Presione:\n";
	cout<<" 1 Ordenamiento burbujeo.\n";
	cout<<" 2 Ordenamiento burbujeo bidireccional.\n";
	cout<<" 3 Ordenamiento shellSort.\n";
	cout<<" 4 Ordenamiento InsertionSort.\n";
	cout<<" 0 para salir.\n";
}

//MENU PRINCIPLA
int main()
{
	int			iOpcion = -1;
	int			f=0;
	ty_vec		vec_desord;
	//omp_set_num_threads(4);

		srand(2);//Siempre usar la semilla antes de llamar la funcion CargarVectorDesordenado, para generar siempre los mismos numeros
		system("cls");
		CargarVectorDesordenado(vec_desord);//Cargo en vector los numeros desordenados
		Menu();
		if(!(cin>>iOpcion))
		{
			LimpiarCin();
		}
		while(iOpcion!=0)
		{
			switch(iOpcion)
			{
				case 1:
					Burbujeo(vec_desord);
					srand(2);
					CargarVectorDesordenado(vec_desord);//Se vuelve a cargar el vector desordenado
					BurbujeoParalelo(vec_desord);
					srand(2);
					CargarVectorDesordenado(vec_desord);//Se vuelve a cargar el vector desordenado
				break;
				case 2:
					BurbujeoBidireccional(vec_desord);
					srand(2);
					CargarVectorDesordenado(vec_desord);//Se vuelve a cargar el vector desordenado
					BurbujeoBidireccionalParalelo(vec_desord);
					srand(2);
					CargarVectorDesordenado(vec_desord);//Se vuelve a cargar el vector desordenado
				break;
				case 3:
					Shell(vec_desord);
					srand(2);
					CargarVectorDesordenado(vec_desord);//Se vuelve a cargar el vector desordenado
					ShellParalelo(vec_desord);
					srand(2);
					CargarVectorDesordenado(vec_desord);//Se vuelve a cargar el vector desordenado
				break;
				case 4:
					InsertionSort(vec_desord);
					srand(2);
					CargarVectorDesordenado(vec_desord);//Se vuelve a cargar el vector desordenado
					InsertionSortParalelo(vec_desord);
					srand(2);
					CargarVectorDesordenado(vec_desord);//Se vuelve a cargar el vector desordenado
				break;
				default:
				cout<<"Debe ingresar un numero entre 0 y 4";
			}
			Menu();
			if(!(cin>>iOpcion))
			{
				LimpiarCin();
			}
		}
}

void LimpiarCin()
{
	cin.clear();//arreglo el stream
	cin.ignore(200,'\n');//ignora el ingreso de hasta 200 caracteres
}
