# Ejecutar Proyecto 1
Para ejecutar el proyecto existen dos opciones: descargar el proyecto comprimido y correrlo, o crear el proyecto y copiar el archivo .cpp al final para correrlo en base a ese archivo.

## A partir del proyecto comprimido
1. Descargar el proyecto .tar
2. Extraer el archivo en alguna carpeta conocida (se requiere conocer bien la ubicación para el paso siguiente)
3. Abrir tres terminales diferentes
4. En las tres correr los siguientes comandos
```
cd ~/pose_turtle
rosdep update
source /opt/ros/indigo/setup.bash
export | grep ROS
```
5. En una sola terminal (de ahora en adelante, la terminal principal) correr los siguientes comandos
```
. ~/proyect1_posetime/devel/setup.bash
source devel/setup.bash
catkin_make
roscore 
rosrun turtlesim turtlesim_node
rosrun tur_pt movtur
```
6. Correr en una terminal el siguiente comando
```
roscore 
```
7. Correr en otra terminal el siguiente comando
```
rosrun turtlesim turtlesim_node
```
8. Correr en la terminal principal el siguiente comando
```
rosrun tur_pt movtur
```
9. Seguir las instrucciones en la terminal
10. Al finalizar el movimiento se deben detener los procesos de las tres terminales usando Ctrl + C y si se quiere reiniciar el movimiento seguir los pasos del 6 al 8.

## A partir del archivo .cpp
1. Abrir la terminal
2. Correr los siguientes comandos
```
mkdir -p ~/pose_turtle/src
cd ~/pose_turtle/src
catkin_init_workspace
cd ..
catkin_make
source devel/setup.bash
cd ~/pose_turtle/src
catkin_create_pkg tur_pt turtlesim geometry_msgs roscpp
cd ..
catkin_make
. ~/pose_turtle/devel/setup.bash
```
3. En el explorador, buscar el proyecto creado y en la carpeta tur_pt copiar el archivo .cpp a un lado de los archivos package y CMakeLists
4. Abrir dos terminales nuevas
5. Correr los siguientes comandos de las terminales nuevas
```
cd ~/pose_turtle
rosdep update
source /opt/ros/indigo/setup.bash
export | grep ROS
```
6. Correr en una de las terminales nuevas el siguiente comando
```
roscore 
```
7. Correr en otra de las terminales nuevas el siguiente comando
```
rosrun turtlesim turtlesim_node
```
8. Correr en la terminal principal el siguiente comando
```
rosrun tur_pt movtur
```
9. Seguir las instrucciones en la terminal
10. Al finalizar el movimiento se deben detener los procesos de las tres terminales usando Ctrl + C y si se quiere reiniciar el movimiento seguir los pasos del 6 al 8.

