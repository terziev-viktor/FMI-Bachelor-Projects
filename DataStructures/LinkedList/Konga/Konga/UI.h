#pragma once
#include "ptr_keeper.hpp"
#include "doubly_linked_list.hpp"
#include "Student.h"
#include "cstring.h"

class command
{
private:
	cstring trigger;
public:
	command(const cstring & trigger);
	cstring get_trigger() const;
	void set_trigger(const cstring & trigger);
	virtual void execute(ptr_keeper<doubly_linked_list<Student>> &) const = 0;
};

class append : public command
{
public:
	append() :command("append"){}

	// Inherited via command
	virtual void execute(ptr_keeper<doubly_linked_list<Student>>&) const override;
};

class remove_last : public command
{
public:
	remove_last() :command("removeLast") {}

	// Inherited via command
	virtual void execute(ptr_keeper<doubly_linked_list<Student>>&) const override;
};

class remove_first : public command
{
public:
	remove_first():command("removeFirst"){}
	
	// Inherited via command
	virtual void execute(ptr_keeper<doubly_linked_list<Student>>&) const override;
};

class UI
{
private:
	ptr_keeper<const command> commands;
	ptr_keeper<doubly_linked_list<Student>> kongas;
public:
	UI();
	~UI();
	void add_command(const command * c);
	void execute_command(const cstring & c);
};

