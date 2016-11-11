# Hzeller Matrix

A module for generating animations on a Raspberry PI.
Before you install this, please read this https://github.com/hzeller/rpi-rgb-led-matrix.

An example of using this module is available here https://github.com/meg768/hzeller-matrix-example.


## Installation
	$ npm install hzeller-matrix --save

Before you install, make sure you have GraphicsMagick installed on the Pi, and some other stuff.

	$ sudo apt-get update && sudo apt-get dist-upgrade && sudo apt-get install libwebp-dev
	$ sudo aptitude update && sudo aptitude install libgraphicsmagick++1-dev

## Usage

	var Matrix = require('hzeller-matrix');
	var matrix = new Matrix({width:32, height:32});

	matrix.runText('Hello World');

## Note (2016-07-28)

This has been working fine on a Raspberry Pi 2. But I have not been able to
get a stable image/animation using a Raspberry Pi 3. It might be my long cables.
Just a heads up.

## Constructor

### new Matrix(config)

Constructs a new matrix object. The **config** argument must contain the following values.

- **width** - Specifies the width of the display.
- **height** - Specifies the height of the display.

If **config** is not specified all methods will only log and no hardware is accessed.

Example

	var Matrix = require('hzeller-matrix');
	var matrix = new Matrix({width:32, height:32})
	...

## Methods

### matrix.runText(text, options, callback)

This method displays scrolling text.
The **callback** argument is called when the animation completes.
The **options** argument may have the following values.

- **textColor** - Changes text color. Default is red.
- **fontName** - Use the specified TrueType font.
- **fontSize** - Change the font size.
- **duration** - Run  no longer this number of seconds. Default is -1, that is until the text is fully displayed.
- **delay** - Scroll delay in milliseconds.
- **speed** - Speed factor. Default 1.0. Lesser value slows down the animation. Greater value speeds it up.
- **iterations** - Number of iterations to display the text. Default is 1. -1 == infinity.

Example

	matrix.runText('Hello World', {textColor: 'blue', fontSize:30});


### matrix.runImage(image, options, callback)

Display an image. The image parameter may be a string representing the file
name or a JavaScript **Buffer** object.
The **callback** argument is called when the animation completes.
The **options** argument may have the following values.

- **scroll** - Specifies the direction to scroll the image. Valid values are 'left', 'right', 'up', 'down' or 'auto'. Default is 'auto'.
- **pause** - Specifies the time, in seconds, to pause after scroll has finished.
- **duration** - Number of seconds to animate. Default is 60 seconds.
- **delay** - Animation delay during display loop in milliseconds.
- **iterations** - Number of iterations to display the image. -1 == infinity.
- **speed** - Speed factor. Default 1.0. Lesser value slows down the animation. Greater value speeds it up.

### matrix.runAmination(gifFile, options, callback)

Runs an animated GIF, frame by frame.
The **callback** argument is called when the animation completes.
The **options** argument may have the following values.

- **duration** - Number of seconds to animate. Default is 60 seconds.
- **delay** - Animation frame delay i milliseconds if the value is missing in the GIF.
- **speed** - Speed factor. Default 1.0. Lesser value slows down the animation. Greater value speeds it up.
- **iterations** - Number of iterations to animate. Default is -1, that is infinite.

### matrix.runRain(options, callback)

Displays a Matrix-like rain animation. The **options** are as follows.
The **callback** argument is called when the animation completes.
The **options** argument may have the following values.

- **hue** - Specifies the hue of the rain. If not specified, it will use the time of day to specify the hue.
- **duration** - Run the animation for **duration** seconds. -1 == inifinity.
- **delay** - Animation delay during display loop in milliseconds.
- **speed** - Speed factor. Default 1.0. Lesser value slows down the animation. Greater value speeds it up.


### matrix.runPerlin(options, callback)

Runs a perlin animation.
The **callback** argument is called when the animation completes.
The *options* argument may have the following values.

- **mode** - Valid modes are 1, 2 or 3.
- **duration** - Run the animation for **duration** seconds. -1 == inifinity.
- **delay** - Animation delay during display loop in milliseconds.
- **speed** - Speed factor. Default 1.0. Lesser value slows down the animation. Greater value speeds it up.

### matrix.isRunning()

Returns true/false if an animation is running or not.

## Properties

### matrix.width
Returns the matrix width.

### matrix.height
Returns the matrix height.

## See also

Check out https://github.com/meg768/hzeller-matrix-example for more information.


## Build Instructions

Instructions how to se up and compile **hzeller-matrix** as of 2016-07-27.

### Install Raspbian image.

Install image 2016-05-27-raspbian-jessie-lite.img. For the Mac, use **Pi Filler**.

### Find out the IP-address of the Raspberry

For the Mac, use Lan Scan or Lan Scan Pro. Google it.

### Connect to the Pi

Start a Terminal session and use **ssh** to connect. When prompted, the password is **raspberry**.

	$ ssh pi@10.0.1.73

### Expand File System

Once connected, start **raspi-config** and select **Expand File System**.
You may want to change the time zone as well.

	$ sudo raspi-config
	$ sudo reboot

### Update apt-get

	$ sudo apt-get update && sudo apt-get dist-upgrade

### Install node

	$ curl -sL https://deb.nodesource.com/setup_6.x | sudo -E bash -
	$ sudo apt-get install -y nodejs

### Install GraphicsMagick++

	$ sudo aptitude update && sudo aptitude install libgraphicsmagick++1-dev

### Install some more stuff
	$ sudo apt-get install git libwebp-dev

### Download hzeller-matrix

	$ git clone https://github.com/meg768/hzeller-matrix.git

### Build it
	$ cd hzeller-matrix
	$ npm install

### Configure Wi-Fi

Edit the configuration file.

	$ sudo nano /etc/wpa_supplicant/wpa_supplicant.conf

And add this at the end.

	network={
		ssid="my-network-name"
    	psk="my-password"
	}

Save and reboot. Connect again using the same ip-address as before and find out the
wlan ip-address by running the following.

	$ ifconfig

Now you can disconnect the network cable and log on to this new ip-address
in the future.
