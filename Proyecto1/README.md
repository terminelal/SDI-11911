##README Proyecto1
Buenos días, este tutorial mostrará paso a paso cómo ejecutar el proyecto 1 de Robótica.
Primero que nada, debemos abrir dos terminales de Ubuntu para correr procesos necesarios para la ejecución del proyecto. En una ventana de la consola de debe correr el comando:
```
roscore
```
Este comando habilita el master de ROS.

En la otra terminal que abrimos, correremos el siguiente comando: 
```
rosrun turtleSim turtleSim_node
```

Este comando muestra el esapacio 2D en donde se moverá la tortuga. Ahora, estamos listos para la siguiente etapa de las instrucciones.


Más abajo, se listan los archivos incluidos y necesarios para correr el proyecto:
* script.sh
* script2.sh
* sub.cpp
* eTortuga.cpp
* CMakeLists.txt
* CMakeListsS.txt

Lo primero a realizar es pegar los archivos .sh en la carpeta de mayor jerarquía en Ubuntu.
Después es necesario abrir una terminal de Ubuntu. En esta terminal, debe correrse
el primer script con el comando:
```
./script
```

Este comando se encarga de crear los directorios y el workspace necesario para la ejecución del proyecto. Ahora, tenemos tres carpetas importantes:
* raiz
* src
* paquete
* 
Una vez que existan estas carpetas es necesario entrar hasta paquete con los siguientes comandos:
```
cd raiz/src/paquete
```
En esta carpeta pondremos los ejecutables para nuestro programa, los archivos con extensión .cpp. Esto puede ser logrado através de pegar los archivos en el directorio correspondiente o bien crearlos a mano para pegar el código con el comando:
```
nano
```

Ya que tenemos el contenido necesario en este archivo, salimos de él presionando Ctrl+X, seguido de Y (para guardar los cambios) y la indicación de el nombre del archivo (etortuga.cpp y sub.cpp en nuestro caso).
Una vez que tengamos listos ambos archivos con extensión .cpp es necesario modificar el archivo CMakeLists.txt presente dentro de la carpeta paquete, situado a un lado de los ejecutables. Dependiendo qué ejecutable sea el que se quiere correr se hacen cambios al CMakeLists.txt. En caso de que primero se corra el etortuga.cpp es necesario modificar el archivo CMakeLists.txt de la carpeta por el contenido de el CMakeLists.txt incluido en esta carpeta. En caso contrario, que el ejecutable a correr sea sub.cpp, es necesario modificar el archivo con los contenidos de CMakeListsS.txt. IMPORTANTE: La 'S' en el nombre de este archivo es empleada únicamente para diferenciar entre uno y otro, cuando el archivo dentro de la carpeta sea modificado, es imperativo que este permanezca con el nombre de CMakeLists.txt

Seguidamente, salimos a la carpeta de mayor jerarquía con el comando:
```
cd
```

Una vez aquí debemos correr el segundo archivo de tipo script con el comando:
```
./script2.sh
```
Este script compila y ejecuta el programa etortuga sobre turtleSim, permitiendo al usuario probar la funcionalidad requerida por el proyecto.

Para correr el programa de subscripción a la pose de la tortuga, es necesario abrir otra consola (para correr otro proceso), accesar hasta la carpeta paquete y modificar el CMakeLists.txt ahí presente con el contenido de el CMakeListsS.txt proporcionado en esta carpeta. Una vez realizado esto, es necesario salir a la carpeta raiz. Esto se logra con los siguientes comandos:
```
cd
cd raiz 
```

Una vez en este directorio procedemos a compilar y ejecutar el programa con los siguientes comandos:
```
catkin_make
source devel/setup.bash
rosrun paquete sub
```

Estos comandos ejecutarán el segundo ejecutable del proyecto, en una ventana aparte de la que se comunica con el usuario. 


#Using Git
Git usually comes built-in with the main OS's. 
If it is not install or a newer version is needed, it can be downloaded from https://git-scm.com/downloads

This guide was tested in Linux.

## Cloning the repository
1. Open Terminal
2. cd to where the repository will be downloaded
3. git clone https://github.com/EagleKnights/SDI-11911.git --single-branch
4. cd into the downloaded repository

## Branches
Git uses branches to keep different versions of the code in the same repository.
The main branch is call "master" and in this repository only administrator will be able to make changes to it. After making git clone, the main branch will be "master".
To make a new branch, be sure to be inside the repo and type: 
```
git branch NEW_BRANCH
```
This only creates tha branch, to switch into the new branch:
```
git checkoout NAME_OF_THE_BRANCH
```

In this new branch is where changes can be made and push to github.

## Add - Commit - Push
The basic workflow to save changes into git is:
```
git add FILES_TO_ADD
git commit -m "Usefull comments about this commit"
```
After executing git commit, the changes are store

NOTE: if the option -m is not use, git will open VIM to enter the comment. A VIM cheat-sheet can be found in http://vim.rtorr.com/ .

After executing commit, the changes are store locally. To store the changes to the repo at github, use:
```
git push origin BRANCH_TO_PUSH
```

## Pull
If there are new changes to the main github repo (called origin), use the following command (be sure to save your last changes):
```
git pull
```

## Other Stuff
* To print commit history
```
git log
```

* Graphically show the branches
```
git log --oneline --decorate --graph --all
```

* Manual of git
```
man git
```

* Official git web page: https://git-scm.com/

* Pro Git Book Available at: https://git-scm.com/book/en/v2


