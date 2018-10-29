#include "UI.h"


UI::UI()
{
}


UI::~UI()
{
	this->commands.delete_elements();
	this->kongas.delete_elements();
}

void UI::add_command(const command * c)
{
	this->commands.keep(c);
}

void UI::execute_command(const cstring & c)
{
	for (size_t i = 0; i < this->commands.get_count(); ++i)
	{
		if (this->commands.get(i)->get_trigger() == c)
		{
			this->commands.get(i)->execute(this->kongas);
		}
	}
}

command::command(const cstring & trigger)
{
	this->trigger = trigger;
}

cstring command::get_trigger() const
{
	return this->trigger;
}

void command::set_trigger(const cstring & trigger)
{
	this->trigger = trigger;
}

void append::execute(ptr_keeper<doubly_linked_list<Student>>& s) const
{

}

void remove_last::execute(ptr_keeper<doubly_linked_list<Student>>& s) const
{
}

void remove_first::execute(ptr_keeper<doubly_linked_list<Student>>& s) const
{
}
