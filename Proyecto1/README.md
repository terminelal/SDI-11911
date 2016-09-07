La forma de ejecutar el programa es la siguiente:

1) descargar la carpeta wsros, mediante git clone
2) entrar a la carpeta y ejecutar: catkin_make (es necesario tener instalado ROS Indigo)
3) ejecutar el comando: source devel/setup.bash
4) ejecutar: roscore
5) En una terminal independiente a la anterior ejecutar: rosrun turtlesim turtlesim_node
6) En una terminal independiente a la anterior ejecutar: rosrun agitr agitr_moveto _x_fin:=10 _y_fin:=10 _theta_fin:=0 _tiempo:=4

_x_fin y _y_fin son las coordenadas a las que se moverá la tortuga
_theta_fin es el ángulo en radianes que adoptará la tortuga en su pose final
_tiempo es el tiempo que se toma como base para calcular la velocidad de traslado