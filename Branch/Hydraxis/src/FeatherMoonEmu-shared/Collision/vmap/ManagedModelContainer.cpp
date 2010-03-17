/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "ManagedModelContainer.h"

namespace VMAP
{

    ManagedModelContainer::ManagedModelContainer(void) : ModelContainer()
    {
        refCount = 0;
    }

    ManagedModelContainer::~ManagedModelContainer(void)
    {
    }

}
