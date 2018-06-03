# FMI-Bachelor-Projects
Repository for all projects during my bachelor's degree in FMI

## Markdown Parser
A simple program that can format plain text into markdown.
On the first line you pass a path to a .txt file and then you enter commands:
```
makeHeading <n> - makes line number <n> a heading
makeBold <n> <from> <to> - makes words on line <n> bold
makeItalic <n> <from> <to> - makes words on line <n> italic
makeCombine <n> <from> <to> - makes words on line <n> bold and italic
addLine <content> - adds a line with content <content> to the file
exit - saves the .txt file as .md file with the changes applied
```
## ProjectFmibook
This project shows OOP design patterns Multiple Inharitance and Singleton
The Singleton design pattern is interesting:
```
class Admin : public Moderator
{
public:
  static Admin & getInstance();
  // ...
private:
  Admin();
	~Admin();
	Admin(const Admin&);
	void operator=(const Admin&);
};
```
Here class Admin can have only one instance. The fist call to getInstancePointer() creates the instance of Admin and every other call returns a pointer to that same object. Constructor, Copy Constructor and operator= are private so the pattern can't be broken.
