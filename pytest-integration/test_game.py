import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "../snake"))

import pytest
from unittest.mock import patch
from utils import MockWindow


def make_game(mode=True):
    """Kreira Game instancu bez pravog curses terminala."""
    mock_win = MockWindow()
    with patch("curses.newwin", return_value=mock_win):
        from game import Game
        return Game(mode=mode)


def test_initial_score():
    game = make_game()
    assert game.get_game_score() == 0


def test_wall_mode_true():
    game = make_game(mode=True)
    assert game.pass_through_walls is True


def test_wall_mode_false():
    game = make_game(mode=False)
    assert game.pass_through_walls is False


def test_is_game_over_returns_false_initially():
    game = make_game()
    assert game.is_game_over() is False


def test_set_game_over():
    game = make_game()
    game.snake.is_game_over = lambda: True
    game.set_game_over()

    assert game.game_over is True


def test_check_fruit_collision():
    game = make_game()

    game.snake.get_snake_head_x = lambda: 5
    game.snake.get_snake_head_y = lambda: 5
    game.board.get_fruit_x = lambda: 5
    game.board.get_fruit_y = lambda: 5
    game.fruit_eaten = 0
    game.score = 0

    game.check_fruit_collision()

    assert game.score == game.SCORE_INCREASE
    assert game.fruit_eaten == 1


def test_increase_game_speed_on_even_fruit():
    game = make_game()
    game.current_game_speed = 100
    game.MAX_GAME_SPEED = 90
    game.fruit_eaten = 2

    game.increase_game_speed()

    assert game.current_game_speed == 99


def test_increase_game_speed_on_odd_fruit():
    """Na neparnom broju voca brzina se ne menja."""
    game = make_game()
    game.current_game_speed = 100
    game.fruit_eaten = 1

    game.increase_game_speed()

    assert game.current_game_speed == 100


def test_increase_game_speed_at_max():
    """Brzina se ne povecava ispod MAX_GAME_SPEED."""
    game = make_game()
    game.current_game_speed = game.MAX_GAME_SPEED
    game.fruit_eaten = 2

    game.increase_game_speed()

    assert game.current_game_speed == game.MAX_GAME_SPEED


def test_game_over_if_wall_hit_left():
    game = make_game()
    game.snake.get_snake_head_x = lambda: 0
    game.snake.get_snake_head_y = lambda: 5
    game.game_over = False

    game.game_over_if_wall_hit()

    assert game.game_over is True


def test_game_over_if_wall_hit_top():
    game = make_game()
    game.snake.get_snake_head_x = lambda: 10
    game.snake.get_snake_head_y = lambda: 0
    game.game_over = False

    game.game_over_if_wall_hit()

    assert game.game_over is True


def test_game_over_y_wall():
    game = make_game()
    game.snake.get_snake_head_y = lambda: 19
    game.snake.get_snake_head_x = lambda: 10
    game.board.get_board_height = lambda: 20
    game.board.get_board_width = lambda: 60
    game.game_over = False

    game.game_over_if_wall_hit()

    assert game.game_over is True


def test_game_over_x_wall():
    game = make_game()
    game.snake.get_snake_head_y = lambda: 10
    game.snake.get_snake_head_x = lambda: 59
    game.board.get_board_height = lambda: 20
    game.board.get_board_width = lambda: 60
    game.game_over = False

    game.game_over_if_wall_hit()

    assert game.game_over is True


def test_pass_through_left_wall():
    game = make_game()
    game.snake.get_snake_head_x = lambda: 0
    game.snake.get_snake_head_y = lambda: 10
    game.board.get_board_width = lambda: 60
    game.board.get_board_height = lambda: 20
    game.snake.set_snake_head_x = lambda x: setattr(game.snake, "_x", x)

    game.pass_through_if_wall_hit()

    assert game.snake._x == 59


def test_pass_through_top_wall():
    game = make_game()
    game.snake.get_snake_head_x = lambda: 10
    game.snake.get_snake_head_y = lambda: 0
    game.board.get_board_width = lambda: 60
    game.board.get_board_height = lambda: 20
    game.snake.set_snake_head_y = lambda y: setattr(game.snake, "_y", y)

    game.pass_through_if_wall_hit()

    assert game.snake._y == 19


def test_pass_through_right_wall():
    game = make_game()
    game.snake.get_snake_head_x = lambda: 59
    game.snake.get_snake_head_y = lambda: 10
    game.board.get_board_width = lambda: 60
    game.board.get_board_height = lambda: 20
    game.snake.set_snake_head_x = lambda x: setattr(game.snake, "_x", x)

    game.pass_through_if_wall_hit()

    assert game.snake._x == 0


def test_pass_through_bottom_wall():
    game = make_game()
    game.snake.get_snake_head_x = lambda: 10
    game.snake.get_snake_head_y = lambda: 19
    game.board.get_board_width = lambda: 60
    game.board.get_board_height = lambda: 20
    game.snake.set_snake_head_y = lambda y: setattr(game.snake, "_y", y)

    game.pass_through_if_wall_hit()

    assert game.snake._y == 0


def test_end_window(monkeypatch):
    game = make_game()
    called = {"end": False}

    def fake_endwin():
        called["end"] = True

    monkeypatch.setattr("curses.endwin", fake_endwin)
    game.end_window()

    assert called["end"] is True


def test_run_game_calls_components():
    """run_game treba da pozove sve kljucne metode jednom."""
    game = make_game(mode=False)

    calls = []
    game.board.display_fruit = lambda: calls.append("display_fruit")
    game.snake.display_snake = lambda: calls.append("display_snake")
    game.snake.move_position = lambda: calls.append("move_position")
    game.snake.check_tail_collision = lambda: calls.append("check_tail_collision")
    game.snake.jump_snake_position = lambda: calls.append("jump_snake_position")
    game.check_fruit_collision = lambda: calls.append("check_fruit_collision")
    game.set_game_over = lambda: calls.append("set_game_over")
    game.game_over_if_wall_hit = lambda: calls.append("game_over_if_wall_hit")

    game.run_game()

    for expected in ["display_fruit", "display_snake", "move_position",
                     "check_tail_collision", "game_over_if_wall_hit"]:
        assert expected in calls
