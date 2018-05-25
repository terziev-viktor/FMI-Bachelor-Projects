#include <iostream>
#include "FmiBook.h"

using namespace fmi;

using std::cout;
using std::endl;
using std::cin;
using std::cerr;

void Run(Fmibook & fmi)
{
	char actor[User::MAX_NICKNAME_LENGTH];
	char command[50];

	cout << "Actor:";
	cin >> actor;
	bool isRunning = strcmp(actor, COMMAND_QUIT) != 0;

	while (isRunning)
	{
		cout << "Command:";
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
			cout << "New moderatos's nickname:";
			cin >> nickname;
			cout << nickname << "'s age:";
			cin >> age;
			try
			{
				fmi.addModerator(actor, nickname, age);
				cout << actor << " added moderator " << nickname << endl;
			}
			catch (const char * err)
			{
				cerr << err << endl;
			}

			break;
		}
		case 1: // add_user
		{
			char nickname[100];
			unsigned short age;
			cout << "New user's nickname:";
			cin >> nickname;
			cout << nickname << "'s age:";
			cin >> age;
			try
			{
				fmi.addUser(actor, nickname, age);
				cout << actor << " added user " << nickname << endl;
			}
			catch (const char * err)
			{
				cerr << err << endl;
			}
			break;
		}
		case 2: // remove_user
		{
			char nickname[100];
			cout << "Nickname of user to remove:";
			cin >> nickname;
			try
			{
				fmi.removeUser(actor, nickname);
				cout << actor << " removed " << nickname << endl;
			}
			catch (const char * err)
			{
				cerr << err << endl;
			}
			break;
		}
		case 3: // block
		{
			char nick[100];
			cout << "Nickname of user to block:";
			cin >> nick;
			try
			{
				fmi.block(actor, nick);
				cout << actor << " blocked " << nick << endl;
			}
			catch (const char * err)
			{
				cerr << err << endl;
			}
			break;
		}
		case 4: // unblock
		{
			char who[100];
			cout << " nickname of user to unblock:";
			cin >> who;
			try
			{
				fmi.unblock(actor, who);
				cout << actor << " unblocked " << who << endl;
			}
			catch (const char * err)
			{
				cerr << err << endl;
			}
			break;
		}
		case 5: // post
		{
			char posttype[10];
			cout << "Enter type of post:";
			cin >> posttype;

			try
			{
				if (strcmp(posttype, POST_TYPE_IMAGE) == 0)
				{
					char content[Post::MAX_CONTENT];
					cout << "Enter url of the image: ";
					cin >> content;
					fmi.postImage(actor, content);
					cout << actor << " posted an image" << endl;
				}
				else if (strcmp(posttype, POST_TYPE_LINK) == 0)
				{
					char content[Post::MAX_CONTENT];
					cout << "Enter the url of your link: ";
					cin >> content;
					char descr[LinkPost::MAX_DESCR_LEN];
					cout << "Enter a description (a sentance of maximum " << LinkPost::MAX_DESCR_LEN << " words):" << endl;
					cin.ignore();
					cin.getline(descr, LinkPost::MAX_DESCR_LEN);
					fmi.postLink(actor, content, descr);
					cout << actor << " posted a link" << endl;
				}
				else if (strcmp(posttype, POST_TYPE_TEXT) == 0)
				{
					char content[Post::MAX_CONTENT];
					cout << "Enter content of post (a sentance of maximum " << Post::MAX_CONTENT << " words):" << endl;
					cin.ignore();
					cin.getline(content, Post::MAX_CONTENT);
					fmi.postText(actor, content);
					cout << actor << " posted text" << endl;
				}
				else
				{
					cout << "No such type of post" << endl;
				}
			}
			catch (const char * err)
			{
				cerr << err << endl;
			}
			break;
		}
		case 6: // remove post
		{
			unsigned int id;
			cout << "Enter id of a post to remove:";
			cin >> id;
			try
			{
				fmi.removePost(actor, id);
				cout << actor << " removed post " << id << endl;
			}
			catch (const char * err)
			{
				cerr << err << endl;
			}
			break;
		}
		case 7: // view post
		{
			unsigned int id;
			cout << "Enter id of post to view:";
			cin >> id;
			try
			{
				fmi.viewPost(actor, id);
				cout << actor << " views post " << id << endl;
			}
			catch (const char * err)
			{
				cerr << err << endl;
			}
			break;
		}
		case 8: // view_all_posts
		{
			char ofwho[100];
			cout << "Enter nickname of user:";
			cin >> ofwho;
			try
			{
				fmi.viewAllPosts(actor, ofwho);
			}
			catch (const char * err)
			{
				cerr << err << endl;
			}

			break;
		}
		case 9:
		{
			char newNickname[User::MAX_NICKNAME_LENGTH];
			cout << "Enter your new nickname: ";
			cin >> newNickname;
			try
			{
				fmi.changeNickname(actor, newNickname);
				cout << actor << " changes his/her nickname to " << newNickname << endl;
			}
			catch (const char * err)
			{
				cerr << err << endl;
			}
			break;
		}
		case 10: // info
		{
			try
			{
				fmi.info();
				cout << actor << " views info" << endl;
			}
			catch (const char * err)
			{
				cerr << err << endl;
			}

			break;
		}
		default:
		{
			isRunning = strcmp(command, COMMAND_QUIT) != 0;
			if (!isRunning)
			{
				continue;
			}
			cout << "No such command" << endl;
			break;
		}
		}
		cout << "Actor:";
		cin >> actor;
		isRunning = strcmp(actor, COMMAND_QUIT) != 0;
	}
}

int main()
{
	Fmibook fmi("Admin", 22);
	Run(fmi);

	return 0;
}