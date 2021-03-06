// This file was generated by `generate-bindings.ts`. DO NOT MODIFY!
//
// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

declare namespace Duktape {
  const version: number;
  const env: string;

  function fin(o: {}, finalizer?: () => void): (() => void) | undefined;
  function enc(format: "base64" | "hex" | "jc" | "jx", ...values: any[]): string;
  function dec(format: "base64" | "hex" | "jc" | "jx", value: string): Buffer | string;
  function info(value: any): {};
  function act(index: number): {};
  function gc(flags?: number): void;
  function compact(o: {}): void;

  let errCreate: ((e: Error) => Error) | undefined;
  let errThrow: ((e: Error) => Error) | undefined;

  class Thread {
    static current(): Thread;
    static resume(thread: Thread, value?: any, thrown?: boolean): any;
    static yield(value?: any, thrown?: boolean): any;

    private readonly $type: "Duktape.Thread";

    constructor(yielder: (value?: any) => any);
  }
}

declare namespace Rainbow {
  export class Animation {
    private readonly $type: "Rainbow.Animation";
    constructor(sprite: Sprite, frames: number[], fps: number, delay: number);
    currentFrame(): number;
    frameRate(): number;
    sprite(): Sprite;
    setCallback(callback: (animation: Animation, event: AnimationEvent) => void): void;
    setDelay(delay: number): void;
    setFrameRate(fps: number): void;
    setFrames(frames: number[]): void;
    setSprite(sprite: Sprite): void;
    jumpTo(frame: number): void;
    rewind(): void;
    start(): void;
    stop(): void;
  }

  export enum AnimationEvent {
    Start = 0,
    End = 1,
    Complete = 2,
    Frame = 3,
  }

  export enum ControllerAxis {
    Invalid = -1,
    LeftX = 0,
    LeftY = 1,
    RightX = 2,
    RightY = 3,
    TriggerLeft = 4,
    TriggerRight = 5,
    Count = 6,
  }

  export enum ControllerButton {
    Invalid = -1,
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    Back = 4,
    Guide = 5,
    Start = 6,
    LeftStick = 7,
    RightStick = 8,
    LeftShoulder = 9,
    RightShoulder = 10,
    DPadUp = 11,
    DPadDown = 12,
    DPadLeft = 13,
    DPadRight = 14,
    Count = 15,
  }

  export class Label {
    private readonly $type: "Rainbow.Label";
    constructor();
    alignment(): TextAlignment;
    angle(): number;
    color(): { r: number, g: number, b: number, a: number };
    height(): number;
    length(): number;
    position(): { x: number, y: number };
    scale(): number;
    text(): string;
    width(): number;
    setAlignment(alignment: TextAlignment): void;
    setColor(color: { r: number, g: number, b: number, a: number }): void;
    setFont(font: string): void;
    setFontSize(fontSize: number): void;
    setPosition(position: { x: number, y: number }): void;
    setRotation(r: number): void;
    setScale(factor: number): void;
    setText(text: string): void;
    move(delta: { x: number, y: number }): void;
  }

  export class Sprite {
    private readonly $type: "Rainbow.Sprite";
    angle(): number;
    color(): { r: number, g: number, b: number, a: number };
    height(): number;
    id(): number;
    isFlipped(): boolean;
    isHidden(): boolean;
    isMirrored(): boolean;
    pivot(): { x: number, y: number };
    position(): { x: number, y: number };
    scale(): { x: number, y: number };
    width(): number;
    setColor(color: { r: number, g: number, b: number, a: number }): void;
    setId(id: number): void;
    setNormal(id: number): void;
    setPivot(pivot: { x: number, y: number }): void;
    setPosition(position: { x: number, y: number }): void;
    setRotation(r: number): void;
    setScale(factor: { x: number, y: number }): void;
    setTexture(id: number): void;
    flip(): void;
    hide(): void;
    mirror(): void;
    move(dt: { x: number, y: number }): void;
    rotate(r: number): void;
    show(): void;
  }

  export class SpriteBatch {
    private readonly $type: "Rainbow.SpriteBatch";
    constructor(count: number);
    isVisible(): boolean;
    setNormal(texture: Texture): void;
    setTexture(texture: Texture): void;
    setVisible(visible: boolean): void;
    clear(): void;
    createSprite(width: number, height: number): Sprite;
    erase(i: number): void;
    findSpriteById(id: number): Sprite;
    swap(a: Sprite, b: Sprite): void;
  }

  export enum TextAlignment {
    Left = 0,
    Right = 1,
    Center = 2,
  }

  export class Texture {
    private readonly $type: "Rainbow.Texture";
    constructor(path: string);
    addRegion(x: number, y: number, width: number, height: number): number;
    trim(): void;
  }

  export enum VirtualKey {
    Unknown = 0,
    A = 1,
    B = 2,
    C = 3,
    D = 4,
    E = 5,
    F = 6,
    G = 7,
    H = 8,
    I = 9,
    J = 10,
    K = 11,
    L = 12,
    M = 13,
    N = 14,
    O = 15,
    P = 16,
    Q = 17,
    R = 18,
    S = 19,
    T = 20,
    U = 21,
    V = 22,
    W = 23,
    X = 24,
    Y = 25,
    Z = 26,
    Number1 = 27,
    Number2 = 28,
    Number3 = 29,
    Number4 = 30,
    Number5 = 31,
    Number6 = 32,
    Number7 = 33,
    Number8 = 34,
    Number9 = 35,
    Number0 = 36,
    Return = 37,
    Escape = 38,
    Backspace = 39,
    Tab = 40,
    Space = 41,
    Minus = 42,
    Equals = 43,
    LeftBracket = 44,
    RightBracket = 45,
    Backslash = 46,
    Semicolon = 47,
    Apostrophe = 48,
    Grave = 49,
    Comma = 50,
    Period = 51,
    Slash = 52,
    CapsLock = 53,
    F1 = 54,
    F2 = 55,
    F3 = 56,
    F4 = 57,
    F5 = 58,
    F6 = 59,
    F7 = 60,
    F8 = 61,
    F9 = 62,
    F10 = 63,
    F11 = 64,
    F12 = 65,
    PrintScreen = 66,
    ScrollLock = 67,
    Pause = 68,
    Insert = 69,
    Home = 70,
    PageUp = 71,
    Delete = 72,
    End = 73,
    PageDown = 74,
    Right = 75,
    Left = 76,
    Down = 77,
    Up = 78,
    NumLock = 79,
    KP_Divide = 80,
    KP_Multiply = 81,
    KP_Minus = 82,
    KP_Plus = 83,
    KP_Enter = 84,
    KP_1 = 85,
    KP_2 = 86,
    KP_3 = 87,
    KP_4 = 88,
    KP_5 = 89,
    KP_6 = 90,
    KP_7 = 91,
    KP_8 = 92,
    KP_9 = 93,
    KP_0 = 94,
    KP_Period = 95,
    LessThan = 96,
    Application = 97,
    Power = 98,
    KP_Equals = 99,
    F13 = 100,
    F14 = 101,
    F15 = 102,
    F16 = 103,
    F17 = 104,
    F18 = 105,
    F19 = 106,
    F20 = 107,
    F21 = 108,
    F22 = 109,
    F23 = 110,
    F24 = 111,
    LeftCtrl = 112,
    LeftShift = 113,
    LeftAlt = 114,
    LeftSuper = 115,
    RightCtrl = 116,
    RightShift = 117,
    RightAlt = 118,
    RightSuper = 119,
    Mode = 120,
    KeyCount = 121,
  }

  export namespace Audio {
    function loadSound(path: string): Sound | undefined;
    function loadStream(path: string): Sound | undefined;
    function release(sound: Sound): undefined;
    function isPaused(channel: Channel): boolean;
    function isPlaying(channel: Channel): boolean;
    function setLoopCount(channel: Channel, count: number): void;
    function setVolume(channel: Channel, volume: number): void;
    function setWorldPosition(channel: Channel, position: { x: number, y: number }): void;
    function pause(channel: Channel): void;
    function play(audial: Channel | Sound): Channel | undefined;
    function stop(channel: Channel): void;
    export class Channel {
      private readonly $type: "Rainbow.Channel";
    }
    export class Sound {
      private readonly $type: "Rainbow.Sound";
    }
  }

  export namespace Input {
    const acceleration: Float64Array;
    const controllers: ReadonlyArray<Readonly<{ id: number, buttons: Int8Array, axes: Int32Array }>>;
    const keysDown: Int8Array;
    const pointersDown: ReadonlyArray<Readonly<{ hash: number, x: number, y: number, timestamp: number }>>;
    const pointersMoved: ReadonlyArray<Readonly<{ hash: number, x: number, y: number, timestamp: number }>>;
    const pointersUp: ReadonlyArray<Readonly<{ hash: number, x: number, y: number, timestamp: number }>>;
  }

  export namespace RenderQueue {
    function add(obj: Animation | Label | SpriteBatch): void;
    function disable(obj: Animation | Label | SpriteBatch | number | string): void;
    function enable(obj: Animation | Label | SpriteBatch | number | string): void;
    function insert(position: number, obj: Animation | Label | SpriteBatch): void;
    function erase(obj: Animation | Label | SpriteBatch | number | string): void;
    function setTag(obj: Animation | Label | SpriteBatch, tag: string): void;
  }
}
