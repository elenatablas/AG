/* ------------------------- PROBLEMA DE LAS N REINAS ----------------------- */
#include <ga/GASimpleGA.h>   //  Algoritmo Genetico simple
#include <ga/GA1DArrayGenome.h>   // Genoma --> array de enteros (dim. 1) alelos
#include <iostream>
#include <fstream>
using namespace std;
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

float Objective(GAGenome &);   // Funcion objetivo --> al final

GABoolean Termina(GAGeneticAlgorithm &);   // Funcion de terminacion --> al final

string mayuscula(string str1);  // Funcion convertir a mayúsculas --> al final

void mostrarTablero(GAGenome &);   // Mostrar solución (tablero) --> al final

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");

    int nreinas=atoi(argv[1]);
    cout << "\n";
    cout << "Problema de las " << nreinas << " reinas mediante AG\n";
    cout << "_____________________________________\n\n\n";
    // Declaramos variables para los parametros del GA y las inicializamos
    int popsize = atoi(argv[2]);
    int ngen = atoi(argv[3]);
    string sel = mayuscula(string(argv[4]));    
    string cross = mayuscula(string(argv[5]));  
    string muta = mayuscula(string(argv[6]));   
    float pcross = atof(argv[7]);
    float pmuta = atof(argv[8]);

    cout << "Valores de los parámetros: (" << popsize << ", " << ngen << ", " << sel << ", " << cross << ", " 
         << muta << ", " << pcross << ", " << pmuta << ")" << endl << endl;

    // Conjunto enumerado de alelos --> valores posibles de cada gen del genoma
    GAAlleleSet<int> alelos;
    for(int i=0;i<nreinas;i++) alelos.add(i);

    // Creamos el genoma y definimos operadores de inicio, cruce y mutación
    GA1DArrayAlleleGenome<int> genome(nreinas,alelos,Objective,NULL);

    if (cross.compare("ONEP")==0) genome.crossover(GA1DArrayAlleleGenome<int>::OnePointCrossover);
	else if (cross.compare("TWOP")==0) genome.crossover(GA1DArrayAlleleGenome<int>::TwoPointCrossover);

	if (muta.compare("FLIP")==0) genome.mutator(GA1DArrayAlleleGenome<int>::FlipMutator);
	else if (muta.compare("SWAP")==0) genome.mutator(GA1DArrayAlleleGenome<int>::SwapMutator);

    // Creamos el algoritmo genetico
    GASimpleGA ga(genome);

    // Inicializamos - minimizar funcion objetivo, tamaño poblacion, nº generaciones,
    // pr. cruce y pr. mutacion, selección y le indicamos que evolucione.
    ga.minimaxi(-1);
    ga.populationSize(popsize);
    ga.nGenerations(ngen);
    ga.pCrossover(pcross);
    ga.pMutation(pmuta);
    if (sel.compare("ROUL")==0) {
            GARouletteWheelSelector selector;
            ga.selector(selector);
            }
    else if (sel.compare("TOUR")==0) {
            GATournamentSelector selector;
            ga.selector(selector);
            }
    ga.terminator(Termina);
    ga.evolve(1);

    // Imprimimos el mejor individuo que encuentra el GA y su valor fitness
    cout << "El sistema encuentra la solución " << endl << endl;
    GA1DArrayAlleleGenome<int> & g = (GA1DArrayAlleleGenome<int> &)ga.statistics().bestIndividual();
    mostrarTablero(g);
    cout << "con " << ga.statistics().minEver() << " jaques. " << endl << endl;
    cout << " " << endl;
}

// Funcion objetivo.
float Objective(GAGenome& g) {
    GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;
    float jaques=0;

    for(int i=0; i<genome.length(); i++)
       for(int j=i+1;j<genome.length();j++)
            if ((genome.gene(i)==genome.gene(j))||(abs(j-i)==abs(genome.gene(j)-genome.gene(i)))) jaques++;
    return jaques;
}

// Funcion de terminacion
GABoolean Termina(GAGeneticAlgorithm & ga){
    if ( (ga.statistics().minEver()==0) ||
        (ga.statistics().generation()==ga.nGenerations()) ) return gaTrue;
    else return gaFalse;
}

// Convertir a mayúscula
string mayuscula(string str1)
{
    for (unsigned int i = 0; i < str1.length(); i++) str1[i] = toupper(str1[i]);
    return str1;
}

// Mostrar solución (Tablero)
void mostrarTablero (GAGenome& g)
{
  int i,j,n; // col

  GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;
  n = genome.length();

  for (i=0; i<n; i++) {
      for (j=0; j<n; j++) {
          if (genome.gene(j)==i) cout << "R "; 
          else cout << "- " ;
      }
      cout << endl;
  }
  cout << endl;
}
