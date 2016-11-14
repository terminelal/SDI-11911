# SDI-11911 Branch jmurolopez
Repositorio del Curso de Robótica SDI 11911 y SDI 31911 (Otoño 2016)

#Proyecto 1

Abra el folder Tarea_1/src/agitr/
Ahí se encuentran los tres archivos necesarios del paquete dentro del workspace Tarea_1 para poder ejecutar el programa.
El programa move_fwd.cpp es el que debe de ejecutarse desde consola junto con el turtlesim_node y el roscore para poder ver la funcionalidad del programa.



<!---holi--->

#Proyecto 2

Clonar el repositorio que se encuentra en el branch que se subió copiando lo siguiente 
en una terminal

	git clone (dirección del branch)
	git checkout PERSONAL_BRANCH

Desencriptar el archivo Proyecto2.zip que es donde se encuentran los archivos del proyecto

Dentro de la misma terminal ir al nuevo directorio y construit el workspace
	
	cd (dirección que tendrá)
	catkin_init_workspace
	cd .. 
	catkin_make

Verificar que no haya errores
En una nueva ventana o pestaña de la terminal copiar el modelo

	cd ~/.gazebo/models/
	mkdir EKBot
	cp PATH_TO_REPO/SDI-11911/Proyecto2/src/ekbot_description/model.* EKBot/(hay que cambiar esto)

Ahora se correrá GAZEBO con el modelo descagado

	source devel/setup.bash
	roscore
	catkin_make 
	roslaunch ekbot_gazebo ekbot.launch #Se abrirá GAZEBO con el carro en (0,0,0)

Correr cada uno de los siguientes comando en diferentes terminales empezando con source devel/setup.bash en cada una

	rostopic pub /target_position_topic geometry_msgs/Twist -- '[3,0,0]' '[0,0,0]' #Publica la posición deseada
	rosrun ekbot_ctrl robot_trajectory_node #nodo para calcular una trayectoria simple
	rosrun ekbot_ctrl tf2_broadcaster_node
	rosrun ekbot_ctrl vision_node
	rosrun ekbot_ctrl perception_node
	rosrun ekbot_ctrl robot_velocity_node #activa el nodo que convierte x,y,w de vel a motorvel y debe ser iniciado después de los demás nodos
	

