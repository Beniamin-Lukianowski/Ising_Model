#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// allocate jest funkcją, która rozmieszcza randomowo -1 lub 1 na przestrzeni
// wektora wektorów tworząc bitmapę. Brak w c++ łatwej implementacji pythonowego
// rand.choise spowodował takie rozwiązanie

vector<vector<int>> allocate(int width1, string choise1, float posibility) {

  vector<vector<int>> vec1(width1);
  if (choise1 == "random") {
    for (int j = 0; j < width1; j++) {
      for (int i = 0; i < width1; i++) {
        int32_t random = rand() % 2;
        if (random == 0) {
          vec1[j].push_back(-1);
        } else {
          vec1[j].push_back(random);
        }
      }
    }
    return vec1;
  } else if (choise1 == "set") {
    for (int j = 0; j < width1; j++) {
      for (int i = 0; i < width1; i++) {
        float random = ((double)rand() / (RAND_MAX));
        if (random >= posibility) {
          vec1[j].push_back(-1);
        } else {
          vec1[j].push_back(1);
        }
      }
    }
    return vec1;
  } else {
    cout << "nothing was chosen";
    return vec1;
  }
};

// funkcja rysująca wykres M(T):
void plot(vector<float> x, vector<float> y){};

// funkcja licząca energie i prawdopodobieństwo zmiany dla dowolnej cząsteczki
// oraz zmienia stan jeżeli będzie to dane
void Do_Energy_and_Prop_Calc(vector<vector<int>> &vecE,
                                            vector<float> &Mags,
                                            vector<float> &Temps, int width,
                                            float bj) {

  // Wybranie dowolnego wiersza 0-input
  int randompos1 = (rand() % (width));
  // wybieranie dowolnej kolumny 0-input
  int randompos2 = (rand() % (width));
  width -= 1;
  int32_t E = 0;

  // cout << " \n pos1: " << randompos1 << " "
  //      << "pos2: " << randompos2 << " ";

  // Rozpisane warunki brzegowe
  // lewy góry róg:
  if (randompos1 == 0 && randompos2 == 0) {
    E = (vecE[0][1] * vecE[0][0]) + (vecE[1][0] * vecE[0][0]);
  }
  // prawy dolny róg:
  else if (randompos1 == width && randompos2 == width) {
    E = (vecE[width][width - 1] * vecE[width][width]) +
        (vecE[width - 1][width] * vecE[width][width]);
  }
  // lewy dolny róg:
  else if (randompos1 == width && randompos2 == 0) {
    E = (vecE[width - 1][0] * vecE[width][0]) +
        (vecE[width][1] * vecE[width][0]);
  }
  // prawy góry róg:
  else if (randompos1 == 0 && randompos2 == width) {
    E = (vecE[0][width - 1] * vecE[0][width]) +
        (vecE[1][width] * vecE[0][width]);
  }
  // inne przypadki:
  else if (randompos2 == 0) {
    E = (vecE[randompos1 + 1][0] * vecE[randompos1][0]) +
        (vecE[randompos1 - 1][0] * vecE[0][0]) +
        (vecE[randompos1][1] * vecE[0][0]);
  }

  else if (randompos1 == 0) {
    E = (vecE[0][randompos2 - 1] * vecE[0][randompos2]) +
        (vecE[0][randompos2 + 1] * vecE[0][randompos2]) +
        (vecE[1][randompos2] * vecE[0][randompos2]);
  }

  else if (randompos1 == width) {
    E = (vecE[randompos1 - 1][randompos2] * vecE[width][randompos2]) +
        (vecE[width][randompos2 - 1] * vecE[width][randompos2]) +
        (vecE[width][randompos2 + 1] * vecE[width][randompos2]);
  }

  else if (randompos2 == width) {
    E = (vecE[randompos1 - 1][width] * vecE[randompos1][width]) +
        (vecE[randompos1 + 1][width] * vecE[randompos1][width]) +
        (vecE[randompos1][width - 1] * vecE[randompos1][width]);
  }

  else {
    E = (vecE[randompos1 + 1][randompos2] * vecE[randompos1][randompos2]) +
        (vecE[randompos1 - 1][randompos2] * vecE[randompos1][randompos2]) +
        (vecE[randompos1][randompos2 + 1] * vecE[randompos1][randompos2]) +
        (vecE[randompos1][randompos2 - 1] * vecE[randompos1][randompos2]);
  }
  // cout << E << "\n";
  int32_t d_Energy = (-2) * E;
  if (d_Energy <= 0) {
    vecE[randompos1][randompos2] *= -1;
    // cout << "changed spin";
  }
  // dodać jeszcze zmianę stanu dla prawdopodobieństwa
  else if (d_Energy > 0 &&
           (exp(-bj * d_Energy) > (double)rand() / (RAND_MAX))) {
    vecE[randompos1][randompos2] *= -1;
    // cout << "changed spin";
  }

  // dodanie punktu dla wykresu M(T)
  float namagnesowanie;
  for (int j = 0; j < vecE.size(); j++) {
    for (int i = 0; i < vecE.size(); i++) {
      namagnesowanie += vecE[j][i];
    }
  }

  float Me = abs(namagnesowanie) / pow(vecE.size(), 2);
  Mags.push_back(Me);
  Temps.push_back(1 / bj);

};

// Klasa map - odpowiada bitmapie(macierzy spinów w tym przypadku) zapełnionej
// -1 i 1 dla dowolnej wartości długości int
class Map {
public:
  vector<vector<int>> vec;
  Map(int length, string choise, float posi) {
    vec = allocate(length, choise, posi);
  }
};

int main(int argc, char *argv[]) {
  // seedowanie randa
  srand(time(NULL));
  cout << "Set length and width of bitmap(NxN): \n";
  int input = 100;
  float d_T = 0.01;
  string inputstr;
  float posibility = 0;
  int iteracje = 0;
  cin >> input;
  cout << "\ninput 'random' for random spin distribution, 'set' to set "
          "propability for finding spin 1 (0-1): ";
  cin >> inputstr;
  if (inputstr == "set") {
    cout << "please input desired propability of finding 1: ";
    cin >> posibility;
  }
  cout << "\nplease specify how many iterations are you interested in: ";
  cin >> iteracje;
  cout << "\n";
  Map map(input, inputstr, posibility);
  vector<float> bj;
  vector<float> Magnes;
  vector<float> Te;

  float bjs = 0.1;
  for (float k = 0; k < iteracje; k++) {
    bj.push_back(bjs);
    bjs += 0.05;
  }

  // dla sprawdzenia poprawnej alokacji:
  // for (int j = 0; j < input; j++) {
  //   for (int i = 0; i < input; i++) {
  //     cout << map.vec[j][i] << " ";
  //   }
  //   cout << "\n";
  // }
  for (int i = 0; i < bj.size(); i++) {
    Do_Energy_and_Prop_Calc(map.vec, Magnes, Te, input, bj[i]);
  }
  plot(Te, Magnes);
}