#include "..\Engine\SDLRenderer.h"
#include "..\Engine\ResourceManager.h"
#include "AnimtionClipEditor.h"


//The editor class will make high level decision regard to the individual editors
//It will basically decide what mode we're gonna run in, and we need to think about 
//Whether we're gonna keep references to the resource manager etc. inside of the editor
//And we'll refactor out the topbar from the animclip editor the bar, the zoom buttons, and the 
//save buttons will each still be contained in the editor the rest the anim clip editor 
// has to figure out

 enum class EditorState
{
	 AnimationClip, Map
};

class Editor
{
public:
	Editor(ResourceManager* _resman);
	~Editor();

	void KeyDown(unsigned int _key);
	void KeyUp(unsigned int _key);
	void MouseDown(unsigned int _key);
	void MouseUp(unsigned int _key);
	void MouseMove( int _x,  int _y);

	void Update(float _dt);
	void Render(SDLRenderer* _renderer);

private:

	void Init(ResourceManager* _resman);

	//Callback functions
	void SaveCallback();
	void LoadCallback();
	void ZoomInCallback();
	void ZoomOutCallback();

	AnimationClipEditor* m_AnimClipEditor;
	ResourceManager* m_ResourceManager;
	EditorState m_EditorState;

	//Todo add anim stuff

	float m_WindowWidth;
	float m_WindowHeight;


};

