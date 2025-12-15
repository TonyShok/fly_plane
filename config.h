#ifndef CONFIG_H
#define CONFIG_H

/*********************** 窗口设置 ***********************/
const int W_WIDTH = 600;                    // 窗口宽度
const int W_HEIGHT = 800;                   // 窗口高度
const int W_FRAME_RATE_LIMIT = 60;          // 帧率限制
const int W_BORDER = 20;                    // 窗口边界宽度

/*********************** 玩家飞机设置 ***********************/
const float P_SIZE = 50;                    // 玩家飞机尺寸
const float P_SPEED = 200;                  // 玩家飞机移动速度(像素/秒)
const float P_FIRE_COOLDOWN = 0.20;         // 玩家射击冷却时间(秒)
const int P_MAX_HEALTH = 100;               // 玩家最大生命值
const int P_INITIAL_LIVES = 3;              // 玩家初始生命数

/*********************** 敌机设置 ***********************/
const float E_RADIUS = 20;                  // 敌机碰撞半径
const float E_SPEED = 90;                  // 敌机移动速度(像素/秒)
const float E_SPAWN_TIME = 2.0;               // 敌机生成间隔(秒)
const float E_FIRE_COOLDOWN = 1.1;        // 敌机开火间隔(秒)
const int E_MAX_COUNT = 20;                 // 屏幕上最大敌机数量
const int E_HEALTH = 3;                     // 敌机生命值
const int E_SCORE_VALUE = 100;              // 击毁敌机得分

/*********************** 子弹设置 ***********************/
const float B_SPEED = 500;                  // 子弹速度(像素/秒)
const float B_RADIUS = 4;                    // 子弹宽度
const int B_DAMAGE = 1;                     // 子弹伤害值

const float TRACKING_BULLET_SPEED = 300.0f;        // 追踪子弹速度
const float TRACKING_MAX_ROTATION = 180.0f;         // 最大转向速度(度/秒)
const float TRACKING_ENEMY_PROBABILITY = 0.1f;      // 追踪敌机生成概率(10%)

/*********************** 游戏机制 ***********************/
const int G_MAX_SCORE = 999999;             // 最大分数限制
const float G_POWERUP_SPAWN_CHANCE = 0.1;   // 道具生成概率
const int G_LEVEL_UP_SCORE = 5000;          // 升级所需分数
const float G_DIFFICULTY_INCREASE = 0.1;    // 难度提升系数

/*********************** 激光设置 ***********************/
const float L_INITIAL_WIDTH = 2.0f;         // 激光初始宽度
const float L_MAX_WIDTH = 8.0f;            // 激光最大宽度
const float L_GROWTH_RATE = 8.0f;          // 激光增长速度(像素/秒)
const float L_HEIGHT = 2000.0f;                     // 激光高度
const float L_FIRE_COOLDOWN = 3.0f;        // 激光发射间隔(秒)



#endif // CONFIG_H