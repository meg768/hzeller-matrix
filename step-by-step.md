# Build Instructions

Instructions how to se up and compile **hzeller-matrix** as of 2016-07-27.

## Install Raspbian image.

Install image 2016-05-27-raspbian-jessie-lite.img. For the Mac, use **Pi Filler**.

## Find out the IP-address of the Raspberry

For the Mac, use Lan Scan or Lan Scan Pro. Google it.

## Connect to the Pi

Start a Terminal session and use **ssh** to connect. When prompted, the password is **raspberry**.

	$ ssh pi@10.0.1.73

## Expand File System

Once connected, start **raspi-config** and select **Expand File System**.
You may want to change the time zone as well.

	$ sudo raspi-config
	$ sudo reboot

## Update apt-get

	$ sudo apt-get update && sudo apt-get dist-upgrade

## Install node

	$ curl -sL https://deb.nodesource.com/setup_6.x | sudo -E bash -
	$ sudo apt-get install -y nodejs

## Install GraphicsMagick and more

	$ sudo aptitude update && sudo aptitude install libgraphicsmagick++1-dev git libwebp-dev

## Download hzeller-matrix

	$ git clone https://github.com/meg768/hzeller-matrix.git

## Build it
	$ cd hzeller-matrix
	$ npm install
