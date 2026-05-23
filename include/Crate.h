#ifndef CRATE_H
#define CRATE_H

#include "Pigeon.h"

class Crate {
public:
    [[nodiscard]] Pigeon* open() const;
};

#endif
