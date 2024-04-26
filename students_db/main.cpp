#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>

struct Student {
   int id;
   std::string name;
   int age;
};

class StudentDatabase {
private:
   std::map<int, std::shared_ptr<Student>> students;

public:
   void addStudent(int id, const std::string& name, int age) {
       auto student = std::make_shared<Student>();
       student->id = id;
       student->name = name;
       student->age = age;
       students[id] = student;
       std::cout << "Student added: id: " << id << ", name: " << name << ", age: " << age << '\n';
   }

   void removeStudent(int id) {
       auto it = students.find(id);
       if (it != students.end()) {
           students.erase(it);
           std::cout << "Student with id " << id << " removed.\n";
       } else {
           std::cout << "Student with id " << id << " was not found in database.\n";
       }
   }

   void getStudent(int id) {
       auto it = students.find(id);
       if (it != students.end()) {
           const Student& student = *(it->second);
           std::cout << "Student: id: " << student.id << ", name: " << student.name << ", age: " << student.age << std::endl;
       } else {
           std::cout << "Student with id " << id << " was not found in database.\n";
       }
   }
};

int main() {
   StudentDatabase db;

   db.addStudent(1, "Daniil Tyan", 28);
   db.addStudent(2, "Nastya", 26);
   db.addStudent(3, "Artjom", 22);

   db.getStudent(2);
   db.removeStudent(3);
   db.getStudent(3);

   return 0;
}