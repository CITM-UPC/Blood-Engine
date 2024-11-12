#pragma once

#include "Application.h"

class Application;
struct PhysBody3D;

class Module
{
public:
	Application* App;

	Module(Application* parent, bool start_enabled = true);
	virtual ~Module();

	virtual bool Init() = 0;                  
	virtual bool Start() = 0;                 
	virtual update_status PreUpdate(float dt) = 0;  
	virtual update_status Update(float dt) = 0;    
	virtual update_status PostUpdate(float dt) = 0;  
	virtual bool CleanUp() = 0;

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
};