#include <iostream>
#include <cstdlib>
#include "matrice.h"
#include <fstream>
using namespace std;

bool riempi_unica(int &n_cicli_backtracking,matrice *matrix,int i,int j);
void riempi_plurime(int &n_soluz,int &n_cicli_backtracking,matrice *matrix,int i,int j);
void set_clues(matrice *matrix);
void read_clues(matrice * &matrix);
