# Proyecto 2:  Control Cinematico de Robots Generacion de trayectorias en ROS

INSTITUTO TECNOLÓGICO AUTÓNOMO DE MÉXICO

Departamento Académico de Sistemas Digitales

Temas Selectos de Robótica: SDI-31911

Materia: Robótica: SDI-1191

Profesor: Dr. Marco Morales

Presenta: 
* 132339 Job Magdiel González González
* 133447 Sebastián Sánchez Alcalá

Otono 2016

```
OBJETIVO

Aplicar la teoría sobre generación de trayectorias a un problema práctico.
```

```
EXPERIMENTO

Este proyecto consiste en el modelado cinem'atico de un robot móvil, determinación de trayectorias en base a tareas y control del robot para el seguimiento de estas trayectorias. La ejecución se realizará en el simulador Gazebo. Para esto se proveen código y modelos del robot en el repositorio que se utilizará para arrancar el proyecto.
```

En una consola escribe:
```
roscore                             		//ROS Master
```

En una segunda consola escribe:
```

cd ~/Proyecto2								//Dirección en donde descargaste la carpeta
catkin_make    		
source devel/setup.bash
roslaunch autonomos_gazebo intersection.launch
```

En una tercera consola:
```
cd ~/Proyecto2								//Dirección en donde descargaste la carpeta
source devel/setup.bash
roslaunch autonomos_simulation ros_autonomos.launch
```
