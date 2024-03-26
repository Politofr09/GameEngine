# GameEngine / GLion 
<img align="left" src="Logo.svg" alt="drawing" width="120px"/>

<br/>
My custom game engine written in C++ for my projects
<br/><br/><br/><br/><br/><br/><br/>

# Cloning this repo 
### Don't forget to `--recurse-submodules` to include all dependencies
```
git clone https://github.com/Politofr09/GameEngine.git --recurse-submodules
```

# Compiling 🛠️
## Windows 🪟 (Visual studio) 
1. Navigate to the `scripts` directory
2. Execute `Win-Setup.bat`
3. This will generate a **.sln** file and open directly Visual Studio.

```powershell
cd scripts
./Win-Setup.bat
```

## Linux 🐧
1. Navigate to the `scripts` directory
2. Run `chmod +x Linux-Setup.sh` to be able to run the build script
3. Execute `Linux-Setup.sh`
4. This will generate a **Makefile**
5. To compile everything, navigate back to the root of the repository and run `make` to compile the engine

```bash
cd scripts
chmod +x Linux-Setup.sh
./Linux-Setup.sh
cd ..
make
```

# Features 🏗️ 
- Works on Windows and Linux

# Dependencies 📦 
- [glfw3](https://github.com/Politofr09/glfw/)
