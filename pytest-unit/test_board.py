import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "../snake"))

from board import Board
from utils import MockWindow


def test_fruit_initial_position():
    win = MockWindow()
    board = Board(win)

    x = board.get_fruit_x()
    y = board.get_fruit_y()

    assert 1 <= x < board.BOARD_WIDTH - 1
    assert 1 <= y < board.BOARD_HEIGHT - 1


def test_display_fruit():
    win = MockWindow()
    board = Board(win)

    board.display_fruit()

    x = board.get_fruit_x()
    y = board.get_fruit_y()

    assert (x, y) in win.chars


def test_update_fruit_position_empty_board(monkeypatch):
    win = MockWindow()
    board = Board(win)

    # kontrola randoma da ne bi bio flaky test
    monkeypatch.setattr("board.randint", lambda a, b: 5)

    board.update_fruit_position()

    x = board.get_fruit_x()
    y = board.get_fruit_y()

    assert 1 <= x <= board.BOARD_WIDTH - 2
    assert 1 <= y <= board.BOARD_HEIGHT - 2


def test_update_fruit_enters_else(monkeypatch):
    """
    Testira else granu u update_fruit_position:
    prvo randint vraca (3,3) sto je zauzeto u MockWindow.inch,
    pa petlja ponovo generise koordinate (5,5) sto je slobodno.
    """
    win = MockWindow()
    board = Board(win)

    sequence = iter([3, 3, 5, 5, 6, 6, 7, 7])
    monkeypatch.setattr("board.randint", lambda a, b: next(sequence))

    board.update_fruit_position()

    assert win.inch(board.get_fruit_y(), board.get_fruit_x()) == ord(" ")
