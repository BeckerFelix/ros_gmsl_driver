#  ros_gmsl_driver
A ROS driver for GMSL cameras on [Drive PX platform](https://www.nvidia.com/en-us/self-driving-cars/drive-platform/)

## Performance
* JPEG images compression: tested with 7 cameras giving 30FPS with 4MB/s each camera of network bandwith usage. This is ideal for loging JPEGs on a separate computer on same network as Drive PX
* Raw images 1920x1208px: tested with 4 cameras giving ~15FPS on same Tegra as driver
* Raw images 1920x1208px rectified with [image_proc_tegra](https://github.com/DavidTorresOcana/image_proc_tegra) on same Tegra as driver: tested with 2 cameras giving ~15FPS on same Tegra as driver
* Raw images donwsampled at 800x503px: tested with 4 cameras giving ~30FPS on same Tegra as driver
* Raw images donwsampled at 800x503px rectified with [image_proc_tegra](https://github.com/DavidTorresOcana/image_proc_tegra) on same Tegra as driver: tested with 4 cameras giving ~30FPS on same Tegra as driver
* Raw images donwsampled at 800x503px rectified with [image_proc_tegra](https://github.com/DavidTorresOcana/image_proc_tegra) on same Tegra as driver: tested with 4 cameras giving ~20FPS and about 45MB/s per camera on different computer on same network as Drive PX

## Getting Started

### Requirements

* Ubuntu 16 LTS
* ROS kinetic
* OpenCV 3.4 for Opencv4CUDA branch

### Installing

```
sudo apt install ros-kinetic-camera-info-manager
cd ~catkin_ws/src/
git clone https://github.com/DavidTorresOcana/ros_gmsl_driver/
cd ..
catkin_make
```
##  gmsl_n_cameras package description

## gmsl_n_cameras_node
This node gets GMSL cameras frames and publishes them into ROS topics
### Publish
   	~image_raw (sensor_msgs/Image)
	~image_raw/compressed (sensor_msgs/CompressedImage)
	~camera_info (sensor_msgs/CameraInfo)
### Argumetns
	selector-mask: This allows you to connect any camera at any port. E.g. "0111" to connect cameras 0,1 and 2 of port 0
### Parameters
	~img_compressed (bool: default false) select to pusblish JPEG compressed images.
	~JPEG_quality (int: default 65) quality of JPEGs being produced.
	~img_raw (bool: default false) select to publish raw 1920x1208px RGBA images.
	~img_raw_downsample (bool: default false) select to publish raw downsampled images as specified in ~img_downsample_width and ~img_downsample_heigth. It is not recommended to publish raw and raw_donwsample at same time
	~img_downsample_width (int: default 1200) width of img_raw_downsample images.
	~img_downsample_height (int: default 800) height of img_raw_downsample images.

	~FPS (int: default 30) select how many FPS needs to be produced. It may not possible to achieve the required FPS in case of CPU shortage
	~do_rectify (bool: default false) publish the flag "do_rectify" on ~camera_info topic.
	~camera_type_names (string: default "") list of comma-separated strings, describing the type of cmera conected in eahc port. This parameters tells the driver the kind of camera you have connected in each port. Ports are considered to be connected in order.
			* fish_eye: Sekonix NA1962 190x150 deg FOV
			* onetwenty: Sekonix NA1262 120x73 deg FOV
			* sixty: Sekonix NA6062 60x38 deg FOV
For more explanation, please read the launchfiles
### Usage
Use launchfiles on ~launch folder to configure the node appropriately 
```
roslaunch gmsl_n_cameras gmsl_n_cameras.launch
```

## Image rectification
To rectify the camera feeds [image_proc](http://wiki.ros.org/image_proc) or [image_proc_tegra](https://github.com/DavidTorresOcana/image_pipeline/tree/indigo/image_proc_tegra) can be used. image_proc_tegra can be used in Tegra cores and needs OpenCV with CUDA support

```
roslaunch gmsl_n_cameras cameras_rectifier.launch
```
Or 
```
roslaunch gmsl_n_cameras cameras_rectifier_tegra.launch
```
