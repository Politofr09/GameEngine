<img src="Proto_logo.svg" alt="drawing" width="600"/>

<h2 style="text-align: center;">My custom game engine written in C++ for my projects</h2>

# Getting started
 Don't forget to `--recurse-submodules` to include all dependencies
```
git clone https://github.com/Politofr09/GameEngine.git --recurse-submodules
```

# Compiling 
## Windows (Visual studio) 
1. Navigate to the `scripts` directory
2. Execute `Win-Setup.bat`
3. This will generate a **.sln** file and open directly Visual Studio.

```powershell
cd scripts
./Win-Setup.bat
```

## Linux 
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

# The engine
- Multiplatform: Works on Windows and Linux (not tested on other platforms) ✅
- 3D and 2D renderer, supports 3d models, text, and rects.

## Dependencies
- [glfw3](https://github.com/Politofr09/glfw/)
- [assimp](https://github.com/Politofr09/assimp/)
- [glm](https://github.com/g-truc/glm/tree/4137519418a933e5863eea7c3ac53890ae7faf9d)
- [GLEW](https://glew.sourceforge.net)
- [imgui](https://github.com/ocornut/imgui)
- [stb](https://github.com/nothings/stb)