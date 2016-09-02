Este archivo explica cómo correr el archivo ejecutable.cpp del proyecto de Diego Cifuentes Jiménez.

En el branch dcifuentesjimenez dentro del repositorio destinado para el proyecto se subieron los siguientes archivos:
	--CMakeLists(SOLOREFERENCIA1).txt
	--CMakeLists(SOLOREFERENCIA2).txt
	--ejcutable.cpp
	--scriptD1.sh
	--scriptD2.sh
	--sub.cpp

Para poder ejecutar el archivo ejecutable.cpp es necesario primero crear el workspace con catkin y ciertos directorios. Para esto es que sirve el archivo scriptD1.sh, pero no puede solamente crearse un archivo Bash, sino que se deben escribir las siguientes dos instrucciones en la consola:

	cd $HOME && touch scriptD1.sh && chmod +x scriptD1.sh
	cd $HOME && echo '#!/bin/bash' > scriptD1.sh && echo '# -*- ENCODING: UTF-8 -*-' >>scriptD1.sh

De igual manera para crear el scriptD2.sh se escriben las instrucciones análogas:

	cd $HOME && touch scriptD1.sh && chmod +x scriptD2.sh
	cd $HOME && echo '#!/bin/bash' > scriptD2.sh && echo '# -*- ENCODING: UTF-8 -*-' >>scriptD2.sh

Posteriormente, se deben editar ambos archivos (por ejemplo, a través del comando nano) para copiar el contenido de los archivos scriptD1.sh y scriptD2.sh en el repositorio de Github. Es necesario correr después el archivo scriptD1.sh de la siguiente manera:

	./scriptD1.sh

En necesario tener a partir de aquí, además de la consola que corre el proceso de roscore y de la consola que corre el nodo de turtlesim, dos consolas más: una para el ejecutable.cpp y una para el sub.cpp

En cada una de dichas consolas se deben editar los archivos CMakeLists.txt de cada uno de los archivos ejecutables, es decir, en la consola donde se correrá el sub.cpp modificar el archivo CMakeLists.txt (dentro del paquete llamado proyecto) para que quede igual que el archivo CMakeLists(SOLOREFERENCIA2).txt o copiarlo íntegramente. De igual manera, en la consola donde se correra el archivo ejecutable.cpp se debe modificar el archivo CMakeLists (dentro del paquete llamado proyecto) para que quede igual al archivo CMakeLists(SOLOREFERENCIA1).txt o copiarlo.

Finalmente, se corre el scriptD2.sh en ambas consolas:

	./scriptD2.sh

y se escribe respectivamente:
	
	rosrun proyecto sub

y en la otra consola:

	rosrun proyecto ejecutable

----------------------------------------------------------------------------------------------------
	