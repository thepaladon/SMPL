# SMPL
 This project was created for the 3rd Block of my First Year at Breda Unviersity of Applied Sciences studying for a game development programmer:
 
 It consists of two solutions:
 
**SimplPi Solution** - this builds and compiles the project cross-platform on the Raspberry Pi 4 Model B.

**SimplWin Solution** - this builds and compiles the project on Windows x64. 

The only difference between those two solutions are the external libraries that they use. If you want to play it I recommend running it on Windows (it should be as simple as running the VS Debugger on Debug/Release on x64) because there is a lot of pre-work that has to be done to get it running on the Raspberry Pi. Also make sure to the setup.sh file **ON THE RASPBERRY PI** because it downloads all external dependencies. Then you have to link it through Cross-Platform under Tools > Options with SSH. 
