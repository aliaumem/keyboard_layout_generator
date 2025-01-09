from dataclasses import dataclass
from typing import NamedTuple, Sequence

import cv2 as cv
import numpy as np
import sys

from finger_keyboard_mapping import keyboard_shape_pb2
from finger_landmarks import finger_landmarks_pb2

Point = NamedTuple("Point", [("x", int), ("y", int)])
Size = NamedTuple("Size", [("width", int), ("height", int)])


@dataclass
class AABB:
    top_left: Point
    size: Size

    @property
    def center(self) -> Point:
        return Point(x=int(self.top_left.x + self.size.width / 2),
                     y=int(self.top_left.y + self.size.height / 2))

    @property
    def bottom_right(self) -> Point:
        return Point(x=self.top_left.x + self.size.width, y=self.top_left.y + self.size.height)

    def __mul__(self, other: float):
        return AABB(Point(int(self.top_left.x * other), int(self.top_left.y * other)),
                    Size(int(self.size.width * other), int(self.size.height * other)))


@dataclass
class Key:
    aabb: AABB
    name: str


def proto_key_to_key(proto_key) -> Key:
    return Key(name=proto_key.name,
               aabb=AABB(Point(proto_key.AABB.x, proto_key.AABB.y),
                         Size(proto_key.AABB.width, proto_key.AABB.height)))


def draw_hand(display, hand, scale_factor):
    draw_finger = lambda finger, color: cv.drawMarker(display, (
        finger.x * scale_factor, finger.y * scale_factor), color,
                                                      cv.MARKER_SQUARE, 2)
    draw_finger(hand.thumb, 240)
    draw_finger(hand.index, 210)
    draw_finger(hand.middle, 180)
    draw_finger(hand.ring, 150)
    draw_finger(hand.pinky, 120)


def print_kb(proto_path: str, fingers_proto_path: str):
    shape = keyboard_shape_pb2.KeyboardShape()
    with open(proto_path, "rb") as f:
        shape.ParseFromString(f.read())

    image = np.zeros((240, 600, 1), dtype="uint8")
    scale_factor = 2

    for proto_key in shape.keys:
        key = proto_key_to_key(proto_key)
        print_key(key, image, scale_factor)

    cv.imshow("kb shape", image)
    cv.waitKey(0)

    fingers = finger_landmarks_pb2.FingerLandmarks()
    with (open(fingers_proto_path, "rb")) as f:
        fingers.ParseFromString(f.read())

    for frame in fingers.frames:
        display = image.copy()
        if frame.leftHand is not None:
            draw_hand(display, frame.leftHand, scale_factor)
        if frame.rightHand is not None:
            draw_hand(display, frame.rightHand, scale_factor)

        cv.imshow("kb shape", display)
        cv.waitKey(33)


def print_key(key: Key, image: np.ndarray, scale_factor: float, font=cv.FONT_HERSHEY_PLAIN,
              font_scale: float = 1, color: Sequence[float] = 70):
    aabb = key.aabb * scale_factor
    cv.rectangle(image, aabb.top_left, aabb.bottom_right, color)

    txt_size, baseline = cv.getTextSize(key.name, font, font_scale, thickness=1)

    center = aabb.center
    text_pos = Point(int(center.x - txt_size[0] / 2), int(center.y + baseline))

    cv.putText(image, key.name, text_pos, font, font_scale, color)


if __name__ == '__main__':
    print_kb(sys.argv[1], sys.argv[2])
