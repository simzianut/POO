#include "Crate.h"

Pigeon* Crate::open() const
{
    return new BabyPigeon();
}
