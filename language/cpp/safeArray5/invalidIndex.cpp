#include "invalidIndex.h"

InvalidIndex::InvalidIndex(int index)
: index_(index)
{

}
	
int InvalidIndex::getInvalidIndex() const
{
	return index_;
}

