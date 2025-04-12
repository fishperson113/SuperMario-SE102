#include "Koopas.h"
#include"KoopasController.h"
Koopas::Koopas()
{
	AddComponent<KoopasController>()->Awake();
}

Koopas::~Koopas()
{
}
