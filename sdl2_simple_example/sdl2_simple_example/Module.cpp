#include "Module.h"

// Constructor de la clase Module. Inicializa el módulo con el puntero a la aplicación y un estado inicial habilitado/deshabilitado.
Module::Module(Application* parent, bool start_enabled) : App(parent)
{

}

// Destructor de la clase Module. Aquí se pueden realizar tareas de limpieza específicas si se necesitan.
Module::~Module()
{

}

// Método de inicialización del módulo. Retorna true si la inicialización fue exitosa.
bool Module::Init()
{
    return true; // Retorna true como valor predeterminado.
}

// Método que se llama al inicio del módulo. Se ejecuta una vez al habilitar el módulo.
bool Module::Start()
{
    return true; // Retorna true como valor predeterminado.
}

// Método que se llama antes del update principal de cada frame. Recibe el delta time (dt) como argumento.
update_status Module::PreUpdate(float dt)
{
    return UPDATE_CONTINUE; // Indica que la ejecución del programa debe continuar.
}

// Método que se llama en el update principal de cada frame. Aquí se procesan las tareas principales del módulo.
update_status Module::Update(float dt)
{
    return UPDATE_CONTINUE; // Indica que la ejecución del programa debe continuar.
}

// Método que se llama después del update principal de cada frame. Útil para tareas de limpieza o post-procesamiento.
update_status Module::PostUpdate(float dt)
{
    return UPDATE_CONTINUE; // Indica que la ejecución del programa debe continuar.
}

// Método de limpieza del módulo. Se llama antes de que el módulo sea destruido.
bool Module::CleanUp()
{
    return true; // Retorna true como valor predeterminado.
}

// Método que se llama cuando ocurre una colisión entre dos objetos. 
// Los parámetros son los cuerpos físicos involucrados en la colisión.
void Module::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
    // Este método está vacío, se debe implementar según las necesidades del módulo.
}