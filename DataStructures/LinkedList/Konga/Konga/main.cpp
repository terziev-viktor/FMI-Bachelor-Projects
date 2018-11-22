#include "UI.h"

using namespace std;
using namespace konga_commands;

int main()
{
	UI konga_manager;
	konga_manager.add_command(new (std::nothrow) append());
	konga_manager.add_command(new (std::nothrow) remove_first());
	konga_manager.add_command(new (std::nothrow) remove_last());
	konga_manager.add_command(new (std::nothrow) quit());
	konga_manager.add_command(new (std::nothrow) print());
	konga_manager.add_command(new (std::nothrow) konga_commands::remove());
	konga_manager.add_command(new (std::nothrow) konga_commands::merge());

	konga_manager.run();


	return 0;
}