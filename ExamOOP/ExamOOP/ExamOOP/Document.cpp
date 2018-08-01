#include "Document.h"
#include <fstream>
#include <stdexcept>
using std::ifstream;
using std::ofstream;

Document::Document(const Basic_WordFactory * concrete_factory)
{
	this->factory = concrete_factory;
}

Document::Document(const Document & other)
{
	for (size_t i = 0; i < other.messages.count(); i++)
	{
		this->messages.add(new Message(*other.messages[i]));
	}
}

void Document::load(const String & path)
{
	ifstream file;
	file.open(path.get_as_char_array());
	if (!file)
	{
		throw std::exception("File does not exist");
	}
	
	char buffer[281];
	buffer[0] = '\0';
	do
	{
		file.getline(buffer, 280);
		if (!file)
		{
			break;
		}
		this->messages.add(new Message(buffer, factory));
		buffer[0] = '\0';
	} while (file);
}

const Vector<Message*> & Document::get_messages() const
{
	return this->messages;
}

Vector<Hashtag> Document::get_hashtags() const
{
	Vector<Hashtag> result;
	const Vector<Message*> & this_msgs = this->get_messages();
	for (size_t i = 0; i < this_msgs.count(); ++i)
	{
		const Vector<Word*> & wrds = this_msgs[i]->get_words();
		for (size_t j = 0; j < wrds.count(); ++j)
		{
			if (wrds[j]->get_type() == "Hashtag")
			{
				result.add(Hashtag(wrds[j]->get_value()));
			}
		}
	}
	return result;
}

Document & Document::operator+(const Message & message)
{
	this->messages.add(new Message(message));
	return *this;
}

Document & Document::operator+=(const Document & other)
{
	for (size_t i = 0; i < other.messages.count(); i++)
	{
		this->messages.add(new Message(*other.messages[i]));
	}
	return *this;
}

Message & Document::operator[](int i)
{
	return *this->messages[i];
}

const Message & Document::operator[](int i) const
{
	return *this->messages[i];
}

Vector<Word> Document::filter_words(const String & filter) const
{
	// todo
	return Vector<Word>();
}

Document::~Document()
{
	delete this->factory;
}

std::ostream & operator<<(std::ostream & os, const Document & obj)
{
	const Vector<Message*> & msgs = obj.get_messages();
	for (size_t i = 0; i < msgs.count(); i++)
	{
		os << *msgs[i] << '\n';
	}
	return os;
}
