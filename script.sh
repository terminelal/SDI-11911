#!/bin/bash
# -*- ENCODING: UTF-8 -*-
echo "Ejecutar este script crea todos los directorios necesarios para la ejecución del proyecto 1."
mkdir raiz
cd raiz
mkdir src
cd src
catkin_init_workspace
echo "Se creo el workspace para el proyecto."
cd
cd raiz
catkin_make
cd src
catkin_create_pkg paquete

exit
