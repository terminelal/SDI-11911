#!/bin/bash
# -*- ENCODING: UTF-8 -*-
echo "Ejecutar este script después de modificar el archivo CMakeLists.txt con los parámetros del proyecto1."
echo "Este script compila y ejecuta el proyecto."
cd 
cd espacio
catkin_make
source devel/setup.bash
rosrun proyecto ejecutable

