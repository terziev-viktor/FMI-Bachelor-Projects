#include "Student.h"
#include "ptr_keeper.hpp"
#include "singly_linked_list.hpp"

using namespace std;

int main()
{
	singly_linked_list<Student> * list = new singly_linked_list<Student>();
	list->push_back(Student("B", UNI::FMI))
		.push_back(Student("C", UNI::TU))
		.push_front(Student("A", UNI::UNWE));

	singly_linked_list<Student> * list1 = new singly_linked_list<Student>();

	list1->push_back(Student("B", UNI::FMI))
		.push_back(Student("C", UNI::TU))
		.push_front(Student("A", UNI::UNWE));


	ptr_keeper<singly_linked_list<Student>> keeper;
	keeper.keep(list);
	keeper.keep(list1);

	keeper.get(1)->remove_front();
	for (size_t i = 0; i < list1->get_count(); ++i)
	{
		cout << (*list1)[i] << endl;
	}
	keeper.delete_elements();
	return 0;
}