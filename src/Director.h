#ifndef DIRECTOR_H_
#define DIRECTOR_H_

#include "LuaMachine.h"

/// Simple game loop for Lua-scripted games.
///
/// Copyright 2011 Bifrost Games. All rights reserved.
/// \author Tommy Nguyen
class Director : public Touchable
{
public:
	//static Vector<Drawable *> drawables;  ///< Drawable objects

	Director();

	void draw();

	/// Load and initialise game script.
	void init(const char *const);

	/// Update world.
	/// \param dt  Time since last update
	void update(const float dt);

	/// Update screen resolution.
	void update_video(const int w = 0, const int h = 0);

#ifdef RAINBOW_BUTTONS

	/// Handle key press events.
	void key_press();

#endif

	/// Handle mouse/touch begin-events.
	void touch_began(const Touch *const touches, const unsigned int count);

	/// Handle mouse/touch cancel-events.
	void touch_canceled();

	/// Handle mouse/touch end-events.
	void touch_ended(const Touch *const touches, const unsigned int count);

	/// Handle mouse/touch move-events.
	void touch_moved(const Touch *const touches, const unsigned int count);

private:
	LuaMachine lua;

	/// Intentionally left undefined.
	Director(const Director &);

	/// Intentionally left undefined.
	Director& operator=(const Director &);
};

inline void Director::update_video(const int w, const int h)
{
	Screen::Instance().set(static_cast<float>(w), static_cast<float>(h));
	this->lua.platform.update(this->lua.L);
}

#ifdef RAINBOW_BUTTONS

inline void Director::key_press()
{
}

#endif

inline void Director::touch_began(const Touch *const touches, const unsigned int count)
{
	this->lua.input.touch_began(this->lua.L, touches, count);
}

inline void Director::touch_canceled()
{
	this->lua.input.touch_canceled(this->lua.L);
}

inline void Director::touch_ended(const Touch *const touches, const unsigned int count)
{
	this->lua.input.touch_ended(this->lua.L, touches, count);
}

inline void Director::touch_moved(const Touch *const touches, const unsigned int count)
{
	this->lua.input.touch_moved(this->lua.L, touches, count);
}

#endif
