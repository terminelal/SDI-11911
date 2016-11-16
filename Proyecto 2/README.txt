# Proyecto 2:  Control Cinematico de Robots Generacion de trayectorias en ROS

INSTITUTO TECNOL�GICO AUT�NOMO DE M�XICO

Departamento Acad�mico de Sistemas Digitales

Temas Selectos de Rob�tica: SDI-31911

Materia: Rob�tica: SDI-1191

Profesor: Dr. Marco Morales

Presenta: 
* 132339 Job Magdiel Gonz�lez Gonz�lez
* 133447 Sebasti�n S�nchez Alcal�

Otono 2016

```
OBJETIVO

Aplicar la teor�a sobre generaci�n de trayectorias a un problema pr�ctico.
```

```
EXPERIMENTO

Este proyecto consiste en el modelado cinem'atico de un robot m�vil, determinaci�n de trayectorias en base a tareas y control del robot para el seguimiento de estas trayectorias. La ejecuci�n se realizar� en el simulador Gazebo. Para esto se proveen c�digo y modelos del robot en el repositorio que se utilizar� para arrancar el proyecto.
```

En una consola escribe:
```
roscore                             		//ROS Master
```

En una segunda consola escribe:
```

cd ~/Proyecto2								//Direcci�n en donde descargaste la carpeta
catkin_make    		
source devel/setup.bash
roslaunch autonomos_gazebo intersection.launch
```

En una tercera consola:
```
cd ~/Proyecto2								//Direcci�n en donde descargaste la carpeta
source devel/setup.bash
roslaunch autonomos_simulation ros_autonomos.launch
```
