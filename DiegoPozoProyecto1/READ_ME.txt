READ ME:

Para ejecutar el pro correo grama “run_turtle_run”, siga los siguientes pasos:
1. Activar ROS master y mantener esa terminal abierta.
roscore
2. Abrir una nueva terminal y correr turtlesim
rosrun turtlesim turtlesim_node
3. Descargar la carpeta adjunta “DiegoPozo_ws”.
4. Abrir otra terminal y cambiar el directorio hacia el workspace DiegoPozo_ws
cd DiegoPozo_ws
5. Compilar el paquete.
catkin_make
source devel\setup.bash
6. Correr el programa
rosrun dp_pkg run_turtle_run
7. Seguir las instrucciones que se imprimen, dando los datos requeridos X, Y, el ángulo de la pose final (en grados) y el tiempo en el que se quiere que se lleve a cabo el trayecto (en segundos). Separar cada dato con un “enter”.
8. Al terminar un trayecto, el programa preguntará si se quiere ingresar una nueva dirección. Ingresar un 1 si la respuesta es sí, un 0 si la respuesta es no.
