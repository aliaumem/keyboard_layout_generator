import sys

import cv2

from finger_landmarks import finger_landmarks_pb2
from finger_landmarks.keyboard_shape_recorder import KeyboardShapeRecorder
from finger_landmarks.rotation_enum import Rotation, parse_rotation


def record_kb_corners(video_path: str, rotation: Rotation):
    cap = cv2.VideoCapture(video_path)

    if not cap.isOpened():
        print("Could not open file")
        exit()

    keyboard_rec = KeyboardShapeRecorder()

    window_name = "keyboard_corners"

    ret, frame = cap.read()

    if rotation != Rotation.ROTATE_IDENTITY:
        frame = cv2.rotate(frame, rotation.value)

    cv2.namedWindow(window_name)
    keyboard_rec.record_corners(window_name, frame)

    to_point = lambda index: finger_landmarks_pb2.Point(x=keyboard_rec.points[index][0],
                                                        y=keyboard_rec.points[index][1])

    out_path = video_path.rstrip(".mp4")
    with open(out_path + "_corners.binarypb", "wb") as f:
        corners = finger_landmarks_pb2.KeyboardCorners(
            topLeft=to_point(0),
            topRight=to_point(1),
            bottomRight=to_point(2),
            bottomLeft=to_point(3)
        )
        f.write(corners.SerializeToString())
        f.close()


if __name__ == '__main__':
    rotation = parse_rotation(sys.argv[2]) if len(sys.argv) > 2 else Rotation.ROTATE_IDENTITY

    record_kb_corners(sys.argv[1], rotation)
