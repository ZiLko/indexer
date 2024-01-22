#include <Geode/Geode.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/PlayLayer.hpp>
using namespace geode::prelude;

bool should_key_jump(int key) {
    return (key >= 'A' && key <= 'Z' && key != 'P') ||
    (key >= '0' && key <= '9') ||
    key == 0x10 ||
    key == 0xBC ||
    key == 0xBE ||
    key == 0x0D;
}

static const std::set<int> p1_keys = {'1','2','3','4','5','Q','W','E','R','T','A','S','D','F','G','Z','X','C','V','B'};
bool g_left_shift = false;
bool g_right_shift = false;
bool is_player1(int key) {
    return p1_keys.find(key) != p1_keys.end();
}
class $modify(CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(cocos2d::enumKeyCodes enumKey, bool down, bool arr) {
        int key = static_cast<int>(enumKey);
    auto play_layer = PlayLayer::get();
    if (play_layer && should_key_jump(key)) {
        auto is_practice_mode = play_layer->m_isPracticeMode;
        auto is_platformer = play_layer->m_levelSettings->m_platformerMode;
        if ((!is_practice_mode || (key != 'Z' && key != 'X')) && ((key != 'A' && key != 'D') || !is_platformer)) {
            bool player1 = true;
            if (key == 0x10) {
                bool left_shift = GetAsyncKeyState(VK_LSHIFT) < 0;
                bool right_shift = GetAsyncKeyState(VK_RSHIFT) < 0;
                if (down) {
                    if (right_shift && !g_right_shift) {
                        g_right_shift = true;
                    } else {
                        g_left_shift = true;
                        right_shift = false;
                    }
                } else {
                    if (g_right_shift && !right_shift) {
                        right_shift = true;
                        g_right_shift = false;
                    } else {
                        right_shift = false;
                        g_left_shift = false;
                    }
                }
                player1 = !right_shift;
            }
            else {
                player1 = is_player1(key);
            }

               play_layer->handleButton(down, 1, player1);
        }
    }
        return CCKeyboardDispatcher::dispatchKeyboardMSG(enumKey, down, arr);
    }
};
