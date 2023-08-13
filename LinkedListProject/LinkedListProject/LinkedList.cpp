#include "LinkedList.h"

lRep lRep::_sharedEmpty = {
	&lRep::_sharedEmpty, &lRep::_sharedEmpty, 0, 0
};
