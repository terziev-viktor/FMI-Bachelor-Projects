#include "Document.h"
#include <fstream>
#include <stdexcept>
using std::ifstream;
using std::ofstream;

Document::Document(const Basic_WordFactory * concrete_factory)
{
	this->factory = concrete_factory;
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
		this->messages.add(Message(buffer, this->get_factory()));
		buffer[0] = '\0';
	} while (file);
}

const Vector<Message> & Document::get_messages() const
{
	return this->messages;
}

Vector<Hashtag> Document::get_hashtags() const
{
	Vector<Hashtag> result;
	const Vector<Message> & this_msgs = this->get_messages();
	for (size_t i = 0; i < this_msgs.count(); ++i)
	{
		const Vector<Word*> & wrds = this_msgs[i].get_words();
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

const Basic_WordFactory * Document::get_factory() const
{
	if (this->factory)
	{
		return this->factory;
	}
	throw std::exception("Factory not set");
}

Document & Document::operator+(const Message & message)
{
	this->messages.add(Message(message));
	return *this;
}

Document & Document::operator+=(const Document & other)
{
	for (size_t i = 0; i < other.messages.count(); i++)
	{
		this->messages.add(Message(other.messages[i]));
	}
	return *this;
}

const Message & Document::operator[](const String & str) const
{
	Word * word = this->get_factory()->create_word(str);
	float max = 0.0f;
	float cur = 0.0f;
	size_t index = 0;
	for (size_t i = 0; i < this->get_messages().count(); i++)
	{
		cur = this->get_messages()[i].compare(*word);
		if (max < cur)
		{
			max = cur;
			index = i;
		}
	}
	return this->get_messages()[index];
}

Vector<Message> Document::filter_messages(const String & filter) const
{
	Vector<Message> result;
	const Vector<Message> & this_messages = this->get_messages();
	Vector<Cmp_Index> comparison_results;
	Word * word = this->get_factory()->create_word(filter);
	for (size_t i = 0; i < this_messages.count(); i++)
	{
		Cmp_Index cmp;
		cmp.comparison = this_messages[i].compare(*word);
		cmp.index = i;
	}
	for (size_t i = 0; i < comparison_results.count(); i++)
	{
		for (size_t j = 0; j < comparison_results.count(); j++)
		{
			if (comparison_results[i].comparison < comparison_results[j].comparison)
			{
				comparison_results.swap(i, j);
			}
		}
	}
	for (size_t i = 0; i < comparison_results.count(); i++)
	{
		result.add(this_messages[comparison_results[i].index]);
	}
	delete word;
	return result;
}

std::ostream & operator<<(std::ostream & os, const Document & obj)
{
	const Vector<Message> & msgs = obj.get_messages();
	for (size_t i = 0; i < msgs.count(); i++)
	{
		os << msgs[i] << '\n';
	}
	return os;
}
