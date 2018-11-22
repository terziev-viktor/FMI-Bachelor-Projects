#pragma once
#include "ptr_keeper.hpp"
#include "dynamic_arr.h"

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
		virtual string execute(dynamic_arr &, bool &) const = 0;
	};

	class print : public command
	{
	public:
		print() :command("print") {}

		// Inherited via command
		virtual string execute(dynamic_arr&, bool&) const override;
	};

	class append : public command
	{
	public:
		append() :command("append") {}

		// Inherited via command
		virtual string execute(dynamic_arr&, bool&) const override;
	};

	class remove_last : public command
	{
	public:
		remove_last() :command("removeLast") {}

		// Inherited via command
		virtual string execute(dynamic_arr&, bool&) const override;
	};

	class remove_first : public command
	{
	public:
		remove_first() :command("removeFirst") {}

		// Inherited via command
		virtual string execute(dynamic_arr&, bool&) const override;
	};

	class quit : public command
	{
	public:
		quit() :command("quit") {}

		// Inherited via command
		virtual string execute(dynamic_arr&, bool&) const override;
	};

	class remove : public command
	{
	public:
		remove() :command("remove") {}

		// Inherited via command
		virtual string execute(dynamic_arr&, bool &) const override;
	};

	class merge : public command
	{
	public:
		merge() : command("merge") {}

		// Inherited via command
		virtual string execute(dynamic_arr&, bool &) const override;
	};
}
// User interface for working with kongas. Manages konga queues and command execution
// The commands are a hierarchy so Im keeping pointers to different commands using a special data structure for keeping pointers
// Kongas are small objects that are made cheap to swap using a method called swap_with()

class UI
{
private:
	bool is_running;
	ptr_keeper<const konga_commands::command> commands;
	dynamic_arr kongas;
public:
	UI();
	~UI();
	void add_command(const konga_commands::command * c);
	void execute_command(const string & c);
	void run();
};

