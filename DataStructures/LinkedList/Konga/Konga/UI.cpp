#include "UI.h"
using namespace konga_commands;

UI::UI()
{
	// Keeping one konga queue at first
	// User can add more
	doubly_linked_list<Student>	* konga = new doubly_linked_list<Student>();
	konga->push_back(Student("Integral4o", UNI::FMI));
	this->kongas.keep(konga);
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

void UI::execute_command(const string & c)
{
	bool command_found = false;
	for (size_t i = 0; i < this->commands.get_count(); ++i)
	{
		if (this->commands.give(i)->get_trigger() == c)
		{
			try
			{
				std::cout << this->commands.give(i)->execute(this->kongas, this->is_running) << std::endl;
				command_found = true;
				break;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}
	if (!command_found)
	{
		std::cout << "Unknown command" << std::endl;
	}
}

void UI::run()
{
	is_running = true;
	string inp;
	while (is_running)
	{
		if (this->kongas.get_count() == 0)
		{
			std::cout << "No more kongas, the only available command is 'quit'" << std::endl;
			std::cin >> inp;
			if (inp == "quit")
			{
				this->execute_command(inp);
			}
		}
		else
		{
			std::cin >> inp;
			this->execute_command(inp);
		}
	}
}

command::command(const string & trigger)
{
	this->set_trigger(trigger);
}

const string & command::get_trigger() const
{
	return this->trigger;
}

void command::set_trigger(const string & trigger)
{
	this->trigger = trigger;
}

string append::execute(ptr_keeper<doubly_linked_list<Student>>& s, bool&) const
{
	string name, uni_str;
	int line;
	std::cin >> name >> uni_str >> line;
	UNI uni = string_to_UNI(uni_str);
	if (line >= s.get_count())
	{
		throw std::exception("Invalid konga index");
	}
	doubly_linked_list<Student> * konga = s.give(line);
	const Student & last_st = konga->get_last();
	if (uni == UNI::FMI && (last_st.get_UNI() == UNI::FMI || last_st.get_UNI() == UNI::TU))
	{
		konga->push_back(Student(name, uni));
	}
	else if (uni == UNI::TU && (last_st.get_UNI() == UNI::TU || last_st.get_UNI() == UNI::UNWE))
	{
		konga->push_back(Student(name, uni));
	}
	else if (uni == UNI::UNWE && (last_st.get_UNI() == UNI::UNWE || last_st.get_UNI() == UNI::FMI))
	{
		konga->push_back(Student(name, uni));
	}
	else
	{
		throw std::exception("Incompatible people");
	}
	return "Added student to konga";
}

string remove_last::execute(ptr_keeper<doubly_linked_list<Student>>& s, bool&) const
{
	int line;
	std::cin >> line;
	doubly_linked_list<Student> * konga = s.give(line);
	konga->remove_back();
	if (konga->empty())
	{
		s.give_up(line);
		delete konga;
	}
	return "removed last student from konga";
}

string remove_first::execute(ptr_keeper<doubly_linked_list<Student>>& s, bool&) const
{
	int line;
	std::cin >> line;
	doubly_linked_list<Student> * konga = s.give(line);
	konga->remove_front();
	if (konga->empty())
	{
		s.give_up(line);
		delete konga;
	}
	return "removed first student from konga";
}

string quit::execute(ptr_keeper<doubly_linked_list<Student>>&, bool& r) const
{
	r = false;
	return "Exiting konga manager";
}

string print::execute(ptr_keeper<doubly_linked_list<Student>>& s, bool &) const
{
	for (size_t i = 0; i < s.get_count(); ++i)
	{
		std::cout << "line " << i << ": ";
		doubly_linked_list<Student> * konga = s.give(i);
		for (doubly_linked_list<Student>::iterator it = konga->begin(); !it.is_done(); ++it)
		{
			std::cout << *it << " <- ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return "printed all kongas";
}

string remove::execute(ptr_keeper<doubly_linked_list<Student>>& s, bool &) const
{
	string name;
	int line;
	std::cin >> name >> line;
	doubly_linked_list<Student> * konga = s.give(line);
	size_t at = 0;
	bool found = false;
	for (doubly_linked_list<Student>::iterator i = konga->begin(); !i.is_done(); ++i)
	{
		if ((*i).get_name() == name)
		{
			found = true;
			break;
		}
		++at;
	}
	if (!found)
	{
		throw std::exception("Student not found");
	}
	doubly_linked_list<Student> * cut = konga->cut(at);
	if (!cut->empty())
	{
		cut->remove_front();

		if (!cut->empty())
		{
			s.keep(cut);
		}
		else
		{
			delete cut;
		}
	}
	else
	{
		delete cut;
	}
	if (konga->empty())
	{
		delete konga;
		s.give_up(line);
	}
	return "removed student from konga";
}

string konga_commands::merge::execute(ptr_keeper<doubly_linked_list<Student>>& s, bool &) const
{
	int line1, line2;
	std::cin >> line1 >> line2;
	doubly_linked_list<Student> * konga1 = s.give(line1);
	doubly_linked_list<Student> * konga2 = s.give(line2);
	konga1->append(*konga2);
	s.give_up(line2);
	delete konga2;
	return "Merged two kongas";
}
