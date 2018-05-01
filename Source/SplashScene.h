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

	private:
		Transform* trans;
	};
}

#endif /* SplashScene_h */