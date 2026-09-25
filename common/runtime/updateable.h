// BismIllahIRRahmaanIRRaheem
/* updateable interface declaration*/
#ifndef FLX_UPDATEABLE_H
#define FLX_UPDATEABLE_H
#include <runtime/enginedef.h>
namespace falx {
	// An interface for objects that can be updated
	class IUpdateable {
	public:
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Dismiss() = 0;
	};
}
#endif // !FLX_UPDATEABLE_H