Entity = {
    Properties = {
        Controllable = 1,
        HasPhysics = 0,
		IsStatic = 0,
    },
    
    Parameters = {
        move_speed = 15.0,
        rotate_speed = 5.0,
    },
    
    Camera = {
        offset = Vector3(10, 20.0, 45.0),
    },
    
    up_vector = Vector3(0.0, 1.0, 0.0),
    radian = Radian(0.0),
    orientation = Quaternion(Radian(0.0), Vector3(0.0, 1.0, 0.0)),
    position = Vector3(0.0, 0.0, 0.0),
    forward_vector = Vector3(0.0, 0.0, 1.0),
}

Entity.OnInit = function()
	Entity.orientation = Quaternion(Entity.radian, Entity.up_vector);
end

Entity.OnUpdate = function(dt)
    local deltaMoveVelocity = 0.0;
    local deltaRotationVelocity = 0.0;
    
    if (inputHandler:isCommandActive("TurnLeft")) then
        deltaRotationVelocity = deltaRotationVelocity + Entity.Parameters.rotate_speed;
    end
    if (inputHandler:isCommandActive("TurnRight")) then
        deltaRotationVelocity = deltaRotationVelocity - Entity.Parameters.rotate_speed;
    end
    if (inputHandler:isCommandActive("MoveForward")) then
        deltaMoveVelocity = deltaMoveVelocity + Entity.Parameters.move_speed;
    end
    if (inputHandler:isCommandActive("MoveBack")) then
        deltaMoveVelocity = deltaMoveVelocity - Entity.Parameters.move_speed;
    end
    
    deltaMoveVelocity = deltaMoveVelocity * dt;
    deltaRotationVelocity = deltaRotationVelocity * dt;
    
    Entity.radian = Entity.radian + Radian(deltaRotationVelocity);
    
    Entity.orientation:setOrientation(Entity.radian, Entity.up_vector);
    Entity.position = Entity.position + Entity.orientation * Entity.forward_vector * deltaMoveVelocity;
end

Entity.GetPosition = function()
    return Entity.position;
end

Entity.SetPosition = function(x, y, z)
    Entity.position.x = x;
    Entity.position.y = y;
    Entity.position.z = z;
end

Entity.GetCameraPosition = function()
    local pos = Vector3(0.0, 0.0, 0.0);
    pos.x = Entity.Camera.offset.x + Entity.position.x;
    pos.y = Entity.Camera.offset.y + Entity.position.y;
    pos.z = Entity.Camera.offset.z + Entity.position.z;
    return pos;
end

Entity.GetCameraOffset = function()
    return Entity.Camera.offset;
end

Entity.SetCameraOffset = function(v)
    Entity.Camera.offset.x = v.x;
    Entity.Camera.offset.y = v.y;
    Entity.Camera.offset.z = v.z;
end

Entity.GetOrientation = function()
    return Entity.orientation;
end

--[[Entity.SetNewOrientation = function(w, x, y, z)
    Entity.orientation = Quaternion(w, x, y, z);
end
--]]

Entity.SetNewOrientation = function(r)
	Entity.radian = Radian(0.0) + r; 
    Entity.orientation:setOrientation(Entity.radian, Entity.up_vector);
end
