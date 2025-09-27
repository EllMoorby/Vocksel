# Vocksel
Vocksel is a learning project exploring **OpenGL** and **modern C++20**.  
It is a small voxel-style engine prototype with custom rendering, terrain, and debugging tools.  

---

## Features
- Written in **C++20**
- Rendering with **OpenGL 4.3**
- Custom shader + texture management
- Entity & world system (chunks, density fields, marching cubes)
- Integration of **Dear ImGui** for debug UI
- Debugging support with **Tracy profiler**

---

## Dependencies
All dependencies are automatically fetched with [CMake FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html).  
No manual setup is required.  

- [OpenGL](https://www.opengl.org/) v4.3  
- [GLAD](https://github.com/Dav1dde/glad) v2.0.8  
- [GLM](https://github.com/g-truc/glm.git) v1.0.1  
- [GLFW](https://github.com/glfw/glfw.git) v3.3.8  
- [FastNoiseLite](https://github.com/Auburn/FastNoiseLite) v1.1.1  
- [STB Image](https://github.com/nothings/stb)  
- [Dear ImGui](https://github.com/ocornut/imgui) v1.92.0  

---

## Build Instructions (Windows, Visual Studio 2022)

### Requirements
- [CMake 3.31+](https://cmake.org/download/)
- [Visual Studio 2022](https://visualstudio.microsoft.com/) with **Desktop Development with C++** installed

### Build
```powershell
git clone https://github.com/EllMoorby/Vocksel
cd Vocksel
cmake -B build -S . -G "Visual Studio 17 2022"
cmake --build build --config Release
```

### Running

```powershell
# Navigate to the built executable
cd build/bin/Release

# Run the application
.\Vocksel.exe
```
---

## Screenshots

<p align="center">
  <img src="https://github.com/user-attachments/assets/623aee4d-6113-4d8d-883d-10caaa1ca988" width="400"/>
  <img src="https://github.com/user-attachments/assets/c53d7404-0c23-4aa0-8ffb-8bec95c1c2fe" width="400"/>
</p>


---

## Controls
- **WASD:** Move camera
- **Mouse:** Look around
- **Space:** Move Up
- **CTRL:** Move Down
- **R:** Regenerate World
- **Enter:** Unlock Mouse
- **ESC:** Exit

---
## Roadmap / Future Work
- Expanding entities
- Improve marching cubes performance
- World Texturing
- Physics

---

## License

This project is released under the MIT License. See [LICENSE](LICENSE.md) for details.
