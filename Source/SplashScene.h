#ifndef SplashScene_h
#define SplashScene_h

#include "QuarkEngine.h"

namespace Quark {
	class SplashScene : public Scene {
	public:
		SplashScene();
		virtual ~SplashScene();

		virtual void OnAwake() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;

		//TranslateGizmo gizmo;

		Program program;
		VertexArray vao;
		Transform* trans;
		int vtxCount;
	};
}

#endif /* SplashScene_h */