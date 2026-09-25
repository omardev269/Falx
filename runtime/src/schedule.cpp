// BismIllahIRRahmaanIRRaheem
/* runtime stuff manager */

#include <runtime/enginedef.h>
#include <timeint.h>

void FalxStart() {
	falx::InitializeTimeInt();
}
bool FalxUpdate() {
	// false if the engine should stop
	return true;
}
void FalxStop() {
	
}