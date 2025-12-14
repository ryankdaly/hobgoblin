# HOBGOBLIN
**!under construction!**  
**!coming soon!**  
the full codebase release is pending certain feature deliverables being met and properly tested  
the only currently released tool is the meshpack tool  
which can be used to export your models from a blender file to a binary file for game runtime using the demo exe or your own build  
please see **MANUAL.md** for instructions on how to use the meshpack tool  
please see the [ROADMAP](#roadmap) for more info on future targets for full release, this will be updated accordingly  
major recent updates will be posted at the top here
thank you for your patience  
    
What is Hobgoblin?
- scalable, lightweight windows c++20 game engine
- minimal d3d11 renderer, custom ECS, prefab/scene loader, and physics lite systems
- designed for deterministic simulation and full source ownership
- minimal third party code dependencies
- everything is considered WIP
- Linux support is planned
- an educational tool with documentation for developers to learn how to create their own games and engines using C++  
    - will include full stack game development guides: from C++ programming and engine architecture to game asset creation including modeling, uv mapping, and animations  
## REQUIREMENTS
### BUILD ENVIRONMENT
- **OS:** Windows 10/11 (x64)
- **Compiler/IDE:** Visual Studio 2022 (MSVC v143)
- **CMake:** ≥ 3.26
- **Windows SDK:** ≥ 10.0.22000
- **Graphics:** Direct3D 11, Feature Level 11_0+
### LIBRARY DEPENDENCIES
- stb_image, cgltf, GLM, GLFW, spdlog, fmt
### DEMO REQUIREMENTS
- **OS:** Windows 10/11 (x64)
- **Graphics:** Direct3D 11, Feature Level 11_0+
## LAYOUT
    Hobgoblin/
        engine/
            render/
                src/
                    mesh.cpp

                include/
                    hobgoblin/
                        render/
                            mesh.hpp
                            meshbin.hpp
                CMakeLists.txt
            extern/
                cgltf/
                    cgltf.h
                stb/
                    stb_image.h
        apps/
            ---> your game goes here for simplicity
            goblin_kingdom/ (not given yet, shown for example)
        tools/
            meshpack/
                CMakeLists.txt
                meshpack_lib.cpp
                meshpack_lib.hpp
        assets/
            models/
                ---> put your blender model exports here
                example.blend (not needed)
                example.glb
                example_albedo.png (include textures here during meshpack usage, see MANUAL.md) 
            meshpack/
                ---> meshpack outputs binary files here
                example.meshbin
                example.meta.json
                example_albedo.png (can delete, output to verify texture used)
        demo/
        licenses/
        README.md
        MANUAL.md
        CHANGELOG.md
        CMakeLists.txt
- see **MANUAL.MD** for future full release layout    
## ENGINE API
see **MANUAL.md** for instructions on how to use the engine and create your own game with it
## MESHPACK TOOL
see **MANUAL.md** for instructions  
easy usage in demo
## DEMO
the game demo can be found in **/demo/apps/goblin_kingdom/goblin_kingdom.exe**  
**!note**: currently dev console is meant to be used mainly for the meshpack tool  
using in its current state will break your current game loop  
this will be updated and fixed when necessary for testing  
### CONTROLS
**FPS:**   
esc - exit (need to exit both game render and game terminal)  
wasd - movement  
h - health  
i - inventory  
j - journal  
left shift - sprint  
space - jump  
left click - interact/select (range limit)  
left alt - head tilt  
F1 - free cam mode  
~ - dev console  
**FREE CAM MODE:**   
left alt - move player  
left shift - enable multiple entity selection  
left click+drag - box select  
F3 - dev toggle (world coordinates, AABB visuals, hidden game stats)    
esc - exit (you need to exit both game render and game terminal)   
h - health  
i - inventory  
j - journal  
left click - interact/select  
F1 - FPS mode  
~ - dev console  
## CHANGELOG & FEATURES
- please see **CHANGELOG.md** for a full list of engine features and a deployment changelog
## CREDITS & USAGE
**see /licenses**      
this project would not be possible without them  

- [stb_image](https://github.com/nothings/stb) --- Sean Barrett
- [cgltf](https://github.com/jkuhlmann/cgltf) --- Johannes Kuhlmann
- [GLM](https://github.com/g-truc/glm) --- G-Truc
- [spdlog](https://github.com/gabime/spdlog) --- Gabi Melman
- [fmt](https://github.com/fmtlib/fmt) --- Victor Zverovich, and fmt contributors
- [GLFW](https://github.com/glfw/glfw) --- Camilla Löwy, Marcus Geelnard

**thank you**  
credit all contributors appropriately and include all licenses

this project is considered open source and free to use
## ROADMAP
This is an **active** roadmap  
changes and updates will be added accordingly as development allows
1. Performance/Critical
    - **1.1** central spatial index (uniform grid, central world query structure)
    - **1.2** render command buffer + sorting + instancing
    - **1.3** zero allocation hot paths (framearena + scratch)
    - 1.4 system cadence + per ticks budget (scalable to hardware, adaptive to frame rate/sim time with on/off) 
    - 1.5 physics query consolidation + ignore rules
    - 1.6 world coordinate/origin strategy (decide and apply across transform/physics)
    - 1.7 determinism guardrails (central RNG, unstable iteration dependeancies, basic state-hash function scaffold)
    - 1.8 minimal in engine performance HUD/profiling hooks (frame timing, entity/component counts, per system timing, queue sizes)
2. ECS<->UI
    - 2.1 right click context menu
    - 2.2 clickable UI (inventory + health)
    - 2.3 drop/pickup/transfer item actions
3. World
    - 3.1 worldseeding
        - true random world seeding using our state of the art algorithm and entropy source, meaning everytime you start a new game you will get an entirely new world, including the way the world evolves
    - 3.2 chunking + world limits
    - 3.3 culling, simulation level LOD
4. Chat System
    - GUI button or "t" key to talk, separate receivier and transmission ranges
5. Skeletons + Animation
    - basic animations: walk, sprint, idle, crouch (with idle/movement), prone (with idle/movement), basic combat swing
    - crouching and proning affect transmission range for chatting 
6. Appearance System
    - system to keep track of separate visual components for a single entity
    - example: body mesh, hair mesh, armor meshes, etc.
7. NPCs, AI, Pathing, & Jobs
    - infrastructure largely relies on experimentation results once above steps are done
8. Animations + Combat
    - AABB collider bodymap? relies on step 5's completion
9. File IO saving/loading, Menus, & Configs
10. Future Gameplay
    - we have many more features planned that we are not quite ready to reveal, so stay tuned!
    - once these first 9 steps are finished and the engine has passed testing, this roadmap will be updated with those features
    - consider the first 9 steps of this roadmap
    - most if not all gameplay features will be included in hobgoblin as part of the open source project
    - these features include networking/**multiplayer**

## THANK YOU

