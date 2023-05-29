some_variable = 3 ^ (5 * 2)

--A lua table
config = {
    title = "My Game Engine",
    fullscreen = false,
    resolution = {
        width = 800,
        height = 600
    },
    test_value = 200.303
}

entities = {
    tank = {
        transform = {
            position = { x = 300, y = 200},
            scale = { x = 1, y = 1},
            rotation = 0
        },

        box_collider = {

        },

        projectile_emitter = {
            angle = {x = 0, 180}
        }
    }
}

function factorial(n)
    if n == 1 then 
        return 1
    end

    return n * factorial(n - 1)
end

print("The cube of the number 3 is: " .. cube(3));