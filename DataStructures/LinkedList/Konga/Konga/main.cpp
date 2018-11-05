#include "UI.h"

using namespace std;
using namespace konga_commands;

int main()
{
	UI konga_manager;
	konga_manager.add_command(new append());
	konga_manager.add_command(new remove_first());
	konga_manager.add_command(new remove_last());
	konga_manager.add_command(new quit());
	konga_manager.add_command(new print());
	konga_manager.add_command(new konga_commands::remove());
	konga_manager.add_command(new konga_commands::merge());

	konga_manager.run();

	return 0;
}