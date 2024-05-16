#include <iostream>
#include <fstream>  // запись в файл
#include <chrono>
#include <random>


void printArray(int arr[], int MAX) {
    for (int i = 0; i < MAX; i++){
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;
}

int fillArrayAndKey(int array[], int seed, int size) {
    std::default_random_engine rng(seed);
    std::uniform_int_distribution<unsigned> dstr(0, size);

    for (int i = 0; i < size; i++){
        array[i] = dstr(rng);
    }

    int key = dstr(rng);
    return key;
}

int searchLinear(int array[], int MAX, int key, int limRight, int limLeft = 0) {
    auto begin = std::chrono::steady_clock::now();  // начало отсчета
    for (int i = limLeft; i < limRight; i++){
        if (array[i] == key){
            auto end = std::chrono::steady_clock::now();  // средний случай
            auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            return time_span.count();
        }
    }
    auto end = std::chrono::steady_clock::now();  // худший случай (key > size или просто не повезло)
    auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    return time_span.count();
}

int searchBin(int array[], int MAX, int key, int limRight, int limLeft = 0) {
    int mid = 0;
    auto begin = std::chrono::steady_clock::now();
    while (limLeft <= limRight) {
        mid = limLeft + (limRight - limLeft) / 2;
        if (array[mid] == key) {
            auto end = std::chrono::steady_clock::now();
            auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            return time_span.count();
        }
        if (array[mid] < key) {
            limLeft = mid + 1; // Ищем справа от середины
        } else {
            limRight = mid - 1; // Ищем слева от середины
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin); 
    return time_span.count();
}

int main() {

    int MIN = 100;
    int MAX = 500000;
    int STEP = 100;
    int LOOP_TIMES = 10000;
    int SEED = 1;

    unsigned int seed = SEED;
    int array[MAX] = {0};

    for (int i = 0; i < LOOP_TIMES; i++){  // количество повторений генерации массива 
        int key = fillArrayAndKey(array, seed, MAX);

        std::ofstream foutLinAvg("outputLinAvg.txt", std::ios::app);
        for (int i = MIN; i <= MAX; i += STEP){
            foutLinAvg << searchLinear(array, MAX, array[key], i) << ' ';
        }
        foutLinAvg << '\n';
        foutLinAvg.close();

        std::ofstream foutLinWorst("outputLinWorst.txt", std::ios::app);
        for (int i = MIN; i < MAX; i += STEP){
            foutLinWorst << searchLinear(array, MAX, MAX + 2, i) << ' ';
        }
        foutLinWorst << '\n';
        foutLinWorst.close();

        for (int i = 0; i < MAX; i++){  // делаем упорядоченный массив
            array[i] = i;
        }

        std::ofstream foutBinAvg("outputBinAvg.txt", std::ios::app);
        for (int i = MIN; i < MAX; i += STEP){
            foutBinAvg << searchBin(array, MAX, array[key], i) << ' ';
        }
        foutBinAvg << '\n';
        foutBinAvg.close();

        std::ofstream foutBinWorst("outputBinWorst.txt", std::ios::app);
        for (int i = MIN; i < MAX; i += STEP){
            foutBinWorst << searchBin(array, MAX, MAX + 2, i) << ' ';
        }
        foutBinWorst << '\n';
        foutBinWorst.close();

        seed += 10;
        std::cout << "Current progress: " << static_cast<float>(i)/LOOP_TIMES*100 << "%\n";
    }
    return 0;
}