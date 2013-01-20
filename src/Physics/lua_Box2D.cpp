#ifdef USE_PHYSICS

#include <Box2D/Box2D.h>

#include "Common/Debug.h"
#include "Lua/LuaHelper.h"

namespace b2
{
	float ptm_ratio = 32.0f;
	int g_body_list = -1;     ///< List of Lua::Body objects for retrieval.
}

#include "Physics/lua_b2Draw.h"
#include "Physics/lua_b2Shape.h"
#include "Physics/lua_b2Fixture.h"
#include "Physics/lua_b2Body.h"
#include "Physics/lua_b2Contact.h"
#include "Physics/lua_b2World.h"

namespace b2
{
	namespace Lua
	{
		namespace
		{
			int set_ptm_ratio(lua_State *L)
			{
				LUA_ASSERT(lua_gettop(L) == 1, "b2.SetPTMRatio(r)");

				ptm_ratio = lua_tonumber(L, 1);
				return 0;
			}
		}

		void Init(lua_State *L)
		{
			lua_createtable(L, 0, 0);
			g_body_list = luaL_ref(L, LUA_REGISTRYINDEX);
			g_debug_data = new DebugData();

			lua_createtable(L, 0, 16);

			lua_rawsetcclosurefield(L, &set_ptm_ratio, 0, "SetPTMRatio");

			// b2CircleShape
			lua_rawsetcclosurefield(L, &CircleShape, 0, "CircleShape");

			// b2PolygonShape
			Rainbow::Lua::wrap<PolygonShape>(L);

			// b2BodyType
			lua_rawsetfield(L, lua_pushinteger, b2_staticBody, "staticBody");
			lua_rawsetfield(L, lua_pushinteger, b2_kinematicBody, "kinematicBody");
			lua_rawsetfield(L, lua_pushinteger, b2_dynamicBody, "dynamicBody");

			// b2BodyDef
			lua_rawsetcclosurefield(L, &BodyDef, 0, "BodyDef");

			// b2Body
			Rainbow::Lua::wrap<Body>(L);

			// b2FixtureDef
			lua_rawsetcclosurefield(L, &FixtureDef, 0, "FixtureDef");

			// b2Fixture
			Rainbow::Lua::wrap<Fixture>(L, true);

			// b2World
			Rainbow::Lua::wrap<World>(L);

			// b2Contact
			Rainbow::Lua::wrap<Contact>(L, true);

			lua_setglobal(L, "b2");
		}
	}
}

#endif
