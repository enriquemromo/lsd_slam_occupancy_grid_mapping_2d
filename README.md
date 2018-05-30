# lsd_slam_occupancy_grid_mapping_2d

Descripción
---------------

Este sistema se comunica y construye un mapa de ocupación 2D a partir de una nube de puntos proporcionada por el sistema LSD SLAM modificado ([Modified LSD SLAM](https://github.com/enriquemromo/modified_lsd_slam)).

Instalación 
============
Este sistema se probó en Ubuntu 14.04 y ROS Indigo.

Instalación de dependencias
----------------------------

```
sudo apt-get install navit-graphics-qt-qpainter
```



En tu ruta de paquete de ROS, clonar el siguiente repositorio:
--------------------------------------
```
git clone https://github.com/enriquemromo/lsd_slam_occupancy_grid_mapping_2d.git
```

Compilar el paquete:
-----------------------------

```
cd lsd_slam_occupancy_grid_mapping_2d
catkin_make
```
Agregar al .bashrc el sistem lsd_slam_occupancy_grid_mapping_2d.
----------------


```
echo "source /package_path/lsd_slam_occupancy_grid_mapping_2d/devel/setup.bash" >> ~/.bashrc

. .bashrc 
```

Ejecución
--------------------
```
rosrun lsd_slam_occupancy_grid_mapping_2d occupancy_grid_mapping_2d
```


Generar el mapa de ocupación pata Gazebo
--------------
En la ventana principal, presionar la tecla **g** para generar el mapa de ocupación al sistema Gazebo


