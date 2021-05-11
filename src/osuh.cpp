#include "header.hpp"

namespace osuh {
Json::Value left_key_value, right_key_value, smoke_key_value, wave_key_value;
int offset_x, offset_y;
int paw_r, paw_g, paw_b, paw_a;
int paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a;
double scale;
bool is_mouse, is_left_handed, is_enable_toggle_smoke;
sf::Sprite bg, up, left, right, device, smoke, wave, hand, head;
sf::CircleShape test_dot;

sf::Font debugFont;
sf::Text debugText;

const float rad_to_deg = 180.0f / 3.141592f;
const float deg_to_rad = 3.141592f / 180.0f;

const float rot_angle = -20.0f * deg_to_rad;
const float sin_rot_angle = sin(rot_angle);
const float cos_rot_angle = cos(rot_angle);

const float off_x = -10;
const float off_y = -70;
const float off_scale_x = 0.4f;
const float off_scale_y = 0.4f;

float p1_x = 131;
float p1_y = 208;
float p2_x = 184;
float p2_y = 324;

float mid_x = 0.0f;
float mid_y = 0.0f;

float test = 0;

int key_state = 0;

bool left_key_state = false;
bool right_key_state = false;
bool wave_key_state = false;
bool previous_smoke_key_state = false;
bool current_smoke_key_state = false;
bool is_toggle_smoke = false;
double timer_left_key = -1;
double timer_right_key = -1;
double timer_wave_key = -1;

bool init() {
    // getting configs
    Json::Value osuh = data::cfg["osuh"];

    is_mouse = osuh["mouse"].asBool();
    is_enable_toggle_smoke = osuh["toggleSmoke"].asBool();

    bool chk[256];
    std::fill(chk, chk + 256, false);
    left_key_value = osuh["key1"];
    for (Json::Value &v : left_key_value) {
        chk[v.asInt()] = true;
    }
    right_key_value = osuh["key2"];
    for (Json::Value &v : right_key_value) {
        if (chk[v.asInt()]) {
            data::error_msg("Overlapping osuh! keybinds", "Error reading configs");
            return false;
        }
    }
    wave_key_value = osuh["wave"];
    for (Json::Value &v : wave_key_value) {
        if (chk[v.asInt()]) {
            data::error_msg("Overlapping osuh! keybinds", "Error reading configs");
            return false;
        }
    }
    smoke_key_value = osuh["smoke"];

    is_left_handed = data::cfg["decoration"]["leftHanded"].asBool();

    if (is_mouse) {
        offset_x = (data::cfg["decoration"]["offsetX"])[0].asInt();
        offset_y = (data::cfg["decoration"]["offsetY"])[0].asInt();
        scale = (data::cfg["decoration"]["scalar"])[0].asDouble();
    } else {
        offset_x = (data::cfg["decoration"]["offsetX"])[1].asInt();
        offset_y = (data::cfg["decoration"]["offsetY"])[1].asInt();
        scale = (data::cfg["decoration"]["scalar"])[1].asDouble();
    }

    // importing sprites
    up.setTexture(data::load_texture("img/osuh/up.png"));
    left.setTexture(data::load_texture("img/osuh/left.png"));
    right.setTexture(data::load_texture("img/osuh/right.png"));
    wave.setTexture(data::load_texture("img/osuh/wave.png"));
    if (is_mouse) {
        bg.setTexture(data::load_texture("img/osuh/mousebg.png"));
        device.setTexture(data::load_texture("img/osuh/mouse.png"), true);
    } else {
        bg.setTexture(data::load_texture("img/osuh/tabletbg.png"));
        device.setTexture(data::load_texture("img/osuh/tablet.png"), true);
    }
    smoke.setTexture(data::load_texture("img/osuh/smoke.png"));
    device.setScale(scale, scale);

    head.setTexture(data::load_texture("img/osuh/head.png"));

    hand.setTexture(data::load_texture("img/osuh/hand.png"));
    hand.setScale(0.55f, 0.55f);
    hand.setOrigin(57.5f, 300.0f);

    test_dot.setOutlineColor(sf::Color::Red);
    test_dot.setFillColor(sf::Color::Red);
    test_dot.setRadius(3);
    test_dot.setOutlineThickness(2);

    debugFont.loadFromFile("font/RobotoMono-Bold.ttf");
    debugText.setCharacterSize(14);
    debugText.setFont(debugFont);
    debugText.setColor(sf::Color::Black);
    debugText.setPosition(100,50);

    mid_x = 0.5f * (p2_x - p1_x) + p1_x + off_x;
    mid_y = 0.5f * (p2_y - p1_y) + p1_y + off_y;

    return true;
}

void draw() {
    window.draw(bg);

    auto [mx, my] = input::get_xy();

    float r_x = (mx + off_x) - mid_x;
    float r_y = (my + off_y) - mid_y;

    float p_x = r_x * cos_rot_angle - r_y * sin_rot_angle;
    float p_y = r_x * sin_rot_angle + r_y * cos_rot_angle;

    float x = off_scale_x * p_x + mid_x;
    float y = off_scale_y * p_y + mid_y;

    float hand_x = x;
    float hand_y = y;

    float hand_vec_x = 300 - hand_x;
    float hand_vec_y = 165 - hand_y;

    float alpha = (-hand_vec_y) / sqrt(hand_vec_x * hand_vec_x + hand_vec_y * hand_vec_y);
    float hand_angle = acos(alpha) * rad_to_deg;


    float device_x = x - 40;
    float device_y = y - 50;


    // setting hand position/rotation
    hand.setRotation(hand_angle);
    hand.setPosition(hand_x, hand_y);

    // setting device position
    device.setPosition(device_x, device_y);

    // drawing device/hand
    window.draw(device);
    window.draw(hand);
    
    // draw head here so it covers the arm
    window.draw(head);

    test_dot.setPosition(x, y);
    window.draw(test_dot);

    debugText.setString("x=" + std::to_string(x) + "  y=" + std::to_string(y));
    window.draw(debugText);


    // drawing keypresses
    bool left_key = false;

    for (Json::Value &v : left_key_value) {
        if (input::is_pressed(v.asInt())) {
            left_key = true;
            break;
        }
    }

    if (left_key) {
        if (!left_key_state) {
            key_state = 1;
            left_key_state = true;
        }
    } else {
        left_key_state = false;
    }

    bool right_key = false;

    for (Json::Value &v : right_key_value) {
        if (input::is_pressed(v.asInt())) {
            right_key = true;
            break;
        }
    }

    if (right_key) {
        if (!right_key_state) {
            key_state = 2;
            right_key_state = true;
        }
    } else {
        right_key_state = false;
    }
    
    bool wave_key = false;

    for (Json::Value &v : wave_key_value) {
        if (input::is_pressed(v.asInt())) {
            wave_key = true;
            break;
        }
    }

    if (wave_key) {
        if (!wave_key_state) {
            key_state = 3;
            wave_key_state = true;
        }
    } else {
        wave_key_state = false;
    }

    if (!left_key_state && !right_key_state && !wave_key_state) {
        key_state = 0;
        window.draw(up);
    }

    if (key_state == 1) {
        if ((clock() - std::max(timer_right_key, timer_wave_key)) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
            if (!is_left_handed) {
                window.draw(left);
            } else {
                window.draw(right);
            }
            timer_left_key = clock();
        } else {
            window.draw(up);
        }
    } else if (key_state == 2) {
        if ((clock() - std::max(timer_left_key, timer_wave_key)) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
            if (!is_left_handed) {
                window.draw(right);
            } else {
                window.draw(left);
            }
            timer_right_key = clock();
        } else {
            window.draw(up);
        }
    } else if (key_state == 3) {
        if ((clock() - std::max(timer_left_key, timer_right_key)) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
            window.draw(wave);
            timer_wave_key = clock();
        } else {
            window.draw(up);
        }
    }
    
    // draw smoke
    bool is_smoke_key_pressed = false;

    for (Json::Value &v : smoke_key_value) {
        if (input::is_pressed(v.asInt())) {
            is_smoke_key_pressed = true;
            break;
        }
    }

    if (is_enable_toggle_smoke) {
        previous_smoke_key_state = current_smoke_key_state;
        current_smoke_key_state = is_smoke_key_pressed;

        bool is_smoke_key_down = current_smoke_key_state && (current_smoke_key_state != previous_smoke_key_state);

        if (is_smoke_key_down) {
            is_toggle_smoke = !is_toggle_smoke;
        }
    }
    else {
        is_toggle_smoke = is_smoke_key_pressed;
    }

    if (is_toggle_smoke) {
        window.draw(smoke);
    }
}
}; // namespace osuh
