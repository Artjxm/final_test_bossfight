#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx; 
int shared_variable = 0; 

void writer_thread() {
    for (int i = 1; i <= 10; ++i) {
        mtx.lock(); 
        shared_variable = i;
        std::cout << "Writer: Wrote " << shared_variable << std::endl;
        mtx.unlock(); 
    }
}

void reader_thread() {
    int value;
    for (int i = 1; i <= 10; ++i) {
        mtx.lock(); 
        value = shared_variable;
        mtx.unlock();
        std::cout << "Reader: Read " << value << std::endl;
    }
}

int main() {
    std::thread writer(writer_thread);
    std::thread reader(reader_thread);

    writer.join();
    reader.join();

    return 0;
}