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

value_to_factorial = 7

function factorial(n)
    local result = n

    for value = n - 1, 1, -1 do
        result = result * value
    end

    return result 
end

factorial_number = factorial(value_to_factorial);
print("The factorial number of " ..value_to_factorial .." is " ..factorial_number)