local PlayerController = {
    name = "PlayerController",
    elapsed_time = 0,
    bias = 18,
    speed = 1.0
}

function PlayerController:onCreate()
end

function PlayerController:onDestroy()
end

function PlayerController:onUpdate(timestep)
    self.parent:GetRigidbody2D().linear_velocity = Vector2:new(0, 0)

    if Input.isKeyPressed(string.byte('W')) then
        self.bias = 6
        self.parent:GetRigidbody2D().linear_velocity.y = self.speed
        -- self.parent:GetTransform().translation.y = self.parent:GetTransform().translation.y + self.speed * timestep
    end
    if Input.isKeyPressed(string.byte('S')) then
        self.bias = 18
        self.parent:GetRigidbody2D().linear_velocity.y = -self.speed
        -- self.parent:GetTransform().translation.y = self.parent:GetTransform().translation.y - self.speed * timestep
    end
    if Input.isKeyPressed(string.byte('D')) then
        self.bias = 0
        self.parent:GetRigidbody2D().linear_velocity.x = self.speed
        -- self.parent:GetTransform().translation.x = self.parent:GetTransform().translation.x + self.speed * timestep
    end
    if Input.isKeyPressed(string.byte('A')) then
        self.bias = 12
        self.parent:GetRigidbody2D().linear_velocity.x = -self.speed
        -- self.parent:GetTransform().translation.x = self.parent:GetTransform().translation.x - self.speed * timestep
    end

    self.elapsed_time = self.elapsed_time + timestep
    if self.elapsed_time > 0.1 then
        if self.parent:GetRigidbody2D().linear_velocity.x == 0 and self.parent:GetRigidbody2D().linear_velocity.y == 0 then
            self.parent:GetTile().coord_x = self.bias
        else
            self.parent:GetTile().coord_x = ((self.parent:GetTile().coord_x + 1) % 6) + self.bias
        end
        self.elapsed_time = self.elapsed_time - 0.1
    end
end

function PlayerController:onCollisionEnter(entity)
    Debug.LogInfo(entity:GetName() .. "Enter")
end

function PlayerController:onCollisionExit(entity)
    Debug.LogInfo(entity:GetName() .. "Exit")
end

return PlayerController
