ubahn = require("ubahn")

assert(ubahn.version == "0.1")

function Triggered(rising_edge)
    if rising_edge then
        print("Aaaah trains!")
    else
        print("Pfew no trains.")
    end
end

ubahn:addTrigger(-5, 2, 1, 0, 0, Triggered);
