#include "Module.h"

// Constructor de la clase Module. Inicializa el m�dulo con el puntero a la aplicaci�n y un estado inicial habilitado/deshabilitado.
Module::Module(Application* parent, bool start_enabled) : App(parent)
{

}

// Destructor de la clase Module. Aqu� se pueden realizar tareas de limpieza espec�ficas si se necesitan.
Module::~Module()
{

}

// M�todo de inicializaci�n del m�dulo. Retorna true si la inicializaci�n fue exitosa.
bool Module::Init()
{
    return true; // Retorna true como valor predeterminado.
}

// M�todo que se llama al inicio del m�dulo. Se ejecuta una vez al habilitar el m�dulo.
bool Module::Start()
{
    return true; // Retorna true como valor predeterminado.
}

// M�todo que se llama antes del update principal de cada frame. Recibe el delta time (dt) como argumento.
update_status Module::PreUpdate(float dt)
{
    return UPDATE_CONTINUE; // Indica que la ejecuci�n del programa debe continuar.
}

// M�todo que se llama en el update principal de cada frame. Aqu� se procesan las tareas principales del m�dulo.
update_status Module::Update(float dt)
{
    return UPDATE_CONTINUE; // Indica que la ejecuci�n del programa debe continuar.
}

// M�todo que se llama despu�s del update principal de cada frame. �til para tareas de limpieza o post-procesamiento.
update_status Module::PostUpdate(float dt)
{
    return UPDATE_CONTINUE; // Indica que la ejecuci�n del programa debe continuar.
}

// M�todo de limpieza del m�dulo. Se llama antes de que el m�dulo sea destruido.
bool Module::CleanUp()
{
    return true; // Retorna true como valor predeterminado.
}

// M�todo que se llama cuando ocurre una colisi�n entre dos objetos. 
// Los par�metros son los cuerpos f�sicos involucrados en la colisi�n.
void Module::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
    // Este m�todo est� vac�o, se debe implementar seg�n las necesidades del m�dulo.
}