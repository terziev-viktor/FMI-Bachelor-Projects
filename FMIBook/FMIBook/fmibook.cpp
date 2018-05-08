#include <iostream>
#include "Admin.h"

using std::cout;
using std::cin;
using std::endl;

int main()
{
	User * * users = new User*[2];
	users[0] = new Admin();
	users[1] = new Moderator();

	users[0]->remove();
	users[1]->remove();

	delete users[0];
	delete users[1];
	delete[] users;
	return 0;
}