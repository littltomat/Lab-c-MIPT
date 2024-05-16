#include <iostream>
#include <chrono>
#include <fstream>
#include <istream>


struct DynamicArray {
    int* data = nullptr;
    int size = 0;
    int capacity = 0;
};

void Clear(DynamicArray& array){
    delete[] array.data;
    array.capacity = 0;
    array.size = 0;
    array.data = nullptr;
}

auto TimerStart() {
    return std::chrono::steady_clock::now();
}

int TimerStop(std::chrono::_V2::steady_clock::time_point  begin) {
    auto end = std::chrono::steady_clock::now();
    auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    return static_cast<int>(time_span.count());
}

void WriteInFile(const std::string& filename, int* result, int size){
    std::ofstream fout(filename, std::ios::app);
    for (int i = 0; i < size; i++){
        fout << result[i] << ' ';
    }
    fout << '\n';
    fout.close();
}

DynamicArray PushBack(const DynamicArray& data) {
    DynamicArray result;
    result.capacity = data.capacity + 1;
    result.size = data.size + 1;
    result.data = new int[result.capacity];
    for (int i = 0; i < data.size; i++) {
        result.data[i] = data.data[i];
    }
    return result;
}

DynamicArray PushBackStep(DynamicArray data, int step) {
    DynamicArray result;
    result.capacity = data.capacity + step;
    result.size = data.size;
    result.data = new int[result.capacity];
    for (int i = 0; i < result.size; i++) {
        result.data[i] = data.data[i];
    }
    return result;
}

DynamicArray PushBackMultiply(const DynamicArray& data, int multiply) {
    DynamicArray result;
    result.capacity = data.capacity * multiply;
    result.size = data.size;
    result.data = new int[result.capacity];
    for (int i = 1; i < result.size; i++) {
        result.data[i-1] = data.data[i-1];
    }
    return result;
}

int* Test1(DynamicArray& array, int END_SIZE = 10000){
    int *result = new int[END_SIZE];
    for (int i = 0; i < END_SIZE; i++){
        auto ts = TimerStart();
        if (array.capacity == array.size){
            array = PushBack(array);
        }
        array.data[i] = INT32_MAX;
        result[i] = TimerStop(ts);
    }
    return result;
}

int* Test2(DynamicArray& array, int step, int END_SIZE = 10000){
    int *result = new int[END_SIZE];
    for (int i = 0; i < END_SIZE; i++){
        if (array.capacity == array.size){
            auto ts = TimerStart();
            array = PushBackStep(array, step);
            result[i] = TimerStop(ts);
        } else {
            auto ts = TimerStart();
            array.data[i] = INT32_MAX;
            result[i] = TimerStop(ts);
        }
        array.size++;
    }
    return result;
}

int* Test3(DynamicArray& array, int step, int END_SIZE =  10000, int MULTIPLY = 2){
    int *result = new int[END_SIZE];
    array.capacity = 1;
    array.size = 1;
    for (int i = 0; i < END_SIZE; i++){
        auto ts = TimerStart();
        if (array.capacity == array.size){
            array = PushBackMultiply(array, MULTIPLY);
            array.data[i] = INT32_MAX;
        }
        array.data[i] = INT32_MAX;
        array.size++;
        result[i] = TimerStop(ts);
    }
    return result;
}


int main(){
    setlocale(LC_ALL, "Russian");
    std::string txtNames[] = { "firstTest.txt", "secondTest.txt", "thirdTest.txt" };

    int config[4] = {0};
    std::string _config[4];
    std::ifstream MyReadFile("config.txt");
    int i = 0;
    while (std::getline(MyReadFile, _config[i])) {
        config[i] = std::__cxx11::stoi(_config[i]);
        i++;
    }
    MyReadFile.close();

    DynamicArray array;
    array.capacity = 0;
    array.size = 0;

    for (int i = 0; i < config[1]; i++){
        WriteInFile(txtNames[0], Test1(array), config[0]);
        Clear(array);

        WriteInFile(txtNames[1], Test2(array, config[2]), config[0]);
        Clear(array);

        WriteInFile(txtNames[2], Test3(array, config[3]), config[0]);
        Clear(array);
    }
    system("jupyter notebook graph.ipynb");
    return 0;
}