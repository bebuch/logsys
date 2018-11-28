#!/bin/bash

# exit script on any error
set -e
set -o xtrace

build=$(pwd)/..
cd $build

INSTALL_PATH=$build/tmp/usr

function github_clone {
	if [ -d $2 ]
	then
		cd $2
		git pull
	else
		git clone https://github.com/$1/$2.git
	fi
	cd $build
}

function install_via_cmake {
	mkdir -p $1/build
	cd $1/build
	cmake -DCMAKE_PREFIX_PATH=$INSTALL_PATH -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH ..
	make install
	cd $build
}


# update or clone dependent projects
github_clone bebuch io_tools
install_via_cmake io_tools
