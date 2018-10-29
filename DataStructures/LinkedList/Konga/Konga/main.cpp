#include "UI.h"

using namespace std;

int main()
{
	UI konga_manager;
	konga_manager.add_command(new append());
	konga_manager.add_command(new remove_first());
	konga_manager.add_command(new remove_last());

	return 0;
}