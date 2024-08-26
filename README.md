# Lucas' Robot Car Code

## Setting Up

### 1. Getting an IDE

First of all we're going to need a code editor, which will allow us to read and edit our code more easily and also run it from an easier to use interface.

I recommend VS Code, which you can [download here](https://code.visualstudio.com/download), because it's lightweight, free and extremely powerful. Follow the installation process provided with VS Code and let it run at the end of the installation.

### 2. Setting up Dependencies

There are a couple of other bits of software we'll also need to make this work, Python and Git. 

#### Python

Before we can install the extension we'll need Python itself. There is a very detailed tutorial [here](https://code.visualstudio.com/docs/python/python-tutorial#_prerequisites) but the quick version is:
- Go to [Python Downloads](https://www.python.org/downloads/)
- Download the installer
- Run the installer
- Select "Add python.exe to PATH" at the bottom of the page
- And then "Install Now"

Once that's finished, open the Extension view in VS Code (it's the 4 stacked boxes on the left-hand side of the window). At the top of the navigation pane (the next column in) you should see a search bar. Type "python" into there and the Python extension should be listed with a blue install button. Click install and let it do its thing.

#### Git

Similarly, got to [Git Downloads](https://git-scm.com/downloads) and download the installer. Run it and it will ask you about loads of options. As far as I can tell the defaults are all good so "Next, Next, ..." until it's installed. VS Code has support for Git already installed so no need to add an extension but you might need to restart VS Code for Git to be recognised.

### Getting the Code

Once you have these softwares installed we can grab the code from GitHub (a source code storage solution that relies on Git). From VS Code go to File -> Open Folder and open a folder where you'd like to store all your code, or create a new one.

Then go to View -> Terminal and you'll see a terminal pop up at the bottom of the screen. It might take a second or two to load but once it does it should have the name of the folder you have opened followed by a '>'. Paste the following line into the terminal and hit Enter:

```git clone https://github.com/sebandraos/lucas-robot-car.git```

This will start downloading all the code we need to start running the robot. Once it's done you should see "lucas-robot-car" appear in the navigation pane and we're ready to start.

Whilst we're in the terminal, you can also run:

```pip install ultralytics```

which will give us access to the AI models we'll use for computer vision later. This might take a few minutes.

## Quick Start

Within the "lucas-robot-car" folder you'll see a subfolder called Code and one called Python within that. This is where all our python code lives. All python files end in ".py" and the ones you'll find in there do the following:

1. BrainFunctions.py - contains all of our basic commands that we can send to the robot. _Library, not an executable_
2. GoUntil.py - uses the BrainFunctions to turn on all the motors at a fixed speed until the ultrasonic sensor reads below a given value.
3. ObstacleAvoider.py - uses the BrainFunctions to help the robot move around a space without crashing into any obstacles.
4. RobotEyes.py - uses the ultralytics library we installed earlier to identify people within the view of the robot. N.B. This was a first test and there is a _lot_ more that could go into this to make the robot do clever things with the camera. _Library, not an executable_
5. RobotTag.py - combines the RobotEyes and BrainFunctions to help the robot look for people and drive towards them.
6. SpeedTest.py - uses the BrainFunctions to test at which speed the motors stopped moving the robot. This type of file is useful to isolate problems and solve them individually, rather than within the context of a much larger, more complex system where multiple factors could be at play. Once you've proven that things work in isolation, then you can start combining them knowing that any issues come from the combination.

To start running these files, ensure the robot is powered on, its Cam/Upload switch is on Cam, and that your computer is connected to its WiFi network.

Then, when you open any of the files (except the libraries) you'll see a play button in the top right-hand corner of the code editor. Clicking that play button will run the python script in the terminal. Pressing ctrl+c / cmd+c will stop the program from running.

You can edit the Python files to make changes, copy and paste them to makes big changes to existing ones, or just create new Python files to create new behaviours.

## All the rest

The rest of the code is the drivers and base code that is running inside the robot to allow the Python libraries to talk to the robot. It is not quite as easy to read or deploy as the Python code, hence adding the Python layer on top but I can add documentation on how to make changes there, and redeploy, if need be.