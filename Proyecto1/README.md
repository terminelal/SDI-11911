README DEL PROYECTO 1

Autor: Hector Hugo Huipet Hernandez


1. Los tres archivos del repositorio de github deben estar dentro de una carpeta de nombre proyecto1
2. Dicha carpeta debe ser colocada dentro de la carpeta src de un workspace de ROS
3. Posteriormente debe ser compilado el paquete como indica el tutorial de "a gentle introduction to ROS", desde la raiz ejecutar catkin_make y source devel/setup.bash 
4. Es necesario tener roscore y una instancia de turtlesim_node en ejecución
5. Para ejecutar el paquete se introduce en la terminal donde se ha hecho todo el procedimiento del punto 3 la siguiente linea: rosrun proyecto1 poseUsr
6. El programa se activa solicitando la pose final dada por (X, Y, ángulo) y el tiempo de ejecución deseado. Todas estas solicitudes se realizan de manera secuencial introduciendo valores decimales validos y presionando enter tras cada captura.
7. Es recomendable introducir tiempos de ejecución mayores a 6 segundos, la aplicación tiene una sección de advertencia que explica que el funcionamiento puede no ser adecuado con tiempos muy pequeños, sin embargo si se desea continuar se permite.
8. La aplicación realiza el movimiento de traslacion y la orientación del angulo y al terminar, apaga el nodo.
9. Puede volverse a ejecutar la aplicación sin necesidad de restaurar el simulador y volver al punto (6).
