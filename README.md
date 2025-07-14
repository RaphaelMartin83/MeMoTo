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

### Simple GUI comes with a few shortcuts to know:

- left click to select, right click to perform action
- Switch between diagrams with ctrl+tab
- Switch between tools with F1, F2 etc...
- Save with ctrl+s
- Save as with ctrl+shift+s
- load with ctrl+o
- undo with ctrl+z
- redo with ctrl+shift+z
- Copy/paste with ctrl+c and ctrl+v
- share with ctrl+space
- open new diagram with ctrl+n
- find into diagram with ctrl+f
- print to svg with ctrl+p

### MeMoTo can also be run with commands

- --help
- --version
- --output-svg (headless print svg from diagram and close)
- --default-diagram (since a class can have multiple diagram types, opens MeMoTo on the desired diagram)
- --focus-on (searchs for the first occurence of the given value and opens MeMoTo to quickly find something)

Note: The sharing feature works like when hosting a videogame. The person with de best connection should be the one hosting if possible. Everybody shall be under the same subnet (VPN works).

# How to add a diagram type

- See doc/MeMoTo.memoto
- You can use the QtCreator project file or use cmake directly
- This project uses Qt6 framework
- You need to implement I_DiagramContainer
- You need to be composed of a toolbox, itself implementing I_ToolBox
- Check SMDiagramScene and SMToolBoxScene for an example
- You can create your tools and items implementing I_Tool and the item type you want
- Plug your diagram into MainWindow::initDiagrams() method, you're good to go!

# How to build
- On windows, nothing exists yet, you need to install Qt and compile yourself, sorry!
- On linux, docker run -v my_memoto_repo:/var/mmt ghcr.io/raphaelmartin83/memoto_linux_x86_64
  - If you don't have access to the docker image, you can build ghcr.io/raphaelmartin83/qt_compilation_base from toolchain/Docker
  - And then, build ghcr.io/raphaelmartin83/memoto_linux_x86_64 from toolchains/linux/x86_64/Dockerfile

# How to install

- On windows, MeMoTo is static so you just download and run the executable file.
- On linux based distributions:
  - MeMoTo is an appimage so you just download and run the appimage.
  - You can also use [appman](https://github.com/ivan-hc/AppMan) and install memoto with command: appman -i memoto

# What is planed

- Usability: svg generation path asking

