import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "../snake"))

import pytest
from snake import Snake
from utils import MockWindow


def test_snake_initial_position():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    assert snake.get_snake_head_x() == 30
    assert snake.get_snake_head_y() == 9


def test_grow_snake():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    old_length = len(snake.snake_body)
    snake.grow_snake()

    assert len(snake.snake_body) == old_length + 1


def test_quit_key():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    win.inputs = [ord("q")]
    snake.move_position()

    assert snake.is_game_over() is True


def test_jump_top_wall():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    snake.snake_position = [10, 19]  # board_height - 1
    snake.key = snake.LEFT_KEY

    snake.jump_snake_position()

    assert snake.snake_position[1] == 1


def test_jump_top_wall_right():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    snake.snake_position = [10, 19]
    snake.key = snake.RIGHT_KEY

    snake.jump_snake_position()

    assert snake.snake_position[1] == 1


def test_jump_bottom_wall():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    snake.snake_position = [10, 0]
    snake.key = snake.RIGHT_KEY

    snake.jump_snake_position()

    assert snake.snake_position[1] == 18  # board_height - 2


def test_jump_bottom_wall_left():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    snake.snake_position = [10, 0]
    snake.key = snake.LEFT_KEY

    snake.jump_snake_position()

    assert snake.snake_position[1] == 18


def test_jump_left_wall():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    snake.snake_position = [0, 10]
    snake.key = snake.UP_KEY

    snake.jump_snake_position()

    assert snake.snake_position[0] == 58  # board_width - 2


def test_jump_left_wall_down():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    snake.snake_position = [0, 10]
    snake.key = snake.DOWN_KEY

    snake.jump_snake_position()

    assert snake.snake_position[0] == 58


def test_jump_right_wall():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    snake.snake_position = [59, 10]
    snake.key = snake.DOWN_KEY

    snake.jump_snake_position()

    assert snake.snake_position[0] == 1


def test_jump_right_wall_up():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    snake.snake_position = [59, 10]
    snake.key = snake.UP_KEY

    snake.jump_snake_position()

    assert snake.snake_position[0] == 1


def test_back_on_self_up_down():
    snake = Snake(MockWindow(), 60, 20)
    snake.last_valid_key = snake.UP_KEY
    snake.key = snake.DOWN_KEY

    assert snake.did_go_back_on_self() is True


def test_back_on_self_down_up():
    snake = Snake(MockWindow(), 60, 20)
    snake.last_valid_key = snake.DOWN_KEY
    snake.key = snake.UP_KEY

    assert snake.did_go_back_on_self() is True


def test_back_on_self_left_right():
    snake = Snake(MockWindow(), 60, 20)
    snake.last_valid_key = snake.LEFT_KEY
    snake.key = snake.RIGHT_KEY

    assert snake.did_go_back_on_self() is True


def test_back_on_self_right_left():
    snake = Snake(MockWindow(), 60, 20)
    snake.last_valid_key = snake.RIGHT_KEY
    snake.key = snake.LEFT_KEY

    assert snake.did_go_back_on_self() is True


def test_back_on_self_false_case():
    snake = Snake(MockWindow(), 60, 20)
    snake.last_valid_key = snake.UP_KEY
    snake.key = snake.RIGHT_KEY

    assert snake.did_go_back_on_self() is False


def test_tail_collision_ignored_when_reverse():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    snake.key = snake.UP_KEY
    snake.did_go_back_on_self = lambda: True
    snake.snake_position = [10, 10]

    snake.check_tail_collision()

    assert snake.game_over is False


def test_check_tail_collision_true():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    snake.key = snake.UP_KEY
    snake.did_go_back_on_self = lambda: False
    snake.snake_position = [10, 10]
    win.inch = lambda y, x: ord(snake.SEGMENT_CHAR)

    snake.check_tail_collision()

    assert snake.game_over is True


@pytest.mark.parametrize("key", [
    Snake.UP_KEY,
    Snake.DOWN_KEY,
    Snake.LEFT_KEY,
    Snake.RIGHT_KEY
])
def test_check_tail_collision_all_keys(key):
    win = MockWindow()
    win.inch = lambda y, x: ord("#")
    snake = Snake(win, 60, 20)

    snake.key = key
    snake.did_go_back_on_self = lambda: False
    snake.snake_position = [10, 10]

    snake.check_tail_collision()

    assert snake.game_over is True


def test_set_snake_head_x_y():
    snake = Snake(MockWindow(), 60, 20)
    snake.set_snake_head_x(15)
    snake.set_snake_head_y(7)

    assert snake.snake_position == [15, 7]


def test_move_up():
    snake = Snake(MockWindow(), 60, 20)
    snake.snake_position = [10, 10]
    snake.move_up()

    assert snake.snake_position == [10, 9]


def test_move_down():
    snake = Snake(MockWindow(), 60, 20)
    snake.snake_position = [10, 10]
    snake.move_down()

    assert snake.snake_position == [10, 11]


def test_move_left():
    snake = Snake(MockWindow(), 60, 20)
    snake.snake_position = [10, 10]
    snake.move_left()

    assert snake.snake_position == [9, 10]


def test_display_snake_erases_tail():
    win = MockWindow()
    snake = Snake(win, 60, 20)

    snake.snake_body = [[10, 10], [9, 10]]
    snake.snake_position = [11, 10]

    snake.display_snake()

    # Proverava da je glava nacrtana na novoj poziciji
    assert (11, 10) in win.chars


# ─── move_position testovi ──────────────────────────────────────────────────────

def test_move_right_via_move_position():
    snake = Snake(MockWindow(), 60, 20)
    snake.key = snake.RIGHT_KEY
    snake.last_valid_key = snake.UP_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [11, 10]


def test_quit_key_sets_game_over():
    snake = Snake(MockWindow(), 60, 20)
    snake.key = snake.QUIT_KEY
    snake.move_position()

    assert snake.game_over is True


def test_invalid_input_follows_last_direction():
    snake = Snake(MockWindow(), 60, 20)
    snake.key = 9999
    snake.last_valid_key = snake.UP_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [10, 9]


def test_pause_key_does_nothing():
    snake = Snake(MockWindow(), 60, 20)
    snake.key = snake.PAUSE_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [10, 10]


def test_fallback_left():
    snake = Snake(MockWindow(), 60, 20)
    snake.key = 9999
    snake.last_valid_key = snake.LEFT_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [9, 10]


def test_fallback_down():
    snake = Snake(MockWindow(), 60, 20)
    snake.key = 9999
    snake.last_valid_key = snake.DOWN_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [10, 11]


def test_fallback_right():
    snake = Snake(MockWindow(), 60, 20)
    snake.key = 9999
    snake.last_valid_key = snake.RIGHT_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [11, 10]


def test_move_up_conflict():
    """UP pritisnut dok ide DOWN → nastavlja DOWN"""
    snake = Snake(MockWindow(), 60, 20)
    snake.key = snake.UP_KEY
    snake.last_valid_key = snake.DOWN_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [10, 11]


def test_move_left_conflict():
    """LEFT pritisnut dok ide RIGHT → nastavlja RIGHT"""
    snake = Snake(MockWindow(), 60, 20)
    snake.key = snake.LEFT_KEY
    snake.last_valid_key = snake.RIGHT_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [11, 10]


def test_move_down_conflict():
    """DOWN pritisnut dok ide UP → nastavlja UP"""
    snake = Snake(MockWindow(), 60, 20)
    snake.key = snake.DOWN_KEY
    snake.last_valid_key = snake.UP_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [10, 9]


def test_move_right_conflict():
    """RIGHT pritisnut dok ide LEFT → nastavlja LEFT"""
    snake = Snake(MockWindow(), 60, 20)
    snake.key = snake.RIGHT_KEY
    snake.last_valid_key = snake.LEFT_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [9, 10]


def test_move_up_no_conflict():
    snake = Snake(MockWindow(), 60, 20)
    snake.key = snake.UP_KEY
    snake.last_valid_key = snake.LEFT_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [10, 9]
    assert snake.last_valid_key == snake.UP_KEY


def test_move_left_no_conflict():
    snake = Snake(MockWindow(), 60, 20)
    snake.key = snake.LEFT_KEY
    snake.last_valid_key = snake.UP_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [9, 10]
    assert snake.last_valid_key == snake.LEFT_KEY


def test_move_down_no_conflict():
    snake = Snake(MockWindow(), 60, 20)
    snake.key = snake.DOWN_KEY
    snake.last_valid_key = snake.LEFT_KEY
    snake.snake_position = [10, 10]

    snake.move_position()

    assert snake.snake_position == [10, 11]
    assert snake.last_valid_key == snake.DOWN_KEY
