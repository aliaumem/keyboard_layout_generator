from typing import NamedTuple, List

Point = NamedTuple("Point", [("x", int), ("y", int)])
Hand = NamedTuple("Hand", [("thumb", Point), ("index", Point), ("middle", Point), ("ring", Point),
                           ("pinky", Point)])
BothHands = NamedTuple("BothHands",
                       [("timestamp", int), ("left", Hand | None), ("right", Hand | None)])


class Results:
    fps: float
    hands: List[BothHands]
    top_left: Point
    top_right: Point
    bottom_right: Point
    bottom_left: Point

    def __init__(self):
        self.hands = list()

    def set_corners(self, top_left: Point, top_right: Point, bottom_right: Point,
                    bottom_left: Point):
        self.top_left = top_left
        self.top_right = top_right
        self.bottom_right = bottom_right
        self.bottom_left = bottom_left

    def add_hands(self, hands: BothHands):
        self.hands.append(hands)
