# VTK_Engine

## Running the Server
We need Python to run the server. To run it locally, open a terminal and navigate to the location of this `README.md`. Run the following commands:  
   
For windows (cmd):
```bash
cd Server_UI
python -m venv env
env\Scripts\activate.bat
pip install -r requirements.txt
python app.py
```
For windows (powershell):
```bash
cd Server_UI
python -m venv env
.\env\Scripts\Activate.ps1
pip install -r requirements.txt
python app.py
```
For macOS/Linux:
```bash
cd Server_UI
python -m venv env
source env/bin/activate
pip install -r requirements.txt
python app.py
```

## Running the Clients
Building the project for operating systems other than Windows remains complicated as of now since the presets specified in CMakePresets.json are windows specific. The following instructions are for Windows only. VTK is a necessary dependency. Please install it from https://vtk.org/Wiki/VTK/Configure_and_Build

First, make sure that the environment variable VTK_DIR correctly points to the VTK build location.

```bash
cmake --preset x64-release
cmake --build --preset x64-release
cd out/build/x64-release/VTK_Engine
./VTK_Engine.exe 800 800 400 400 0   0   0    0   2.5 3.0 72.0  0   1_1 ws://localhost:5000/connect 0
```

## Demo from the Immersive Facility Silo!
![WIN_20241209_15_34_20_Pro](https://github.com/user-attachments/assets/1aeba8e2-f553-4759-86c3-212ab56358a0)

