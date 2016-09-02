En Github se localiza el zip "Proyecto1_000148748.zip". AL momento de descargarlo, extraer su carpeta en el directorio donde se desee.
Se extraera una carpeta llamada "catkin_ws" la cual contiene lo necesario para correr el programa.
Para correrlo se necesitan abrir 3 terminales de UBUNTU. 
Añadir en la primera el código : roscore 
Dicho código iniciará el master que permite correr el programa.
En la segunda terminal poner el código : rosrun turtlesim turtlesim_node 
Dicho código crea la instancia de turtlesim que ejecutará el programa.
En la última poner el siguiente código (esperando a que se ejecute después de cada renglón): 
cd ~/catkin_ws
catkin_make
source devel/setup.bash
cd
rosrun proyecto1 tortuga

Y posteriormente, se seguirán las indicaciones que aparecerán en la terminal. 
