# MeMoTo

MeMoTo for MErgeable MOdeling TOol is yet another modeling tool focused on DevOps issues.

![Class diagram sample](doc/ClassDiagramSample.png)

Please note that this program is in it's early development stages and has flows and issues. My opinion is that it's already usable in production by a dev team but the team has to be trained.

## Principles

- Text file based, readable and writable from text editors to solve conflicts and understand diffs in a pull request.
- Simple GUI: I didn't want one of those modeling tools leaving you a quarter of your screen to see what you draw.
- Pluggable: Json files are saved, read it with a code generator? Easy.
- LightWeight: C++ over Qt for cross platform code
- No project file: Link between diagrams are made with environment variables, no project file to easily integrate to CI/CD

## Use case

Let's assume you are developing a critical/complex system and need code to be generated from design. Design should not be different than code and should be manageable through branches and pull requests.

## How to use it

Simple GUI comes with a few shortcuts to know:

- left click to select, right click to perform action
- Switch between diagrams with alt+1, alt+2 etc...
- Switch between tools with 1, 2 etc...
- Save with ctrl+s
- Save as with ctrl+shift+s
- load with ctrl+o
- undo with ctrl+z
- redo with ctrl+y
- share with ctrl+space
- find into diagram with ctrl+f
- print to png with ctrl+p

Note: The sharing feature is a bit wierd. It relies on a common network repository and uses a sqlite file. Choose a network volume to put the sharing file on it and give the full network path to your coworkers.

# How to add a diagram type

- See doc/diagram_hierarchy.memoto
- You can use the QtCreator project file or use cmake by hand
- This project uses Qt6 framework
- You need to implement I_DiagramContainer
- You need to be composed of a toolbox, itself implementing I_ToolBox
- Check SMDiagramScene and SMToolBoxScene for an example
- You can create your tools and items implementing I_Tool and the item type you want
- Plug your diagram into MainWindow::initDiagrams() method, you're good to go!

# What is planed

- Improve usability: Multiple selection, Copy/paste, hide class content
- Beauty: curved connections, rounded items
- CD integration: Headless png generation
