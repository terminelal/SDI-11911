#Este programa está desarrollado en ROS, en el que la tortuga del simulador hace una trayectoria en un determinado tiempo.

A continuación se describen los pasos a seguir:

##Instalar ROS en la máquina virtual
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

## Crear el archivo .cpp que en este caso lo llamamos metodo_move utilizando el siguiente código
```
sudo sh -c 'echo " main" > /metodo_move.cpp'
```
Copiar y pegarlo en la carpeta scr de tu paquete 
Aquí estará el método de la tortuga 
Agregar el CMakeLists.txt las siguientes lineas
```
add_executable(metodo_move metodo_move.cpp)
target_link_libraries(metodo_move ${catkin_LIBRARIES})
```

##Compilar el programa con la instrucción

```
$ catkin_make
```
##Ejecutar el programa
Abrir una terminal y ahí poner el codigo siguiente:
```
$ cd ~/pose_turtle
$ rosdep update
$ source /opt/ros/indigo/setup.bash
$ export | grep ROS
$ roscore
```

Abrir otra terminal y poner el siguiente código para cargar el simulador:
```
$ cd ~/pose_turtle    ...esta instrucción solo se utiliza si no estas dentro del archivo pose_turtle..
$ rosdep update
$ source /opt/ros/indigo/setup.bash
$ rosrun turtlesim turtlesim_node
```
En este punto aparecerá una tortuga en el centro de un mapa cartesiano

Abrir otra terminal donde correrar el metodo del archivo .cpp
Ingresar el siguiente código:

```
$ rosdep update
$ source /opt/ros/indigo/setup.bash
$ catkin_make
$ source devel/setup.bash
$ rosrun turtle_program metodo_move
```
En este punto es donde el usuaio ingresará los datos y observará lo siguiente en la terminal

```
ingrese x:
ingrese y:
ingrese grados:
ingrese tiempo:
```
Finalmente observará el movimiento de la tortuga de un punto inicial a un punto final en un tiempo dado y al llegar girará.

Nota: al finalizar utilizar Ctrl + C para detener el procedimiento.
