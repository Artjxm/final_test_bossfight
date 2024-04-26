#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int BUFFER_SIZE = 1024;
const int CIRCULAR_BUFFER_SIZE = 4096;

class CircularBuffer {
private:
    std::vector<char> buffer;
    int head, tail;

public:
    CircularBuffer() : buffer(CIRCULAR_BUFFER_SIZE), head(0), tail(0) {}

    void put(char c) {
        buffer[head] = c;
        head = (head + 1) % CIRCULAR_BUFFER_SIZE;
        if (head == tail) {
            tail = (tail + 1) % CIRCULAR_BUFFER_SIZE;
        }
    }

    char get() {
        if (head == tail) {
            return '\0';
        }
        char c = buffer[tail];
        tail = (tail + 1) % CIRCULAR_BUFFER_SIZE;
        return c;
    }
};

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char read_buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Error setting socket options" << std::endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Error listening on socket" << std::endl;
        return -1;
    }

    std::cout << "Server started on port 8080" << std::endl;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        CircularBuffer buffer;
        char ack[] = "Message received\n";

        while (true) {
            int valread = read(new_socket, read_buffer, BUFFER_SIZE);
            if (valread == 0) {
                break;
            } else if (valread < 0) {
                std::cerr << "Error reading from socket" << std::endl;
                break;
            }

            for (int i = 0; i < valread; i++) {
                buffer.put(read_buffer[i]);
            }

            send(new_socket, ack, sizeof(ack), 0);

            std::string message;
            char c;
            while ((c = buffer.get()) != '\0') {
                message += c;
            }

            if (!message.empty()) {
                std::cout << "Received message: " << message << std::endl;
            }
        }

        close(new_socket);
    }

    return 0;
}