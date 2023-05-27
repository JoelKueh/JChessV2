#include "Scene.h"

// DEBUG
#include <fstream>
extern std::ofstream debug_out;

Scene::Scene() = default;

Scene::~Scene()
{
  // DEBUG
  debug_out << "Destructing Base Scene" << std::endl;
}