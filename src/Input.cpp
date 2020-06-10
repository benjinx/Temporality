#include <Input.hpp>

#include <App.hpp>
#include <Log.hpp>

Input::Input()
{
    // Initialize each one with the defaults
    _mMouseButtonMap.emplace("LEFT_CLICK", MouseButton::MOUSE_BUTTON_LEFT);
    _mMouseButtonMap.emplace("MIDDLE_CLICK", MouseButton::MOUSE_BUTTON_MIDDLE);
    _mMouseButtonMap.emplace("RIGHT_CLICK", MouseButton::MOUSE_BUTTON_RIGHT);

    //_mKeyboardMap.emplace("QUIT", KeyboardKey::ESC);
}

Input::~Input()
{

}

void Input::Init()
{
    // Controller
    int const MAX_JOYSTICKS = SDL_NumJoysticks();

    for (int i = 0; i < MAX_JOYSTICKS; ++i)
    {
        if (SDL_IsGameController(i))
        {
            _mGameControllers.push_back(SDL_GameControllerOpen(i));
        }
    }

    for (size_t i = 0; i < _mGameControllers.size(); ++i)
        LogInfo("CONTROLLER: %s\n", SDL_GameControllerName(_mGameControllers[i]));
}

void Input::ProcessEvent(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN) //|| event->type == SDL_KEYUP)
    {
        // Handle Keyboard
        auto key = SDLKeyToMyKey(event->key.keysym.sym);

        // Then change the switch below to match ours
        switch (key)
        {
            case KEY_UNKNOWN:
                LogInfo("Unknown Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_RETURN:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_ESCAPE:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_BACKSPACE:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_TAB:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_SPACE:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_EXCLAIM:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_QUOTEDBL:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_HASH:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_PERCENT:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_DOLLAR:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_AMPERSAND:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_QUOTE:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_LEFTPAREN:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_RIGHTPAREN:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_ASTERISK:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_PLUS:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_COMMA:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_MINUS:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_PERIOD:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_SLASH:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_0:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_1:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_2:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_3:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_4:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_5:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_6:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_7:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_8:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_9:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_COLON:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_SEMICOLON:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_LESS:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_EQUALS:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_GREATER:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_QUESTION:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_AT:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_LEFTBRACKET:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_BACKSLASH:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_RIGHTBRACKET:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_CARET:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_UNDERSCORE:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_BACKQUOTE:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_A:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_B:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_C:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_D:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_E:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_G:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_H:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_I:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_J:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_K:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_L:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_M:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_N:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_O:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_P:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_Q:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_R:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_S:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_T:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_U:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_V:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_W:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_X:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_Y:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_Z:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_CAPSLOCK:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F1:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F2:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F3:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F4:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F5:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F6:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F7:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F8:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F9:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F10:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F11:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_F12:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_PRINTSCREEN:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_SCROLLLOCK:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_PAUSE:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_INSERT:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_HOME:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_PAGEUP:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_DELETE:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_END:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_PAGEDOWN:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_RIGHT:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_LEFT:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_DOWN:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_UP:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_NUMLOCKCLEAR:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_DIVIDE:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_MULTIPLY:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_MINUS:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_PLUS:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_ENTER:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_1:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_2:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_3:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_4:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_5:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_6:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_7:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_8:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_9:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_0:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
            case KEY_KP_PERIOD:
                LogInfo("Key: %d\n", event->key.keysym.sym);
                break;
        }
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        // Handle Mouse
        auto mouse = SDLMouseButtonToMyButton(event->button.button);

        switch (mouse)
        {
        case MOUSE_BUTTON_UNKNOWN:
            LogInfo("Unknown Mouse Button: %d\n", event->button.button);
            break;
        case MOUSE_BUTTON_LEFT:
            LogInfo("Left Mouse Button: %d\n", event->button.button);
            break;
        case MOUSE_BUTTON_MIDDLE:
            LogInfo("Middle Mouse Button: %d\n", event->button.button);
            break;
        case MOUSE_BUTTON_RIGHT:
            LogInfo("Right Mouse Button: %d\n", event->button.button);
            break;
        }
    }
    else if (event->type == SDL_CONTROLLERBUTTONDOWN)
    {
        // Handle Gamepad
        auto gamepad = SDLGamepadButtonToMyButton(event->cbutton.button);

        switch (gamepad)
        {
            case GAME_CONTROLLER_BUTTON_INVALID:
                LogInfo("Controller Button Invalid: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_A:
                LogInfo("Controller Button A: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_B:
                LogInfo("Controller Button B: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_X:
                LogInfo("Controller Button X: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_Y:
                LogInfo("Controller Button Y: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_BACK:
                LogInfo("Controller Button Back: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_GUIDE:
                LogInfo("Controller Button Guide: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_START:
                LogInfo("Controller Button Start: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_LEFTSTICK:
                LogInfo("Controller Button Left Stick: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_RIGHTSTICK:
                LogInfo("Controller Button Right Stick: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_LEFTSHOULDER:
                LogInfo("Controller Button Left Shoulder: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_RIGHTSHOULDER:
                LogInfo("Controller Button Right Shoulder: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_DPAD_UP:
                LogInfo("Controller Button Dpad Up: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_DPAD_DOWN:
                LogInfo("Controller Button Dpad Down: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_DPAD_LEFT:
                LogInfo("Controller Button Dpad Left: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_DPAD_RIGHT:
                LogInfo("Controller Button Dpad Right: %d\n", event->cbutton.button);
                break;
            case GAME_CONTROLLER_BUTTON_MAX:
                LogInfo("Controller Button Max: %d\n", event->cbutton.button);
                break;
        }
    }
}

Input::KeyboardKey Input::SDLKeyToMyKey(SDL_Keycode key)
{
    switch (key)
    {
        case SDLK_UNKNOWN:          //= 0,
            return KEY_UNKNOWN;
        case SDLK_RETURN:           //= '\r',
            return KEY_RETURN;
        case SDLK_ESCAPE:           //= '\033',
            return KEY_ESCAPE;
        case SDLK_BACKSPACE:        //= '\b',
            return KEY_BACKSPACE;
        case SDLK_TAB:              //= '\t',
            return KEY_TAB;
        case SDLK_SPACE:            //= ' ',
            return KEY_SPACE;
        case SDLK_EXCLAIM:          //= '!',
            return KEY_EXCLAIM;
        case SDLK_QUOTEDBL:         //= '"',
            return KEY_QUOTEDBL;
        case SDLK_HASH:             //= '#',
            return KEY_HASH;
        case SDLK_PERCENT:          //= '%',
            return KEY_PERCENT;
        case SDLK_DOLLAR:           //= '$',
            return KEY_DOLLAR;
        case SDLK_AMPERSAND:        //= '&',
            return KEY_AMPERSAND;
        case SDLK_QUOTE:            //= '\'',
            return KEY_QUOTE;
        case SDLK_LEFTPAREN:        //= '(',
            return KEY_LEFTPAREN;
        case SDLK_RIGHTPAREN:       //= ')',
            return KEY_RIGHTPAREN;
        case SDLK_ASTERISK:         //= '*',
            return KEY_ASTERISK;
        case SDLK_PLUS:             //= '+',
            return KEY_PLUS;
        case SDLK_COMMA:            //= ',',
            return KEY_COMMA;
        case SDLK_MINUS:            //= '-',
            return KEY_MINUS;
        case SDLK_PERIOD:           //= '.',
            return KEY_PERIOD;
        case SDLK_SLASH:            //= '/',
            return KEY_SLASH;
        case SDLK_0:                //= '0',
            return KEY_0;
        case SDLK_1:                //= '1',
            return KEY_1;
        case SDLK_2:                //= '2',
            return KEY_2;
        case SDLK_3:                //= '3',
            return KEY_3;
        case SDLK_4:                //= '4',
            return KEY_4;
        case SDLK_5:                //= '5',
            return KEY_5;
        case SDLK_6:                //= '6',
            return KEY_6;
        case SDLK_7:                //= '7',
            return KEY_7;
        case SDLK_8:                //= '8',
            return KEY_8;
        case SDLK_9:                //= '9',
            return KEY_9;
        case SDLK_COLON:            //= ':',
            return KEY_COLON;
        case SDLK_SEMICOLON:        //= ';',
            return KEY_SEMICOLON;
        case SDLK_LESS:             //= '<',
            return KEY_LESS;
        case SDLK_EQUALS:           //= '=',
            return KEY_EQUALS;
        case SDLK_GREATER:          //= '>',
            return KEY_GREATER;
        case SDLK_QUESTION:         //= '?',
            return KEY_QUESTION;
        case SDLK_AT:               //= '@',
            return KEY_AT;
        case SDLK_LEFTBRACKET:      //= '[',
            return KEY_LEFTBRACKET;
        case SDLK_BACKSLASH:        //= '\\',
            return KEY_BACKSLASH;
        case SDLK_RIGHTBRACKET:     //= ']',
            return KEY_RIGHTBRACKET;
        case SDLK_CARET:            //= '^',
            return KEY_CARET;
        case SDLK_UNDERSCORE:       //= '_',
            return KEY_UNDERSCORE;
        case SDLK_BACKQUOTE:        //= '`',
            return KEY_BACKQUOTE;
        case SDLK_a:                //= 'a',
            return KEY_A;
        case SDLK_b:                //= 'b',
            return KEY_B;
        case SDLK_c:                //= 'c',
            return KEY_C;
        case SDLK_d:                //= 'd',
            return KEY_D;
        case SDLK_e:                //= 'e',
            return KEY_E;
        case SDLK_f:                //= 'f',
            return KEY_F;
        case SDLK_g:                //= 'g',
            return KEY_G;
        case SDLK_h:                //= 'h',
            return KEY_H;
        case SDLK_i:                //= 'i',
            return KEY_I;
        case SDLK_j:                //= 'j',
            return KEY_J;
        case SDLK_k:                //= 'k',
            return KEY_K;
        case SDLK_l:                //= 'l',
            return KEY_L;
        case SDLK_m:                //= 'm',
            return KEY_M;
        case SDLK_n:                //= 'n',
            return KEY_N;
        case SDLK_o:                //= 'o',
            return KEY_O;
        case SDLK_p:                //= 'p',
            return KEY_P;
        case SDLK_q:                //= 'q',
            return KEY_Q;
        case SDLK_r:                //= 'r',
            return KEY_R;
        case SDLK_s:                //= 's',
            return KEY_S;
        case SDLK_t:                //= 't',
            return KEY_T;
        case SDLK_u:                //= 'u',
            return KEY_U;
        case SDLK_v:                //= 'v',
            return KEY_V;
        case SDLK_w:                //= 'w',
            return KEY_W;
        case SDLK_x:                //= 'x',
            return KEY_X;
        case SDLK_y:                //= 'y',
            return KEY_Y;
        case SDLK_z:                //= 'z',
            return KEY_Z;
        case SDLK_CAPSLOCK:         //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK),
            return KEY_CAPSLOCK;
        case SDLK_F1:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1),
            return KEY_F1;
        case SDLK_F2:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2),
            return KEY_F2;
        case SDLK_F3:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3),
            return KEY_F3;
        case SDLK_F4:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4),
            return KEY_F4;
        case SDLK_F5:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5),
            return KEY_F5;
        case SDLK_F6:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6),
            return KEY_F6;
        case SDLK_F7:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7),
            return KEY_F7;
        case SDLK_F8:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8),
            return KEY_F8;
        case SDLK_F9:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9),
            return KEY_F9;
        case SDLK_F10:              //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10),
            return KEY_F10;
        case SDLK_F11:              //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11),
            return KEY_F11;
        case SDLK_F12:              //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12),
            return KEY_F12;
        case SDLK_PRINTSCREEN:      //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN),
            return KEY_PRINTSCREEN;
        case SDLK_SCROLLLOCK:       //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK),
            return KEY_SCROLLLOCK;
        case SDLK_PAUSE:            //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE),
            return KEY_PAUSE;
        case SDLK_INSERT:           //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT),
            return KEY_INSERT;
        case SDLK_HOME:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME),
            return KEY_HOME;
        case SDLK_PAGEUP:           //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP),
            return KEY_PAGEUP;
        case SDLK_DELETE:           //= '\177',
            return KEY_DELETE;
        case SDLK_END:              //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END),
            return KEY_END;
        case SDLK_PAGEDOWN:         //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN),
            return KEY_PAGEDOWN;
        case SDLK_RIGHT:            //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT),
            return KEY_RIGHT;
        case SDLK_LEFT:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT),
            return KEY_LEFT;
        case SDLK_DOWN:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN),
            return KEY_DOWN;
        case SDLK_UP:               //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP),
            return KEY_UP;
        case SDLK_NUMLOCKCLEAR:     //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR),
            return KEY_NUMLOCKCLEAR;
        case SDLK_KP_DIVIDE:        //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE),
            return KEY_KP_DIVIDE;
        case SDLK_KP_MULTIPLY:      //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY),
            return KEY_KP_MULTIPLY;
        case SDLK_KP_MINUS:         //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MINUS),
            return KEY_KP_MINUS;
        case SDLK_KP_PLUS:          //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUS),
            return KEY_KP_PLUS;
        case SDLK_KP_ENTER:         //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER),
            return KEY_KP_ENTER;
        case SDLK_KP_1:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_1),
            return KEY_KP_1;
        case SDLK_KP_2:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_2),
            return KEY_KP_2;
        case SDLK_KP_3:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_3),
            return KEY_KP_3;
        case SDLK_KP_4:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_4),
            return KEY_KP_4;
        case SDLK_KP_5:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_5),
            return KEY_KP_5;
        case SDLK_KP_6:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_6),
            return KEY_KP_6;
        case SDLK_KP_7:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_7),
            return KEY_KP_7;
        case SDLK_KP_8:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_8),
            return KEY_KP_8;
        case SDLK_KP_9:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_9),
            return KEY_KP_9;
        case SDLK_KP_0:             //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_0),
            return KEY_KP_0;
        case SDLK_KP_PERIOD:        //= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERIOD),
            return KEY_KP_PERIOD;
    }

    return KEY_UNKNOWN;
}

Input::MouseButton Input::SDLMouseButtonToMyButton(Uint8 mouseButton)
{
    switch (mouseButton)
    {
        case SDL_BUTTON_LEFT:
            return MOUSE_BUTTON_LEFT;
        case SDL_BUTTON_MIDDLE:
            return MOUSE_BUTTON_MIDDLE;
        case SDL_BUTTON_RIGHT:
            return MOUSE_BUTTON_RIGHT;
    }

    return MOUSE_BUTTON_UNKNOWN;
}

Input::GamepadButton Input::SDLGamepadButtonToMyButton(Uint8 button)
{
    switch (button)
    {
        case SDL_CONTROLLER_BUTTON_INVALID:
            return GAME_CONTROLLER_BUTTON_INVALID;
        case SDL_CONTROLLER_BUTTON_A:
            return GAME_CONTROLLER_BUTTON_A;
        case SDL_CONTROLLER_BUTTON_B:
            return GAME_CONTROLLER_BUTTON_B;
        case SDL_CONTROLLER_BUTTON_X:
            return GAME_CONTROLLER_BUTTON_X;
        case SDL_CONTROLLER_BUTTON_Y:
            return GAME_CONTROLLER_BUTTON_Y;
        case SDL_CONTROLLER_BUTTON_BACK:
            return GAME_CONTROLLER_BUTTON_BACK;
        case SDL_CONTROLLER_BUTTON_GUIDE:
            return GAME_CONTROLLER_BUTTON_GUIDE;
        case SDL_CONTROLLER_BUTTON_START:
            return GAME_CONTROLLER_BUTTON_START;
        case SDL_CONTROLLER_BUTTON_LEFTSTICK:
            return GAME_CONTROLLER_BUTTON_LEFTSTICK;
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
            return GAME_CONTROLLER_BUTTON_RIGHTSTICK;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            return GAME_CONTROLLER_BUTTON_LEFTSHOULDER;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            return GAME_CONTROLLER_BUTTON_RIGHTSHOULDER;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            return GAME_CONTROLLER_BUTTON_DPAD_UP;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            return GAME_CONTROLLER_BUTTON_DPAD_DOWN;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            return GAME_CONTROLLER_BUTTON_DPAD_LEFT;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            return GAME_CONTROLLER_BUTTON_DPAD_RIGHT;
        case SDL_CONTROLLER_BUTTON_MAX:
            return GAME_CONTROLLER_BUTTON_MAX;
    }
    return GAME_CONTROLLER_BUTTON_INVALID;
}