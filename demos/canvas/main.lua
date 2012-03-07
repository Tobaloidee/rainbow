--! Canvas Demo
--!
--! Resolution: 960x640
--!
--! Copyright 2012 Bifrost Entertainment. All rights reserved.
--! \author Tommy Nguyen

require("Utils")  -- Rainbow (debugging) utilities
require("Input")  -- Input event handler

assets   = nil  -- Sprite sheet
batch    = nil  -- Sprite batch with only one sprite
bgm      = nil  -- Background music
canvas   = nil  -- Canvas
original = nil  -- Backdrop

function init()
	assets = rainbow.texture("canvas.png")

	batch = rainbow.spritebatch(1)
	batch:set_texture(assets)

	local screen = rainbow.platform.screen
	local width = 960
	local height = 640

	original = batch:add(0, 0, width, height)
	original:set_position(screen.width * 0.5, screen.height * 0.5)
	original:set_scale(screen.width / width)

	local blurred = assets:create(1088, 0, width, height)
	local brush   = assets:create(0, 919, 105, 105)

	canvas = rainbow.canvas()
	canvas:set_brush(assets, brush)
	canvas:set_brush_size(64)
	canvas:set_foreground(0x000000ff)
	canvas:set_background(assets, blurred, width, height)

	canvas:clear()

	rainbow.scenegraph:add_batch(batch)
	rainbow.scenegraph:add_drawable(canvas)

	bgm = rainbow.audio.load_stream("kimjongil.ogg");
	rainbow.audio.play(bgm);
end

function update()
end