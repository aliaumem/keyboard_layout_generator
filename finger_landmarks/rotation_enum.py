from enum import Enum

import cv2 as cv


class Rotation(Enum):
    ROTATE_90_CLOCKWISE = cv.ROTATE_90_CLOCKWISE
    ROTATE_90_COUNTERCLOCKWISE = cv.ROTATE_90_COUNTERCLOCKWISE
    ROTATE_180 = cv.ROTATE_180
    ROTATE_IDENTITY = 4


def parse_rotation(rot: str) -> Rotation:
    if rot == "90": return Rotation.ROTATE_90_CLOCKWISE
    if rot == "180": return Rotation.ROTATE_180
    if rot in ("270", "-90"): return Rotation.ROTATE_90_COUNTERCLOCKWISE
    return Rotation.ROTATE_IDENTITY
