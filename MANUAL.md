# HOBGOBLIN MANUAL
**!under construction!**  
consider the current state of this manual as an example for the pending full codebase release  
will be adjusted for readability in the future, updates will be added accordingly  
see **README.md** for demo usage  
- [Engine Layout](#engine-layout)
- [Engine API](#engine-api)
- [Meshpack Tool](#meshpack)
- [Game Layout](#game-layout)
- [Development Guide](#quick-dev-guide)
## ENGINE LAYOUT
[Home](#hobgoblin-manual)

    Hobgoblin/
        engine/
            app/
                ***src***
                    app
                    
                ***headers***
                    app
            core/
                ***src***
                    logger
                    timer

                ***headers***
                    logger
                    rar
                    timer
            dev/
                ***src***
                    cmd_pack
                    dev_console

                ***headers***
                    cmd_pack
                    dev_console
            ecs/
                ***src***
                    placeholder

                ***headers***
                    components
                    components_interaction
                    components_physics
                    components_player
                    entity
                    registry
            physics/
                ***src***
                    systems_physics

                ***headers***
                    camera_collision
                    components_collider
                    systems_physics
            platform/
                ***src***
                    window_glfw

                ***headers***
                    input
                    window
            render/
                ***src***
                    mesh
                    mesh_loader
                    renderer_d3d11
                    texture

                ***headers***
                    camera
                    camera_helpers
                    mesh
                    mesh_loader
                    meshbin
                    renderer
                    shader
                    texture
            scene/
                ***src***
                    prefab_loader
                    systems_interaction

                ***headers***
                    prefab_loader
                    systems_interaction
            extern/
                cgltf/
                    cgltf.h
                stb/
                    stb_image.h
        apps/
            ---> your game goes here for simplicity
            goblin_slayer/ (demo src+assets)
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
                example_albedo.png (include textures here during meshpack usage, see MESHPACK) 
            meshpack/
                ---> meshpack outputs binary files here
                example.meshbin
                example.meta.json (included but not needed depending on your game's infrastructure)
                example_albedo.png (can delete, output to verify texture used)
        demo/ (see README.md)
        licenses/
        README.md
        MANUAL.md
        CHANGELOG.md
        CMakeLists.txt
## ENGINE API
[Home](#hobgoblin-manual)



**App/Core** 
    
    init()                                      -> bool # logging, window, D3D11 device, subsystems
    run()                                       -> int # main loop (fixed tick + render)
    shutdown()                                  -> void

    set_fixed_timestep(hz)                      -> void          # e.g., 60
    time_delta()                                -> float         # last sim dt
**ECS**
    
    createEntity()                              -> EntityID
    destroyEntity(id)                           -> void

    addComponent<T>(id, T value)                -> T&
    hasComponent<T>(id)                         -> bool
    getComponent<T>(id)                         -> T&            # non-const ref
    removeComponent<T>(id)                      -> void

    view<Ts...>()                               -> iterable      # for-range over entities with Ts
    each<Ts...>(fn(entity, Ts&...))             -> void
**Components**
    
    Transform { position: vec3, rotation: vec3, scale: vec3 }
    Mesh { handle, bounds }
    Texture { srv }
    AABB { halfExtents: vec3, localCenter: vec3 } # set localCenter.y = halfExtents.y for characters
    RigidBody{ kinematic: bool, velocity: vec3 }
    Player { } # tag
    Selectable{ } # tag
**Renderer**
    
    createDevice(window)                         -> bool
    resize(width, height)                        -> void
    beginFrame(clearColor)                       -> void
    submit(entity)                               -> void # collect visible meshes
    endFrame()                                   -> void # present

    createTextureFromFile(path)                  -> Texture
    createCheckerboard(w, h, tile)               -> Texture
    createMeshFromMeshbin(path)                  -> Mesh

    #temporary primitives (engine-side helpers)
    createPlane(size, y)                         -> Mesh
    createCube()                                 -> Mesh
**Scene**
    
    loadPrefab(path)                            -> EntityID # builds an entity from JSON
    loadScene(path)                             -> bool # spawns all entities in scene json
    instantiatePrefab(path, overrides?)         -> EntityID # optional field overrides
**Physics**
    
    step(fixed_dt)                              -> void # integrate + collide ground/AABBs
    setGravity(vec3 g)                          -> void
    setKinematic(id, bool)                      -> void

    #queries (basic)
    collideAABB(a: AABB, b: AABB)               -> bool
**CameraRig**
    
    setMode(mode)                               -> void # "free" | "fps"
    getMode()                                   -> string
    setYawPitch(yaw, pitch)                     -> void
    basisFromYaw(yaw)                           -> {fwd: vec3, right: vec3} # yaw-only WASD basis
    snapToPlayer(id, eye_height)                -> void
**Input**
    
    poll()                                      -> void
    keyDown(code)                               -> bool
    mouseDelta()                                -> {dx: float, dy: float}
**DevConsole**
    
    show(bool)                                   -> void
    isShown()                                    -> bool
    registerCommand(name, fn(args)->string)      -> void
    execute(line)                                -> string
**Logging**
    
    info(fmt, ...)                               -> void
    warn(fmt, ...)                               -> void
    error(fmt, ...)                              -> void

**Types**
    
    EntityID                                    -> uint32/uint64
    vec3                                        -> glm::vec3
## MESHPACK
[Home](#hobgoblin-manual)
- you can create binary mesh files (**.meshbin**s) by using the meshpack tool and the **cmd_pack** and **dev_console** files (not yet added)  
- **.meshbin**s can be used for rendering in game runtime, such as with the hobgoblin engine  
- model input is **.glb** **AND** texture input **IF** your model is textured, this can be a **.png** file type for example
    - if so add the suffix "**_albedo**" to your texture file and the meshpack will automatically pull it in
    - this is shown by the included example files
- in blender you can export your model to glb/gltf by clicking **File->Export->glTF 2.0 (.glb/.gltf)** no extensions required as of 4.3.0
- in the demo **goblin_slayer.exe** you can use **"~"** to bring up dev_console and use meshpack from there
    - inputs from **root/assets/models/**, outputs to **root/assets/meshpack/**, can be changed in **cmd_pack**
- youll stil need to copy or move your files to the appropriate game directories after or otherwise link them properly, consider **/assets** a general workspace
## GAME LAYOUT
[Home](#hobgoblin-manual)

    apps/
        ---> your game goes here
        goblin_slayer/ (example demo)
            assets/
                meshes/
                    characters/
                        player.meshbin
                prefabs/
                    characters/
                        player_prefab.json
                    scenes/
                        scene_basic.json
                shaders/
                    basic_ps.hlsl
                    basic_vs.hlsl
                    hlsl_basic.hlsli
                    hlsl_basic.hpp
                textures/
                    characters/
                        player_albedo.png
                content.manifest.json
            logs/
            src/
                game/
                dev/
            goblin_slayer.exe (shortcut)
- example game layout, feel free to use as inspiration
- most game source code will not be released, but extensive documentation on goblin_slayer's infrasturcture will be provided in the future
    - some game code will be released for educational purposes
## QUICK DEV GUIDE
[Home](#hobgoblin-manual)
- empty stub for now, will include how to start your own game and use the engine api