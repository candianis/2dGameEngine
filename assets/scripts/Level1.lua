Level = {
    assets = {
        [0] = 
        {type = "texture",  id = "tilemap-texture",  file = "./assets/tilemaps/jungle.map"},
        {type = "texture",  id = "chopper-texture",  file = "./assets/images/chopper-spritesheet.png"},
        {type = "texture",  id = "takeoff-texture",  file = "./assets/images/takeoff-base.png"},
        {type = "texture",  id = "tank-texture",     file = "./assets/images/tank-panther-right.png"},
        {type = "texture",  id = "truck-texture",    file = "./assets/images/truck-ford-right.png"},
        {type = "texture",  id = "radar-texture",    file = "./assets/images/radar.png"},
        {type = "texture",  id = "bullet-texture",   file = "./assets/images/bullet.png"},
        {type = "texture",  id = "tree-image",       file = "./assets/images/tree.png"},
    
        {type = "font",     id = "charriot-font",    file = "./assets/fonts/charriot.ttf",          font_size = 18},
        {type = "font",     id = "pixelFull-font",   file = "./assets/fonts/PixellettersFull.ttf",   font_size = 16}
    },
    
    -- Map Configuration
    tilemap = {
        map_file = "./assets/tilemaps/jungle.map",
        texture_asset_id = "tilemap-texture",
        num_rows = 20,
        num_cols = 25,
        tile_size = 32,
        tile_scale = 2.0
    },
    
    
    -- Entities Configuration
    entities = {
        [0] = 
        {
            --Player
            tag = "player",
            components = {
                transform = {
                    position = {x = 100.0, y = 80.0},
                    scale = {x = 1.0, y = 1.0},
                    rotation = 0.0 --Deg
                },
    
                rigidbody = {
                    velocity = {x = 0.0, y = 0.0}
                },
    
                sprite = {
                    texture_asset_id = "chopper-texture",
                    width = 32,
                    height = 32,
                    layer = "PLAYER" 
                },
    
                animation = {
                    num_frames = 2,
                    speed_rate = 10 --fps
                },
    
                box_collider = {
                    offset = {x = 0, y = 0},
                    width = 32, 
                    height = 25
                },
    
                keyboard_control = {
                    up_velocity =    {x =  0.0,  y = -80.0},
                    right_velocity = {x =  80.0, y =  0.0},
                    down_velocity =  {x =  0.0,  y =  80.0},
                    left_velocity =  {x = -80.0, y =  0.0}
                },
    
                projectile_emitter = {
                    projectile_velocity = {x = 150.0, y = 150.0},
                    projectile_duration = 1500,
                    repeat_frequency = 3000,
                    hit_percent_damage = 10,
                    friendly = true
                },
    
                health = {
                    health_percentage = 100
                },
    
                camera_follow = {
                    follow = true
                }
    
            }
        },
    
        {
            --Tank
            group = "enemies",
            components = {
                transform = {
                    position = {x = 600.0, y = 500.0},
                    scale = {x = 1.0, y = 1.0},
                    rotation = 0.0
                },
    
                rigidbody = {
                    velocity = {x = 0.0, y = 0.0}
                },
    
                sprite = {
                    texture_asset_id = "tank-texture",
                    width = 32, 
                    height = 32,
                    layer = "ENEMIES"
                },
    
                box_collider = {
                    offset = {x = 0, y = 0},
                    width = 32,
                    height = 32
                },
    
                projectile_emitter = {
                    projectile_velocity = {x = 100.0, y = 0.0},
                    projectile_duration = 5000,
                    repeat_frequency = 3000,
                    hit_percent_damage = 10,
                    friendly = false
                },

                health = {
                    health_percentage = 100
                }
            }
        },
    
        {
            --Truck
            group = "enemies",
            components = {
                transform = {
                    position = {x = 410.0, y = 730.0},
                    scale = {x = 1.0, y = 1.0},
                    rotation = 0.0
                },
    
                rigidbody = {
                    velocity = {x = 0.0, y = 0.0}
                },
    
                sprite = {
                    texture_asset_id = "truck-texture",
                    width = 32, 
                    height = 32,
                    layer = "ENEMIES"
                },
    
                box_collider = {
                    offset = {x = 0, y = 0},
                    width = 32,
                    height = 32
                },
    
                projectile_emitter = {
                    projectile_velocity = {x = 100.0, y = 0.0},
                    projectile_duration = 2000,
                    repeat_frequency = 5000,
                    hit_percent_damage = 10,
                    friendly = false
                },

                health = {
                    health_percentage = 100
                }
            }
        },
    
        {
            --Radar
            group = "UI",
            components = {
                transform = {
                    position = {x = 1125.0, y = 16.0},
                    scale = {x = 1.0, y = 1.0},
                    rotation = 0.0
                },
    
                rigidbody = {
                    velocity = {x = 0.0, y = 0.0}
                },
    
                sprite = {
                    texture_asset_id = "radar-texture",
                    width =  64, 
                    height = 64,
                    layer = "GUI",
                    isFixed = true
                },
    
                animation = {
                    num_frames = 8,
                    speed_rate = 7 --fps
                }
            }
        },
    
        {
            --Label for "Chopper 1.0"
            group = "UI",
            components = {
                text_label = {
                    position = {x = 560, y = 0},
                    text = "Chopper 1.0",
                    font = "charriot-font",
                    color = {r = 0, g = 255, b = 0},
                    isFixed = true
                }
            }
        },
    
        {
            --Tree A
            group = "obstacles",
            components = {
                transform = {
                    position = {x = 700.0, y = 500.0},
                    scale = {x = 1.0, y = 1.0},
                    rotation = 0.0
                },
    
                sprite = {
                    texture_asset_id = "tree-image",
                    width = 16, 
                    height = 32, 
                    layer = "OBSTACLES"
                },
    
                box_collider = {
                    offset = {x = 0, y = 0},
                    width =  16,
                    height = 32
                }
            }
        },
    
        {
            --Tree B
            group = "obstacles",
            components = {
                transform = {
                    position = {x = 500.0, y = 500.0},
                    scale = {x = 1.0, y = 0},
                    rotation = 0.0
                },
    
                sprite = {
                    texture_asset_id = "tree-image",
                    width = 16,
                    height = 16,
                    layer = "OBSTACLES"
                },
    
                box_collider = {
                    offset = {x = 0, y = 0},
                    width =  16,
                    height = 32
                }
            }
        }
    }
}