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
- scalable, lightweight windows C++20 game engine
- minimal d3d11 renderer, custom ECS, prefab/scene loader, and physics lite systems
- designed for deterministic simulation and full source ownership
- minimal third party code dependencies
- everything is considered WIP
- Linux support is planned
- an educational tool with documentation for developers to learn how to create their own games and engines using C++  
    - will include full stack game development guides: from C++ programming and engine architecture to game asset creation including modeling, uv mapping, and animations
- note that the hobgoblin engine is specifically designed for the game goblin_kingdom to speedup development time but the open source engine and instructional docs are intended to provide the tools for other developers to modify and use engine as they see fit for their game without a paywall
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
            goblin_kingdom/ (not given, show for example)
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
**demo will be deployed soon**  
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
left click - interact/select (range limit, must be close to entity)  
left alt - head tilt  
F1 - free cam mode  
~ - dev console  
**FREE CAM MODE:**   
left alt - move player entity  
left click - interact/select (no limit)  
left shift+click - select multiple entities  
left click+drag - box select  
F3 - debug toggle (world coordinates, AABB visuals, hidden game stats)    
esc - exit (you need to exit both game render and game terminal)   
h - health  
i - inventory  
j - journal  
F1 - FPS mode  
~ - dev console  
## CHANGELOG & FEATURES
- please see **CHANGELOG.md** for a full list of engine features and a deployment changelog
## CREDITS & USAGE
see **/licenses**      
this project would not be possible without them  

- [stb_image](https://github.com/nothings/stb) --- Sean Barrett
- [cgltf](https://github.com/jkuhlmann/cgltf) --- Johannes Kuhlmann
- [GLM](https://github.com/g-truc/glm) --- G-Truc
- [spdlog](https://github.com/gabime/spdlog) --- Gabi Melman
- [fmt](https://github.com/fmtlib/fmt) --- Victor Zverovich, and fmt contributors
- [GLFW](https://github.com/glfw/glfw) --- Camilla Löwy, Marcus Geelnard

**thank you**  
credit all contributors appropriately and include all licenses

all copyrights and trademarks retained by BrainJuice LLC  
the hobgoblin engine is considered open source and free to modify and use for your own projects as long all credits are correct  
shipped with an MIT license, see **hb_LICENSE.txt** in **/licenses**  
## ROADMAP
This is an **active** roadmap  
changes and updates will be added accordingly as development allows  
demo deployment is almost there, just cleaning up the game directory and a couple small gameplay bugfixes and most importantly choosing deployment host while simultaneously working on Phase 1  
1. Performance/Critical
    - Done. ~~1.1 central spatial index (uniform grid, central world query structure)~~
    - 1.2 render command buffer + sorting + instancing
    - 1.3 zero allocation hot paths (framearena + scratch)
    - 1.4 system cadence + per ticks budget (scalable to hardware, adaptive to frame rate/sim time with on/off) 
    - 1.5 physics query consolidation + ignore rules
    - 1.6 world coordinate/origin strategy (decide and apply across transform/physics)
    - 1.7 determinism guardrails (central RNG, unstable iteration dependeancies, basic state-hash function scaffold)
    - 1.8 minimal in engine performance HUD/profiling hooks (frame timing, entity/component counts, per system timing, queue sizes)
2. ECS<->UIX
    - 2.1 pickup/drop action (world<->player hands)
    - 2.2 right click context menu (same ranges as current selection ranges for FPS/Free Cam modes)
    - 2.3 clickable GUI
        - 2.3.1 inventory: grid system per equipment w/ inventory piece (backpack, bags, jacket, cargo pants, etc.),
        - 2.3.2 health
        - 2.3.3 journal (pages for factions, family, skills, and stubs for individual encyclopedia and notes pages)
        - 2.3.4  fps hud
    - 2.4 item drag in inventory (such as hands<->backpack or backpack<->jacket or just rearranging a container)
    - 2.5 loading/progress bar for pickup/drop, item drag and other future actions
    - 2.6 inspectable (right click context menu)  
3. World
    - 3.1 seeding
        - true random world seeding using our state of the art algorithm and entropy source, meaning everytime you start a new game you will get an entirely new world, including the way the world evolves
    - 3.2 chunking grid + coordinate system
    - 3.3 chunk generation
    - 3.4 chunk streaming/culling
    - 3.5 end wall
    - 3.3 simulation level LOD
4. Chat System
    - GUI button or "t" key to talk, separate receivier and transmission ranges
    - chats show above a speakers head for a brief period of time
    - chat log tab on GUI button press only
5. Skeletons + Animation
    - basic animations: walk, sprint, idle, crouch (with idle/movement), prone (with idle/movement), basic combat swing
    - crouching and proning affect transmission range for chatting 
6. Appearance System
    - system to keep track of separate visual components for a single entity
    - example: body mesh, hair mesh, armor meshes, etc.
7. NPCs, AI, Pathing, & Jobs
    - infrastructure largely relies on experimentation results once above steps are done, simple optimized and scalable are the primary goals
8. Animations + Combat
    - simple AABB collider bodymap and weapon/equipment colliders with appropriate animations potentially? relies on step 5's completion
    - reactions (such as general hit, stun, etc.) & stamina drains
9. File IO saving/loading, Configs, & Menus
10. Future Gameplay
    - we have many more features planned that we are not quite ready to reveal, so stay tuned!
        - including a small LLM integration
    - once these first 9 steps are finished and the engine has passed testing, this roadmap will be updated with those features and the engine source code will likely be fully released
    - most if not all gameplay features will be included in hobgoblin as part of the open source project
        - these features include networking/**multiplayer**

## THANK YOU












