#define KeyHeld(b) input->buttons[b].isDown
#define KeyDown(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define KeyUp(b) (!input->buttons[b].isDown && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 90, arena_half_size_y = 45;
float player_half_size_x = 1.5, player_half_size_y = 8;
float ball_p_x, ball_p_y, ball_dp_x = 130, ball_dp_y, ball_half_size = 1;

int p1Score, p2Score;

bool GameOver = false;

internal void
DrawStage() {
	drawRect(90, 0, .5, 46, white);
	drawRect(-90, 0, .5, 46, white);
	drawRect(0, 46, 90.5, .5, white);
	drawRect(0, -46, 90.5, .5, white);
}
internal void
DrawStageWLines() {
	float startingY = -40;
	for (int i = 0; i < 5; i++) {
		drawRect(0, startingY, .5, 5 , white);
		startingY += 20;
	}

	drawRect(90, 0, .5, 46, white);
	drawRect(-90, 0, .5, 46, white);
	drawRect(0, 46, 90.5, .5, white);
	drawRect(0, -46, 90.5, .5, white);
}

internal void
simulate_player(float* p, float* dp, float ddp, float dt) {
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}

enum GameMode {
	Menu,
	GamePlay,
};

GameMode currGameMode;
int hotButton;
bool enemyIsAi;

internal void
winCondition() 
{
	if (enemyIsAi) {
		if (p2Score == 10) {
			clear_screen(black);
			DrawStage();
			drawText("YOU LOSE", -45, 20, 2, white);
			drawText("YOUR SCORE", -30, 1, 1, white);
			drawNumber(p1Score, 37, -2, 1.5, white);
			GameOver = true;
		}
		else {
			GameOver = false;
		}
	}
	else {
		if (p1Score == 10) {
			clear_screen(black);
			DrawStage();
			drawText("PLAYER", -35, 20, 2, white);
			drawNumber(1, 35, 14, 2.5, white);
			drawText("WINS", -20, 0, 2, white);
			GameOver = true;
		}
		else if (p2Score == 10) {
			clear_screen(black);
			DrawStage();
			drawText("PLAYER", -35, 20, 2, white);
			drawNumber(2, 40, 14, 2.5, white);
			drawText("WINS", -20, 0, 2, white);
			GameOver = true;
		}
		else {
			GameOver = false;
		}
	}

}
internal void
simulateGame(Input* input, float dt) {

	DrawStage();
	
	if (currGameMode == GamePlay) {

		winCondition();
		if (GameOver) return;

		drawRect(0, 0, arena_half_size_x, arena_half_size_y, black);	
		float player_1_ddp = 0.f;
		if (!enemyIsAi) {
			if (KeyHeld(Down)) player_1_ddp += 2000;
			if (KeyHeld(Up)) player_1_ddp -= 2000;
		}
		else {
			//if (ball_p_y > player_1_p + 2.f && ball_p_x > -10) player_1_ddp += 1300;
			//if (ball_p_y < player_1_p - 2.f && ball_p_x > -10) player_1_ddp -= 1300;
			float detectX = 50;
			float AiSpeed = 75;
			if (ball_dp_y > 20) {
				detectX = 50;
				AiSpeed = 100;
			}
			else {
				detectX = 46.5;
				AiSpeed = 75;
			}
			if (ball_p_x > detectX) {
				player_1_ddp = (ball_p_y - player_1_p) * AiSpeed;
			}
		}

		float player_2_ddp = 0.f;
		if (KeyHeld(W)) player_2_ddp += 2000;
		if (KeyHeld(S)) player_2_ddp -= 2000;

		simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
		simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);


		// Simulate Ball
		
		ball_p_x += ball_dp_x * dt;
		ball_p_y += ball_dp_y * dt;

		if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y)) {
			ball_p_x = 80 - player_half_size_x - ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * .75f;
		}
		else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y)) {
			ball_p_x = -80 + player_half_size_x + ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * .75f;
		}

		if (ball_p_y + ball_half_size > arena_half_size_y) {
			ball_p_y = arena_half_size_y - ball_half_size;
			ball_dp_y *= -1;
		}
		else if (ball_p_y - ball_half_size < -arena_half_size_y) {
			ball_p_y = -arena_half_size_y + ball_half_size;
			ball_dp_y *= -1;
		}

		if (ball_p_x + ball_half_size > arena_half_size_x) {
			ball_dp_x *= -1;
			ball_dp_y = 0;
			ball_p_x = 0;
			ball_p_y = 0;
			p1Score++;
		}
		else if (ball_p_x - ball_half_size < -arena_half_size_x) {
			ball_dp_x *= -1;
			ball_dp_y = 0;
			ball_p_x = 0;
			ball_p_y = 0;
			p2Score++;
		}

		drawNumber(p1Score, -10, 40, 1.f, white);
		drawNumber(p2Score, +10, 40, 1.f, white);

		drawRect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, white);

		drawRect(80, player_1_p, player_half_size_x, player_half_size_y, white);
		drawRect(-80, player_2_p, player_half_size_x, player_half_size_y, white);
		DrawStageWLines();
		
		
	}
	else {
		if (KeyDown(W) || KeyDown(S)) {
			hotButton = !hotButton;
		}
		if (KeyDown(Enter)) {
			currGameMode = GamePlay;
			enemyIsAi = hotButton ? 0 : 1;
		}
		if (hotButton == 0) {
			clear_screen(black);
			drawText("PONG", -30, 40, 3, white);
			drawRect(10, 0, 21, 6, green);
			drawRect(-10, -15, 20, 5, white);
			drawText("SINGLE PLAYER", -8.5, 1.5, .5, black);
			drawText("MULTI PLAYER", -27, -13, .5, black);
			
			DrawStage();
			
		}
		else {
			clear_screen(black);
			drawText("PONG", -30, 40, 3, white);
			drawRect(10, 0, 20, 5, white);
			drawRect(-10, -15, 21, 6, green);
			drawText("SINGLE PLAYER", -8.5, 1.5, .5, black);
			drawText("MULTI PLAYER", -27, -13, .5, black);
			
			DrawStage();
			
		}
		
	}
}