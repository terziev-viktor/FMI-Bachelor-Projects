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
