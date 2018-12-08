#include "Hash.hpp"
using namespace std;

struct Student
{
	char name[20];
	size_t age;
};

int main()
{
	Hash<char*, Student> students;
	Student pesho;
	pesho.age = 20;
	students.Map(pesho);
	return 0;
}