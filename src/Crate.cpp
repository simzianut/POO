#include "Crate.h"

Pigeon* Crate::open()
{
    return new BabyPigeon();
}
