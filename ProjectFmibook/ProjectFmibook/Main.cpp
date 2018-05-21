#include <iostream>

#include "FmiBook.h"
#include "Main.h"

using namespace fmi;

using std::cout;
using std::endl;
using std::cin;
using std::cerr;

void Run(Fmibook & fmi)
{
	char actor[1024];
	char command[50];
	char subject[1024];
	
	cin >> actor;
	while (strcmp(actor, COMMAND_QUIT) != 0)
	{
		cin >> command;
		int commandIndex;
		for (commandIndex = 0; commandIndex < COMMANDS_COUNT; ++commandIndex)
		{
			if (strcmp(command, COMMANDS[commandIndex]) == 0)
			{
				break;
			}
		}
		switch (commandIndex)
		{
		case 0: // add moderator
		{
			char nickname[100];
			unsigned short age;
			cin >> nickname;
			cin >> age;
			try
			{
				fmi.addModerator(actor, nickname, age);
			}
			catch (const std::exception& err)
			{
				cerr << err.what() << endl;
			}
			
			break;
		}
		case 1: // add_user
		{
			char nickname[100];
			unsigned short age;
			cin >> nickname;
			cin >> age;
			try
			{
				fmi.addUser(actor, nickname, age);
			}
			catch (const std::exception& err)
			{
				cerr << err.what() << endl;
			}
			break;
		}
		case 2: // remove_user
		{
			char nickname[100];
			cin >> nickname;
			try
			{
				fmi.removeUser(actor, nickname);
			}
			catch (const std::exception& err)
			{
				cerr << err.what() << endl;
			}
			break;
		}
		case 3: // block
		{
			char nick[100];
			cin >> nick;
			try
			{
				fmi.block(actor, nick);
			}
			catch (const std::exception& err)
			{
				cerr << err.what() << endl;
			}
			break;
		}
		case 4: // unblock
		{
			char who[100];
			try
			{
				fmi.unblock(actor, who);
			}
			catch (const std::exception& err)
			{
				cerr << err.what();
			}
			break;
		}
		case 5: // post
		{
			char posttype[10];
			cin >> posttype;
			char content[Post::MAX_CONTENT];
			cin >> content;
			if (strcmp(posttype, POST_TYPE_IMAGE) == 0)
			{
				fmi.postImage(actor, content);
			}
			else if (strcmp(posttype, POST_TYPE_LINK) == 0)
			{
				fmi.postLink(actor, content);
			}
			else if (strcmp(posttype, POST_TYPE_TEXT) == 0)
			{
				fmi.postText(actor, content);
			}
			else
			{
				cout << "No such type of post" << endl;
			}
			break;
		}
		case 6: // remove post
		{
			unsigned int id;
			cin >> id;
			try
			{
				fmi.removePost(actor, id);
			}
			catch (const std::exception& err) 
			{
				cerr << err.what() << endl;
			}
			break;
		}
		default:
		{
			cout << "No such command" << endl;
			break;
		}
		}
	}
}

int main()
{
	
	return 0;
}