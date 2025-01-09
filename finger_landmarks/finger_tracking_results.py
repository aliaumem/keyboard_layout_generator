from typing import NamedTuple, List

Point = NamedTuple("Point", [("x", int), ("y", int)])
Hand = NamedTuple("Hand", [("thumb", Point), ("index", Point), ("middle", Point), ("ring", Point),
                           ("pinky", Point)])
BothHands = NamedTuple("BothHands",
                       [("timestamp", int), ("left", Hand | None), ("right", Hand | None)])


class Results:
    fps: float
    hands: List[BothHands]

    def __init__(self):
        self.hands = list()

    def add_hands(self, hands: BothHands):
        self.hands.append(hands)
