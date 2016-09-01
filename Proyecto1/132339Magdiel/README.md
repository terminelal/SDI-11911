# Proyecto 1: Generación de trayectorias en ROS

INSTITUTO TECNOLÓGICO AUTÓNOMO DE MÉXICO

Departamento Académico de Sistemas Digitales

Temas Selectos de Robótica: SDI-31911

Materia: Robótica: SDI-1191

Profesor: Dr. Marco Morales

Presenta: Job Magdiel González González

Otoño 2016

```
OBJETIVO

Aprender los fundamentos de ROS y aplicarlos para construir nodos con capacidad de
publicar y suscribirse a tópicos.
```

```
EXPERIMENTO

Se escribió un programa para generar trayectorias para las tortugas de Turtlesim. El
programa tiene las siguientes características:

1. Se ejecuta desde la terminal
2. Pide al usuario la posición final y el tiempo de ejecución deseados.
3. Lee la posicón actual de la tortuga que publica Turlesim.
4. Envia los mensajes de velocidad necesarios a una frecuencia de 10Hz para que la tortuga
siga una trayectoria de la posición actual a posición final en un tiempo de ejecución.
Durante la ejecución, se mantiene actualizada la posición actual con los datos publicados
por Turtlesim.
5. Regresar al paso 2 en forma iterativa.
```


En una consola escribe:
```
roscore                             		//ROS Master
```

En una segunda consola escribe:
```
rosrun turtlesim turtlesim_node     		//turtlesim

```

En una tercera consola:
```
cd catkin_ws/src		//Ve a la carpeta src de tu carpeta catkin_ws
catkin_create_pkg agitr		//Crea un package con el nombre 'agitr'
```

Copia los siguientes archivos en tu carpeta local catkin_ws/src/agitr
```
package.xml
CMakeLists.txt
magdiel.cpp
```

Regresa a la carpeta catkin_ws en la tercera consola y escribe:
```
source devel/setup.bash	
catkin_make
rosrun agitr magdiel		//Ejecuta programa
``` 

