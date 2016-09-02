#Instalar ROS en la máquina virtual
Git usually comes built-in with the main OS's. 
If it is not install or a newer version is needed, it can be downloaded from https://git-scm.com/downloads

This guide was tested in Linux.

## Crear Workspace
1. Abrir Terminal
2. Dentro de la terminal ingresar el código siguente:
```
$ source /opt/ros/indigo/setup.bash
$ mkdir -p ~/pose_use/src    ... crea la carpeta con el nombre src dentro de pose_use ...
$ cd ~/pose_use/src
~/pose_use/src$ catkin_init_workspace
$ cd ~/pose_use/   
$ catkin_make      ... se construyen y actualizan los archivos dentro del espacio de trabajo ...
$ source devel/setup.bash
```

## Crear el paquete dentro del workspace
Dentro de la terminal que ya abriste ingresa el siguiente codigo:

```
$ cd ~/pose_use/src    ...regresas a la carpeta src...
$ catkin_create_pkg turtle_program  geometry_msgs turtlesim roscp ...se crea el paquete con las dependencias que utilizarás...
$ cd ~/pose_use
$ catkin_make
. ~/catkin_ws/devel/setup.bash

```


## Crear el archivo .cpp que en este caso lo llamamos 
```
git add FILES_TO_ADD
git commit -m "Usefull comments about this commit"
```



