#include "hex.h"
#include "transaction/esdt.h"

bool operator==(ESDTProperties const &lhs, ESDTProperties const &rhs)
{
    return (lhs.canFreeze                == rhs.canFreeze) &&
           (lhs.canWipe                  == rhs.canWipe) &&
           (lhs.canPause                 == rhs.canPause) &&
           (lhs.canChangeOwner           == rhs.canChangeOwner) &&
           (lhs.canUpgrade               == rhs.canUpgrade) &&
           (lhs.canAddSpecialRoles       == rhs.canAddSpecialRoles);
}

bool operator!=(ESDTProperties const &lhs, ESDTProperties const &rhs)
{
    return !(lhs == rhs);
}
