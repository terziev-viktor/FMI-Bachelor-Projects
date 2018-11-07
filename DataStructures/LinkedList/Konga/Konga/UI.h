#pragma once
#include "ptr_keeper.hpp"
#include "doubly_linked_list.hpp"
#include "Student.h"

namespace konga_commands
{

	class command
	{
	private:
		string trigger;
	public:
		command(const string & trigger);
		const string & get_trigger() const;
		void set_trigger(const string & trigger);
		virtual string execute(ptr_keeper<doubly_linked_list<Student>> &, bool &) const = 0;
	};

	class print : public command
	{
	public:
		print() :command("print") {}

		// Inherited via command
		virtual string execute(ptr_keeper<doubly_linked_list<Student>>&, bool&) const override;
	};

	class append : public command
	{
	public:
		append() :command("append") {}

		// Inherited via command
		virtual string execute(ptr_keeper<doubly_linked_list<Student>>&, bool&) const override;
	};

	class remove_last : public command
	{
	public:
		remove_last() :command("removeLast") {}

		// Inherited via command
		virtual string execute(ptr_keeper<doubly_linked_list<Student>>&, bool&) const override;
	};

	class remove_first : public command
	{
	public:
		remove_first() :command("removeFirst") {}

		// Inherited via command
		virtual string execute(ptr_keeper<doubly_linked_list<Student>>&, bool&) const override;
	};

	class quit : public command
	{
	public:
		quit() :command("quit") {}

		// Inherited via command
		virtual string execute(ptr_keeper<doubly_linked_list<Student>>&, bool&) const override;
	};

	class remove : public command
	{
	public:
		remove() :command("remove") {}

		// Inherited via command
		virtual string execute(ptr_keeper<doubly_linked_list<Student>>&, bool &) const override;
	};

	class merge : public command
	{
	public:
		merge() : command("merge") {}

		// Inherited via command
		virtual string execute(ptr_keeper<doubly_linked_list<Student>>&, bool &) const override;
	};
}
//	User interface for working with kongas. Manages konga queues and command execution
class UI
{
private:
	bool is_running;
	ptr_keeper<const konga_commands::command> commands;
	ptr_keeper<doubly_linked_list<Student>> kongas;
public:
	UI();
	~UI();
	void add_command(const konga_commands::command * c);
	void execute_command(const string & c);
	void run();
};

