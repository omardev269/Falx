// BismIllahIRRahmaanIRRaheem
/* animator class decl */

#ifndef FLX_ANIMATOR_H
#define	FLX_ANIMATOR_H
#include <graphics/graphicsdef.h>
#include <graphics/scene.h>
namespace falx {
	class Animator {
		IGraphicsScene* i_GraphicsScene;
		ObjectID m_Object;
	public:
		bit Create(falx::IGraphicsScene* aiScene, const char* aFile);
		void Render(matrix4x4 aPV, largefloat aDelta);
		void Render(matrix4x4 aPV);// uses the time exposition for this
		FLX_INLINEFUNC void GetObjectID(ObjectID& oObject) { oObject = m_Object; }
		~Animator();
	};
}
#endif // !FLX_ANIMATOR_H