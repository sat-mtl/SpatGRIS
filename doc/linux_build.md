# Compile SpatGRIS on Linux (Ubuntu 22.04):

- Dependencies
    - Install all dependencies (tested on Ubuntu 22.04):
      ```bash
      sudo apt install -y juce-tools ladspa-sdk freeglut3-dev libasound2-dev libcurl4-openssl-dev libfreetype6-dev libx11-dev libxcomposite-dev libxcursor-dev libxinerama-dev libxrandr-dev mesa-common-dev libjack-dev build-essential gnome-devel
      ```
- Prepare SpeakerView
    - Download and install [Godot](https://github.com/godotengine/godot/releases/) version 4 or above.
    - Navigate to the folder you want to keep your cloned repositories and execute the following commands:
      ```bash
      git clone git@github.com:GRIS-UdeM/SpeakerView.git
      ```
    - Open the Godot project located in the **SpeakerView** folder:
      1. From the Godot project list, import project.godot of the SpeakerView folder.
      2. Select Edit the SpeakerView project.
      3. Export the project to the platform of your choice.
         1. It might be required to download the proper Export Template. The **Manage Export Templates** tool inside Godot can be used.
         2. Linux users might need to export twice: PCK and the binary. The pck file must be named **SpeakerView.pck** and the binary **SpeakerView.x86_64**.
- Compile and install SpatGRIS
    - Navigate to the folder you want to keep your cloned repositories and execute the following commands:
      ```bash
      git clone https://github.com/GRIS-UdeM/SpatGRIS.git
      cd SpatGRIS
      Projucer --resave SpatGRIS.jucer
      cd Builds/LinuxMakefile/
      make CONFIG=Release
      ```
    - Alternatively, you can set the number of assigned cores to compile it faster, e.g., `make CONFIG=Release -j 8`
    - Move the Godot exported files to the same folder as the SpatGris executable.
- Run: `cd ~/Documents/sources/SpatGRIS && ./Builds/LinuxMakefile/build/SpatGRIS & disown`
