#include "cellindex.h"

namespace Pable {

QDebug operator<<(QDebug dbg, const CellIndex &index)
{
    dbg.nospace() << "(" << index.row << ", " << index.col << ")";
    return dbg.space();
}

} // namespace Pable
