import time
import random
import board
import displayio
import framebufferio
import rgbmatrix

class PongGame:
    def __init__(self):
        # Release any resources currently in use
        displayio.release_displays()
        
        # Configuration for display size and colors
        self.WIDTH = 64
        self.HEIGHT = 32
        self.BITMAP_COLORS = 256
        
        # Setup the RGB matrix display
        self.matrix = rgbmatrix.RGBMatrix(
            width=self.WIDTH,
            height=self.HEIGHT,
            bit_depth=6,
            rgb_pins=[
                board.MTX_R1, board.MTX_G1, board.MTX_B1,
                board.MTX_R2, board.MTX_G2, board.MTX_B2
            ],
            addr_pins=[
                board.MTX_ADDRA, board.MTX_ADDRB, board.MTX_ADDRC, board.MTX_ADDRD
            ],
            clock_pin=board.MTX_CLK,
            latch_pin=board.MTX_LAT,
            output_enable_pin=board.MTX_OE,
            tile=1,
            serpentine=True,
            doublebuffer=True,
        )
        self.display = framebufferio.FramebufferDisplay(self.matrix, auto_refresh=False)
        
        # Create a display group and attach a TileGrid for the bitmap
        self.group = displayio.Group()
        self.display.root_group = self.group
        
        # Create bitmap and palette
        self.bitmap = displayio.Bitmap(self.WIDTH, self.HEIGHT, self.BITMAP_COLORS)
        self.palette = displayio.Palette(self.BITMAP_COLORS)
        self.palette[0] = 0x000000  # Black background
        self.palette[4] = 0x0000FF  # Blue border
        
        # Define bright colors for ball and players
        self.BRIGHT_COLORS = [
            0xFF0000,  # Red
            0xFFFF00,  # Yellow
            0x00FF00,  # Green
            0x00FFFF,  # Cyan
            0xFF00FF,  # Magenta
            0xFFA500,  # Orange
            0xFFFFFF,  # White
        ]
        # Ball color settings
        self.ball_colors = self.BRIGHT_COLORS.copy()
        self.ball_color_index = 0
        self.palette[1] = self.ball_colors[self.ball_color_index]
        
        # Player (paddle) color settings
        self.player_colors = self.BRIGHT_COLORS.copy()
        self.paddle1_color_index = 0
        self.palette[2] = self.player_colors[self.paddle1_color_index]
        self.paddle2_color_index = 0
        self.palette[3] = self.player_colors[self.paddle2_color_index]
        
        # Attach the tile grid to the group
        self.tile_grid = displayio.TileGrid(self.bitmap, pixel_shader=self.palette)
        self.group.append(self.tile_grid)
        
        # Game variables
        self.ball_x = self.WIDTH // 2
        self.ball_y = self.HEIGHT // 2
        self.ball_speed = 1.6
        self.ball_dx = random.choice([-self.ball_speed, self.ball_speed])
        self.ball_dy = random.choice([-self.ball_speed, self.ball_speed])
        
        # Paddle settings
        self.PADDLE_HEIGHT = 5
        self.PADDLE_WIDTH = 2
        self.PADDLE_OFFSET = 6  # Inset from the edge
        self.paddle1_x = self.PADDLE_OFFSET
        self.paddle2_x = self.WIDTH - self.PADDLE_OFFSET - self.PADDLE_WIDTH
        self.paddle1_y = (self.HEIGHT - self.PADDLE_HEIGHT) // 2
        self.paddle2_y = (self.HEIGHT - self.PADDLE_HEIGHT) // 2
        
        # Which paddle is allowed to move
        self.paddle1_can_move = True
        self.paddle2_can_move = False  # Only the paddle the ball is moving toward can move
        
        # Player speed settings (dynamic abilities)
        self.MAX_PLAYER_SPEED = 4.3  # Maximum paddle speed
        self.MIN_PLAYER_SPEED = 1    # Minimum paddle speed
        # Dynamic speed starts at the minimum and increases with consecutive hits
        self.paddle1_speed = self.MIN_PLAYER_SPEED
        self.paddle2_speed = self.MIN_PLAYER_SPEED
        self.paddle1_hit_count = 0
        self.paddle2_hit_count = 0
        
        # Draw the static border once
        self.draw_border()
        self.update_display()
    
    def draw_border(self):
        """Draw static borders around the display edges."""
        for x in range(self.WIDTH):
            self.bitmap[x, 0] = 4               # Top border
            self.bitmap[x, self.HEIGHT - 1] = 4   # Bottom border

    def clear_paddles(self):
        """Clear the paddle areas by setting them to the background color."""
        for y in range(self.HEIGHT):
            for x in range(self.PADDLE_WIDTH):
                self.bitmap[self.paddle1_x + x, y] = 0
                self.bitmap[self.paddle2_x + x, y] = 0

    def draw_paddles(self):
        """Draw both paddles on the screen."""
        self.clear_paddles()
        # Draw paddle 1
        for y in range(int(self.paddle1_y), int(self.paddle1_y + self.PADDLE_HEIGHT)):
            if 0 <= y < self.HEIGHT:
                for x in range(self.PADDLE_WIDTH):
                    self.bitmap[self.paddle1_x + x, y] = 2
        # Draw paddle 2
        for y in range(int(self.paddle2_y), int(self.paddle2_y + self.PADDLE_HEIGHT)):
            if 0 <= y < self.HEIGHT:
                for x in range(self.PADDLE_WIDTH):
                    self.bitmap[self.paddle2_x + x, y] = 3

    def draw_ball(self):
        """Update the ball's position, handle collisions, and draw it."""
        # Erase previous ball pixel
        prev_x, prev_y = int(self.ball_x), int(self.ball_y)
        if 0 <= prev_x < self.WIDTH and 0 <= prev_y < self.HEIGHT:
            self.bitmap[prev_x, prev_y] = 0

        # Update ball position based on its velocity
        self.ball_x += self.ball_dx
        self.ball_y += self.ball_dy

        # Bounce off the top and bottom borders (keeping room for the border)
        if self.ball_y <= 1:
            self.ball_dy *= -1
            self.ball_y = 1
        elif self.ball_y >= self.HEIGHT - 2:
            self.ball_dy *= -1
            self.ball_y = self.HEIGHT - 2

        # Handle collisions with paddles and scoring
        if self.ball_dx < 0 and self.ball_x <= self.paddle1_x + self.PADDLE_WIDTH:
            # Left paddle collision (player 1)
            if self.paddle1_y <= self.ball_y < self.paddle1_y + self.PADDLE_HEIGHT:
                self.ball_dx *= -1
                self.ball_x = self.paddle1_x + self.PADDLE_WIDTH
                # Increase player 1's dynamic ability
                self.paddle1_hit_count += 1
                self.paddle1_speed = min(self.MAX_PLAYER_SPEED, self.MIN_PLAYER_SPEED + self.paddle1_hit_count * 0.2)
                self.change_ball_properties()
                self.paddle1_can_move = False
                self.paddle2_can_move = True
            elif self.ball_x <= self.paddle1_x:
                # Player 2 scores
                self.reset_ball(winner=2)
        elif self.ball_dx > 0 and self.ball_x >= self.paddle2_x:
            # Right paddle collision (player 2)
            if self.paddle2_y <= self.ball_y < self.paddle2_y + self.PADDLE_HEIGHT:
                self.ball_dx *= -1
                self.ball_x = self.paddle2_x
                # Increase player 2's dynamic ability
                self.paddle2_hit_count += 1
                self.paddle2_speed = min(self.MAX_PLAYER_SPEED, self.MIN_PLAYER_SPEED + self.paddle2_hit_count * 0.2)
                self.change_ball_properties()
                self.paddle1_can_move = True
                self.paddle2_can_move = False
            elif self.ball_x >= self.paddle2_x + self.PADDLE_WIDTH:
                # Player 1 scores
                self.reset_ball(winner=1)

        # Draw the ball at its new position
        current_x, current_y = int(self.ball_x), int(self.ball_y)
        if 0 <= current_x < self.WIDTH and 0 <= current_y < self.HEIGHT:
            self.bitmap[current_x, current_y] = 1

    def change_ball_properties(self):
        """Increase ball speed with a slight random factor, update direction, and cycle its color."""
        if self.ball_speed < 3.8:
            # Add a base increase with a random jitter
            self.ball_speed += 0.1 + random.uniform(-0.05, 0.05)
            self.ball_speed = max(1.0, min(self.ball_speed, 3.8))
        # Preserve the horizontal direction while updating the speed
        self.ball_dx = self.ball_speed if self.ball_dx > 0 else -self.ball_speed
        # Update vertical speed with a random choice for bounce effect
        self.ball_dy = random.choice([-self.ball_speed, self.ball_speed])
        # Cycle the ball color
        self.ball_color_index = (self.ball_color_index + 1) % len(self.ball_colors)
        self.palette[1] = self.ball_colors[self.ball_color_index]

    def reset_ball(self, winner):
        """Reset ball position and update game state after a score."""
        # Erase the current ball position
        prev_x, prev_y = int(self.ball_x), int(self.ball_y)
        if 0 <= prev_x < self.WIDTH and 0 <= prev_y < self.HEIGHT:
            self.bitmap[prev_x, prev_y] = 0

        # Reset ball position and speed
        self.ball_x = self.WIDTH // 2
        self.ball_y = self.HEIGHT // 2
        self.ball_speed = 1.0
        self.ball_dx = random.choice([-self.ball_speed, self.ball_speed])
        self.ball_dy = random.choice([-self.ball_speed, self.ball_speed])

        # Determine which paddle is allowed to move based on ball direction
        if self.ball_dx < 0:
            self.paddle1_can_move = True
            self.paddle2_can_move = False
        else:
            self.paddle1_can_move = False
            self.paddle2_can_move = True

        # Reset the dynamic ability for the paddle that lost
        if winner == 1:
            # Player 1 scored: reset player 2's ability and update player 1's color as a cue
            self.paddle2_hit_count = 0
            self.paddle2_speed = self.MIN_PLAYER_SPEED
            self.paddle1_color_index = (self.paddle1_color_index + 1) % len(self.player_colors)
            self.palette[2] = self.player_colors[self.paddle1_color_index]
        elif winner == 2:
            # Player 2 scored: reset player 1's ability and update player 2's color as a cue
            self.paddle1_hit_count = 0
            self.paddle1_speed = self.MIN_PLAYER_SPEED
            self.paddle2_color_index = (self.paddle2_color_index + 1) % len(self.player_colors)
            self.palette[3] = self.player_colors[self.paddle2_color_index]

    def ai_move_paddles(self):
        """Move the allowed paddle toward the ball using dynamic speeds and a slight random jitter."""
        # Paddle 1 movement (if allowed)
        if self.paddle1_can_move:
            jitter = random.uniform(-0.1, 0.1)
            if self.paddle1_y + self.PADDLE_HEIGHT / 2 < self.ball_y:
                self.paddle1_y += self.paddle1_speed + jitter
            elif self.paddle1_y + self.PADDLE_HEIGHT / 2 > self.ball_y:
                self.paddle1_y -= self.paddle1_speed + jitter
            self.paddle1_y = max(1, min(self.paddle1_y, self.HEIGHT - self.PADDLE_HEIGHT - 1))
        
        # Paddle 2 movement (if allowed)
        if self.paddle2_can_move:
            jitter = random.uniform(-0.1, 0.1)
            if self.paddle2_y + self.PADDLE_HEIGHT / 2 < self.ball_y:
                self.paddle2_y += self.paddle2_speed + jitter
            elif self.paddle2_y + self.PADDLE_HEIGHT / 2 > self.ball_y:
                self.paddle2_y -= self.paddle2_speed + jitter
            self.paddle2_y = max(1, min(self.paddle2_y, self.HEIGHT - self.PADDLE_HEIGHT - 1))

    def update_display(self):
        """Refresh the display to show the updated frame."""
        self.display.refresh(minimum_frames_per_second=60)

    def run(self):
        """Main game loop."""
        try:
            while True:
                self.ai_move_paddles()
                self.draw_paddles()
                self.draw_ball()
                self.update_display()
                time.sleep(0.02)  # Approximately 50 FPS
        except KeyboardInterrupt:
            print("Game terminated.")

if __name__ == "__main__":
    game = PongGame()
    game.run()
