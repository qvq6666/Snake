#include "raylib.h"
#include <random>
#include <fstream>
std::mt19937 rng(std::random_device{}());
//全局变量
int game_start = 0;

//本游戏全部需要用到的汉字Unicode码点
int cnCodePoints[] = {
    0x8D2A,0x5403,0x86C7,//贪吃蛇
    0x6309,0x7A7A,0x683C,0x952E,0x5F00,0x59CB,0x6E38,0x620F,//按空格键开始游戏
    0x6E38,0x620F,0x7ED3,0x675F,//游戏结束
    0x6309,0x0051,0x91CD,0x65B0,0x5F00,0x59CB,//按Q重新开始
    0x6309,0x0045,0x0073,0x0063,0x9000,0x51FA,0x6E38,0x620F,//按ESC退出游戏
    0x6700,0x9AD8,0x5206,0x6570,//最高分数
    31616,21333,27169,24335,25353,39,49,39,44,26222,36890,27169,24335,25353,39,50,39,44,22256,38590,27169,24335,25353,39,51,39,//简单模式按'1',普通模式按'2',困难模式按'3'
    24403,21069,27169,24335,58//当前模式:
};

int cnCodeCount = sizeof(cnCodePoints)/sizeof(int);

int high_Factions(){
    //获取Fraction_high.txt的数据
    int high = 0;
    std::ifstream fin("Fraction_high.txt");
    if (fin.is_open()){
        fin>>high;
    }
    fin.close();
    return high;
}

int new_high(int new_high){
    //new_high参数用来跟Fraction_high.txt的数据比较，如果大替换
    int high = high_Factions();
    if (high < new_high){
        std::ofstream fin("Fraction_high.txt");
        if (fin.is_open()){
            fin<<new_high;
        }
        fin.close();
    }
    return 0;
}

int random(int a,int x){
    //随机数
    std::uniform_int_distribution<int>dist(a,x);
    return dist(rng);
}

void SpawnFood(int &eat_gx, int &eat_gy, int snake_gx, int snake_gy, int bodyGx[], int bodyGy[], int snakeLen){
    int new_gx, new_gy;
    while(true){
        new_gx = random(0,19);
        new_gy = random(0,14);
        
        int dx = new_gx - snake_gx;
        int dy = new_gy - snake_gy;
        if(dx < 0) dx = -dx;
        if(dy < 0) dy = -dy;
        int dist = dx + dy;
        
        bool posValid = true;
        for(int i = 0; i < snakeLen; i++){
            if(new_gx == bodyGx[i] && new_gy == bodyGy[i]){
                posValid = false;
                break;
            }
        }
        
        if(posValid && dist >= 6){
            eat_gx = new_gx;
            eat_gy = new_gy;
            break;
        }
    }
}

int main(){
    //======初始化======//
    int W = 800;
    int H = 600;
    
    InitWindow(W,H,"贪吃蛇");
    SetExitKey(KEY_NULL);
    SetTargetFPS(70);
    SetTraceLogLevel(LOG_NONE);
    RenderTexture2D freeze_frame = LoadRenderTexture(W,H);
    //======加载字体======//
    Font cnFont = LoadFontEx("SourceHanSansSC-Heavy.otf",600,cnCodePoints,cnCodeCount);
    SetTextureFilter(cnFont.texture,TEXTURE_FILTER_BILINEAR);
    //============｜｜游戏主区域｜｜============//
    int game_difficulty = 1;//1=简单，2=普通，3=困难
    //GAME_STAR标签用来重新再判断场景
    GAME_STAR:
    int high = high_Factions();//high用来显现最高分数
    int score = 0;//分数
    //======game_start是游戏状态，用来改变场景======//
    //======game_start = 0 是主界面======//
    //======game_start = 1 是游戏主场景======//
    //======game_start = 2 是游戏失败界面======//
    if (game_start==0){
        ClearBackground(BLACK);
        while (!WindowShouldClose()){
            if (IsKeyPressed(KEY_ONE)){
                game_difficulty = 1;
            }
            if (IsKeyPressed(KEY_TWO)){
                game_difficulty = 2;
            }
            if (IsKeyPressed(KEY_THREE)){
                game_difficulty = 3;
            }
            if (IsKeyPressed(KEY_SPACE)){
                game_start = 1;
                goto GAME_STAR;
            }
            BeginDrawing();
            ClearBackground(BLACK);
            //调用汉字Unicode码点：贪吃蛇
            int titleSnake[] = {0x8D2A,0x5403,0x86C7};
            int titleCnt = sizeof(titleSnake)/sizeof(int);
            DrawTextCodepoints(cnFont,titleSnake, titleCnt,{210,100},160, 2, WHITE);
            //调用汉字Unicode码点：//按空格键开始游戏
            int startTip[] = {0x6309,0x7A7A,0x683C,0x952E,0x5F00,0x59CB,0x6E38,0x620F};
            int startTipCnt = sizeof(startTip)/sizeof(int);
            DrawTextCodepoints(cnFont,startTip,startTipCnt,{215,300},60,2,BLUE);
            //调用汉字Unicode码点:简单模式按'1',普通模式按'2',困难模式按'3'
            int start_difficulty_Char[] = {31616,21333,27169,24335,25353,39,49,39,44,26222,36890,27169,24335,25353,39,50,39,44,22256,38590,27169,24335,25353,39,51,39};
            int start_temp = sizeof(start_difficulty_Char)/sizeof(int);
            DrawTextCodepoints(cnFont,start_difficulty_Char,start_temp,{160,450},30,2,YELLOW);
            //调用汉字Unicode码点:当前模式:
            int Curr_Mod_Char[] = {24403,21069,27169,24335,58};
            int Curr_Mod_temp = sizeof(Curr_Mod_Char)/sizeof(int);
            DrawTextCodepoints(cnFont,Curr_Mod_Char,Curr_Mod_temp,{20,20},50,2,WHITE);
            if (game_difficulty == 1){
                int Mod_Char[] = {31616,21333};
                int Mod_temp = sizeof(Mod_Char)/sizeof(int);
                DrawTextCodepoints(cnFont,Mod_Char,Mod_temp,{185,20},50,2,GREEN);
            }
            else if (game_difficulty == 2){
                int Mod_Char[] = {26222,36890};
                int Mod_temp = sizeof(Mod_Char)/sizeof(int);
                DrawTextCodepoints(cnFont,Mod_Char,Mod_temp,{185,20},50,2,YELLOW);
            }
            else{
                int Mod_Char[] = {22256,38590};
                int Mod_temp = sizeof(Mod_Char)/sizeof(int);
                DrawTextCodepoints(cnFont,Mod_Char,Mod_temp,{185,20},50,2,RED);
            }
            EndDrawing();
        }
    }
    else if (game_start==1){
        const int cell = 40;//cell方格大小
        const int MAX_BODY = 300;//贪吃蛇长度上限
        int bodyGx[MAX_BODY];//贪吃蛇蛇身蛇尾长度x
        int bodyGy[MAX_BODY];//贪吃蛇蛇身蛇尾长度y
        int snakeLen = 1;//贪吃蛇长度
        bodyGx[0] = 2;//设置贪吃蛇蛇身蛇尾位置x
        bodyGy[0] = 3;//设置贪吃蛇蛇身蛇尾位置y
        /*gx是蛇头
        这里gx = bodyGx[0],gy = bodyGx[0]
        意思是说bodyGx[0]，bodyGx[0]，代表的是蛇头坐标*/
        int gx = bodyGx[0];
        int gy = bodyGy[0];
        //dirX是x的方向，dirY是y的方向
        int dirX = 1;
        int dirY = 0;
        //食物方块位置//
        int eat_gx = 9;
        int eat_gy = 9;
        bool justAte = false;
        double stop_time = 0.1;
        if (game_difficulty == 1){//如果后面要在游戏失败后也能回到主见面要改这个和else if(game_start == 1)
            stop_time = 0.2;
        }
        else if (game_difficulty == 2){
            stop_time = 0.17;
        }
        else{
            stop_time = 0.1;
        }
        while (!WindowShouldClose()){
            //记时器timer//
            static double timer = 0;
            timer += GetFrameTime();
            //每0.2秒移动一格
            if (timer > stop_time){
                timer = 0;
                
                int tailGx = bodyGx[snakeLen - 1];
                int tailGy = bodyGy[snakeLen - 1];
                //所有蛇身跟上蛇头
                for(int i = snakeLen - 1; i >= 1; i--){
                    bodyGx[i] = bodyGx[i-1];
                    bodyGy[i] = bodyGy[i-1];
                }
                

                //移动逻辑
                bodyGx[0] += dirX;
                bodyGy[0] += dirY;
                gx = bodyGx[0];
                gy = bodyGy[0];
                //是否吃到食物？
                if(justAte){
                    snakeLen++;
                    bodyGx[snakeLen - 1] = tailGx;
                    bodyGy[snakeLen - 1] = tailGy;
                    justAte = false;
                    score++;
                }
            }
            //bool类型变量hitEdge判断是否游戏失败
            bool hitEdge = gx < 0 || gx >= 20 || gy < 0 || gy >= 15;
            //for循环用来检测蛇头有没有碰到蛇身
            for (int i = 1; i < snakeLen; i++){
                if (bodyGx[0] == bodyGx[i] && bodyGy[0] == bodyGy[i]){
                    hitEdge = true;
                    break;
                }
            }
        
            if (hitEdge){
                BeginTextureMode(freeze_frame);
                EndTextureMode();
                new_high(score);
                game_start = 2;
                break;
            }
            
            // ============按键检测============ //

            //这里的移动按键都有一个如果你按了一个与你现方向相反方向的按键死亡判断
            if (IsKeyPressed(KEY_W)){ 
                if (dirY == 1 && snakeLen > 1){
                    BeginTextureMode(freeze_frame);
                    EndTextureMode();
                    new_high(score);
                    game_start = 2;
                    break;
                }
                dirX = 0; dirY = -1; 
            }
            if (IsKeyPressed(KEY_S)){
                if (dirY == -1 && snakeLen > 1){
                    BeginTextureMode(freeze_frame);
                    EndTextureMode();
                    new_high(score);
                    game_start = 2;
                    break;
                }
                dirX = 0; dirY = 1; 
            }
            if (IsKeyPressed(KEY_A)){
                if (dirX == 1 && snakeLen > 1){
                    BeginTextureMode(freeze_frame);
                    EndTextureMode();
                    new_high(score);
                    game_start = 2;
                    break;
                }
                dirX = -1; dirY = 0; 
            }
            if (IsKeyPressed(KEY_D)){
                if (dirX == -1 && snakeLen > 1){
                    BeginTextureMode(freeze_frame);
                    EndTextureMode();
                    new_high(score);
                    game_start = 2;
                    break;
                }
                dirX = 1; dirY = 0;
            }
            if (IsKeyPressed(KEY_R)){
                std::ofstream fin("Fraction_high.txt");
                if (fin.is_open()){
                    fin<<0;
                }
                fin.close();
                high = 0;
            }
            
            
            // ============绘制区域============ //
            BeginDrawing();
            ClearBackground(BLACK);
            // 绘制方格
            for (int x = 0; x <= W; x += cell){
                DrawLine(x,0,x,H,DARKGRAY);
            }
            for (int y = 0;y <= H;y += cell){
                DrawLine(0,y,W,y,DARKGRAY);
            }
            if (gx == eat_gx && gy == eat_gy){
                SpawnFood(eat_gx,eat_gy,gx,gy,bodyGx,bodyGy,snakeLen);
                justAte = true;
            }
            // 绘制方块
            for(int i = 1;i < snakeLen;i++){
                Rectangle seg = {
                    (float)bodyGx[i] * cell,
                    (float)bodyGy[i] * cell,
                    (float)cell,
                    (float)cell
                };
                DrawRectangleRec(seg,GREEN);
            }
            Rectangle rec = {
                (float)gx * cell,
                (float)gy * cell,
                (float)cell,
                (float)cell
            };
            DrawRectangleRec(rec,RED);
            Rectangle eat_rec = {
                (float)eat_gx * cell,
                (float)eat_gy * cell,
                (float)cell,
                (float)cell
            };
            DrawRectangleRec(eat_rec,YELLOW);
            // 绘制文字
            if (score > high){
                int fenShu[] = {0x5206,0x6570};//分数
                int fsCnt = sizeof(fenShu)/sizeof(int);
                DrawTextCodepoints(cnFont,fenShu,fsCnt,{5,2},40,1,GOLD);
                DrawText(TextFormat(":%d", score),63,10,30,GOLD);
            }
            else{
                int fenShu[] = {0x5206,0x6570};//分数
                int fsCnt = sizeof(fenShu)/sizeof(int);
                DrawTextCodepoints(cnFont,fenShu,fsCnt,{5,2},40,1,BLUE);
                DrawText(TextFormat(":%d", score),63,10,30,BLUE);
            }

            int zuiGao[] = {0x6700,0x9AD8,0x5206,0x6570};//最高分数
            int zgCnt = sizeof(zuiGao)/sizeof(int);
            DrawTextCodepoints(cnFont,zuiGao,zgCnt,{130,2},40,1,BLUE);
            DrawText(TextFormat(":%d",high),245,10,30,BLUE);
            EndDrawing();
        }
    }
    if (game_start==2){
        while (!WindowShouldClose()){
            if (IsKeyPressed(KEY_Q)){
                game_start = 1;
                goto GAME_STAR;
            }
            else if (IsKeyDown(KEY_ESCAPE)){
                break;
            }
            BeginDrawing();
            DrawTextureRec(freeze_frame.texture, {0,0,800,-600}, {0,0}, (Color){255,255,255,160});
            int gameOver[] = {0x6E38,0x620F,0x7ED3,0x675F};//游戏结束
            int goCnt = sizeof(gameOver)/sizeof(int);
            DrawTextCodepoints(cnFont,gameOver,goCnt,{200,100},160,2,MAROON);

            int restartTip[] = {0x6309,0x0051,0x91CD,0x65B0,0x5F00,0x59CB};//按Q重新开始
            int rstCnt = sizeof(restartTip)/sizeof(int);
            DrawTextCodepoints(cnFont,restartTip,rstCnt,{200,250},30,2,WHITE);

            int exitTip[] = {0x6309,0x0045,0x0073,0x0063,0x9000,0x51FA,0x6E38,0x620F};//按ESC退出游戏
            int exitCnt = sizeof(exitTip)/sizeof(int);
            DrawTextCodepoints(cnFont,exitTip,exitCnt,{200,300},30,2,WHITE);
            EndDrawing();
        }
    }
    new_high(score);
    UnloadRenderTexture(freeze_frame);
    UnloadFont(cnFont);
    CloseWindow();
    return 0;
}
